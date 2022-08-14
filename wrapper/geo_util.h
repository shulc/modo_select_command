#pragma once

#include "vector.h"

#include <optional>

#include <lxmesh.h>

#define _USE_MATH_DEFINES
#include <math.h>

template<typename T>
constexpr T Deg2Rad(T angle) {
	return angle * static_cast<T>(M_PI / 180.0);
}

template<typename T>
constexpr T Rad2Deg(T angle) {
	return angle * 180.0 * M_1_PI;
}

class TEdge;
class TPoint;

namespace NGeometry {

	std::optional<TVectorF> IntersectLinePlane(const TVectorF& lineA, const TVectorF& lineB, const TVectorF& planeCo, const TVectorF& planeNo);
	int IntersectLineLine(const TVectorF& v1, const TVectorF& v2, const TVectorF& v3, const TVectorF& v4, TVectorF& res1, TVectorF& res2);
	std::optional<std::array<TVectorF, 2>> IntersectLineLine(const TVectorF& v1, const TVectorF& v2, const TVectorF& v3, const TVectorF& v4);

	TVectorF IntersectPointLine(const TVectorF& pt, const TVectorF& lineP1, const TVectorF& lineP2);

	float AngleNormalized(const TVectorF& v1, const TVectorF& v2);
	float ClosestToLine(TVectorF& rClose, const TVectorF& p, const TVectorF& l1, const TVectorF& l2);
	float ClosestToRay(TVectorF& rClose, const TVectorF& p, const TVectorF& rayOrig, const TVectorF& rayDir);

	LXtPointID OtherPointIdFromEdge(LXtPointID pointId, TEdge& edge);
	LXtPointID OtherPointFromEdge(TPoint& point, TEdge& edge);

	float GetAngleBetweenEdges(TEdge& e1, TEdge& e2);

	float VectorAngle(const TVectorF& v1, const TVectorF& v2);

	std::vector<TVectorF> InterpolateBezier(TVectorF knot1, TVectorF handle1, TVectorF handle2, TVectorF knot2, unsigned resolution);

} // namespace NGeometry