#include "geo_util.h"

#include "edge.h"
#include "point.h"

#include <limits>
#include <cassert>
#include <vector>

namespace {
	float saasin(float fac) {
		if (fac <= -1.0f) {
			return -M_PI_2;
		}
		else if (fac >= 1.0f) {
			return M_PI_2;

		}

		return asinf(fac);
	}

	float saacos(float fac) {
		if (fac <= -1.0f) {
			return M_PI;
		}
		else if (fac >= 1.0f) {
			return 0.0f;
		}
		else {
			return acosf(fac);
		}
	}
} // anonymous namespace

std::optional<TVectorF> NGeometry::IntersectLinePlane(const TVectorF & lineA, const TVectorF & lineB, const TVectorF & planeCo, const TVectorF & planeNo)
{
	const TVectorF u = lineB - lineA;
	const TVectorF h = lineA - planeCo;
	// u = lineB - lineA
	// h = lineA - planeCo
	// dot = dot(planeNo, u)
	// check dot....
	// lambda = - dot(planeNo, h) / dot
	// result = lineA + u * lambda

	const float d = dot(planeNo, u);
	if (std::abs(d) < std::numeric_limits<float>::epsilon()) {
		return {};
	}

	const float lambda = - dot(planeNo,h) / d;

	return lineA + u * lambda;
}

int NGeometry::IntersectLineLine(const TVectorF& v1, const TVectorF& v2, const TVectorF& v3, const TVectorF& v4, TVectorF& res1, TVectorF& res2)
{
	constexpr float epsilon = 0.000001f;

	TVectorF c = v3 - v1;
	TVectorF a = v2 - v1;
	TVectorF b = v4 - v3;

	TVectorF ab = cross(a, b);
	float div = dot(ab, ab);

	// important not to use an epsilon here, see: T45919
	// test zero length line
	if (div == 0.0f) {
		return 0;
	}

	// test if the two lines are coplanar
	if (fabs(div) < epsilon) {
		const TVectorF cb = c - b;
		a *= dot(cb, ab) / div;
		res1 = res2 = v1 + a;

		// one intersection only
		return 1;
	}
	// if not
	TVectorF t = v1 - v3;

	// offset between both plane where the lines lies
	const TVectorF n = cross(a, b);
	t = project(t, n);
	const TVectorF v3t = v3 + t;
	const TVectorF v4t = v4 + t;

	// for the first line, offset the second line until it is coplanar
	c = v3t - v1;
	a = v2 - v1;
	b = v4t - v3t;
	
	ab = cross(a, b);
	TVectorF cb = cross(c, b);

	a *= (dot(cb, ab) / dot(ab, ab));

	res1 = v1 + a;

	// for the second line, just subtract the offset from the first intersection point
	res2 = res1 - t;

	// two nearest points
	return 2;
}

std::optional<std::array<TVectorF, 2>> NGeometry::IntersectLineLine(const TVectorF& v1, const TVectorF& v2, const TVectorF& v3, const TVectorF& v4)
{
	std::array<TVectorF, 2> i;
	const int res = IntersectLineLine(v1, v2, v3, v4, i[0], i[1]);
	if (res == 1) {
		ClosestToLine(i[1], i[0], v3, v4);
	}

	if (res == 0) {
		return {};
	}

	return { std::move(i) };
}

TVectorF NGeometry::IntersectPointLine(const TVectorF& pt, const TVectorF& lineP1, const TVectorF& lineP2)
{
	const TVectorF u = lineP2 - lineP1;
	TVectorF rClose;
	ClosestToRay(rClose, pt, lineP1, u);
	return rClose;
}

float NGeometry::AngleNormalized(const TVectorF& v1, const TVectorF& v2)
{
	auto a = v1;
	auto b = v2;

	a.normalized();
	b.normalized();

	if (dot(a, b) >= 0.0f) {
		return 2.0f * saasin((b - a).len() / 2.0f);
	}

	auto negB = -b;

	return M_PI - 2.0f * saasin((a - negB).len() / 2.0f);
}

float NGeometry::ClosestToLine(TVectorF& rClose, const TVectorF& p, const TVectorF& l1, const TVectorF& l2)
{
	const TVectorF u = l2 - l1;
	return ClosestToRay(rClose, p, l1, u);
}

float NGeometry::ClosestToRay(TVectorF& rClose, const TVectorF& p, const TVectorF& rayOrig, const TVectorF& rayDir)
{
	if (rayDir.empty()) {
		rClose = rayOrig;
		return 0.0f;
	}

	const TVectorF h = p - rayOrig;
	const float lambda = dot(rayDir, h) / dot(rayDir, rayDir);
	rClose = rayOrig + rayDir * lambda;
	return lambda;
}

LXtPointID NGeometry::OtherPointFromEdge(TPoint& point, TEdge& edge) {
	return OtherPointIdFromEdge(point.ID(), edge);
}

LXtPointID NGeometry::OtherPointIdFromEdge(LXtPointID pointId, TEdge& edge) {
	auto points = edge.EndpointsID();
	if (pointId == points[0]) {
		return points[1];
	}
	if (pointId == points[1]) {
		return points[0];
	}

	return nullptr;
}

TVectorF OtherPos(TPoint& point, TEdge& edge) {
	auto otherPointId = NGeometry::OtherPointFromEdge(point, edge);

	assert(otherPointId);

	CLxUser_Point pointUser;
	edge.Init(&pointUser);
	pointUser.Select(otherPointId);

	return TPoint(pointUser).Pos();
}

// return angle in degree
float NGeometry::GetAngleBetweenEdges(TEdge& e1, TEdge& e2)
{
	LXtPointID centerVert = nullptr;
	

	for (auto pointFromEdge1 : e1.EndpointsID()) {
		for (auto pointFromEdge2 : e2.EndpointsID()) {
			if (pointFromEdge1 == pointFromEdge2) {
				centerVert = pointFromEdge1;
			}
		}
	}

	TVectorF vec1;
	TVectorF vec2;

	if (centerVert) {
		CLxUser_Point pointUser;
		e1.Init(&pointUser);

		pointUser.Select(centerVert);

		TPoint centerPoint(pointUser);

		vec1 = centerPoint.Pos() - OtherPos(centerPoint, e1);
		vec2 = centerPoint.Pos() - OtherPos(centerPoint, e2);
	}
	else {
		vec1 = e1.ToVector();
		vec2 = e2.ToVector();
	}

	return Rad2Deg(VectorAngle(vec1, vec2));
}

float NGeometry::VectorAngle(const TVectorF& v1, const TVectorF& v2)
{
	return saacos(dot(v1,v2) / (sqrt(dot(v1, v1) * dot(v2, v2))));
}

std::vector<TVectorF> NGeometry::InterpolateBezier(
	TVectorF q0,
	TVectorF q1,
	TVectorF q2,
	TVectorF q3,
	unsigned it)
{
	float f = static_cast<float>(it);
	const TVectorF rt0 = q0;
	const TVectorF rt1 = 3.0f * (q1 - q0) / f;
	f *= f;
	const TVectorF rt2 = 3.0f * (q0 - 2.0f * q1 + q2) / f;
	f *= it;
	const TVectorF rt3 = (q3 - q0 + 3.0f * (q1 - q2)) / f;

	q0 = rt0;
	q1 = rt1 + rt2 + rt3;
	q2 = 2 * rt2 + 6 * rt3;
	q3 = 6 * rt3;

	std::vector<TVectorF> p;
	for (unsigned a = 0; a <= it; ++a) {
		p.push_back(q0);
		q0 += q1;
		q1 += q2;
		q2 += q3;
	}

	return p;
}
