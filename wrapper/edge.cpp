#include "edge.h"
#include "point.h"
#include "polygon.h"
#include "geo_util.h"
#include "mesh.h"

TEdgeId::TEdgeId(unsigned index)
	: Index(index)
{
}

TEdgeId::operator unsigned() const
{
	return Index;
}

TEdge::TEdge(CLxUser_Edge& edge)
	: Edge(edge)
{
}

void TEdge::SetMark(TMarkMode mark)
{
	Edge.SetMarks(mark.Mode);
}

bool TEdge::TestMark(TMarkMode mark) const
{
	return Edge.TestMarks(mark.Mode) == LXe_OK;
}

TEdgeId TEdge::Index() const
{
	unsigned index;
	Edge.Index(&index);
	return TEdgeId(index);
}

LXtEdgeID TEdge::ID() const
{
	return Edge.ID();
}

std::array<LXtPointID, 2> TEdge::EndpointsID() const
{
	LXtPointID p0, p1;
	Edge.Endpoints(&p0, &p1);
	return {p0, p1};
}

TEdge::TPolygonContainer TEdge::Polygons()
{
	return TPolygonContainer(*this);
}

void TEdge::Init(CLxUser_Polygon* polygon) const
{
	CLxUser_Mesh mesh;
	Edge.Mesh(mesh);
	polygon->fromMesh(mesh);
}

void TEdge::Init(CLxUser_Point* point) const
{
	CLxUser_Mesh mesh;
	Edge.Mesh(mesh);
	point->fromMesh(mesh);
}

bool TEdge::Test() const
{
	return Edge.test();
}

unsigned TEdge::PolygonCount() const
{
	unsigned count;
	Edge.PolygonCount(&count);
	return count;
}

unsigned TEdge::Count(CLxUser_Polygon* p) const
{
	return PolygonCount();
}

TPolygon TEdge::Get(CLxUser_Polygon* p, unsigned index)
{
	LXtPolygonID id;
	Edge.PolygonByIndex(index, &id);
	p->Select(id);
	return TPolygon(*p);
}

bool TEdge::CheckNGon() 
{
	for (auto polygon : Polygons()) {
		if (polygon.VertexCount() > 4) {
			return true;
		}
	}
	return false;
}

bool TEdge::IsManifold() const
{
	return PolygonCount() == 2;
}

// return angle in radians.
float TEdge::CalcFaceAngle()
{
	if (IsManifold()) {
		std::array<TVectorF, 2> normals;
		size_t n = 0;
		for (auto polygon : Polygons()) {
			normals[n++] = polygon.NormalF();
		}
		return NGeometry::AngleNormalized(normals[0], normals[1]);
	}

	return Deg2Rad(90.0f);
}

float TEdge::Length() const
{
	return ToVector().len();
}

TVectorF TEdge::ToVector() const
{
	CLxUser_Point userPoint1;
	CLxUser_Point userPoint2;

	Init(&userPoint1);
	Init(&userPoint2);

	auto pointIds = EndpointsID();
	userPoint1.Select(pointIds[0]);
	userPoint2.Select(pointIds[1]);

	return TPoint(userPoint1).Pos() - TPoint(userPoint2).Pos();
}
