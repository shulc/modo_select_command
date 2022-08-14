#include "polygon.h"
#include "point.h"
#include "edge.h"
#include "mesh.h"
#include <cassert>

TPolygonId::TPolygonId(int index)
	: Index(index)
{
}

TPolygonId::operator int() const
{
	return Index;
}


TPolygon::TPolygon(CLxUser_Polygon& polygon)
	: Polygon(polygon)
{
}

LXtPolygonID TPolygon::ID() const
{
	return Polygon.ID();
}

unsigned TPolygon::VertexCount() const
{
	unsigned count;
	Polygon.VertexCount(&count);
	return count;
}

void TPolygon::SetMark(TMarkMode mark)
{
	Polygon.SetMarks(mark.Mode);
}

bool TPolygon::TestMark(TMarkMode mark) const
{
	return Polygon.TestMarks(mark.Mode) == LXe_OK;
}

TPolygonId TPolygon::Index() const
{
	int index;
	Polygon.Index(&index);
	return TPolygonId(index);
}

TVectorD TPolygon::Normal() const
{
	TVectorD normal;
	Polygon.Normal(normal);
	return normal.normalized();
}

TVectorF TPolygon::NormalF() const
{
	TVectorF fnormal(Normal());
	return fnormal;
}

/*
TVectorF TPolygon::CalcNormalF() {
	TVectorF pv;
	size_t n = 0;
	std::vector<CLxUser_Point> points;
	for (auto p : Vertexes()) {
		points.push_back(p.Point);
	}

	for (auto e : Edges()) {
		if (!pv) {
			pv = e.ToVector();
		}
		else
		{
			auto norm = cross(pv, e.ToVector());
			if (!norm.empty()) {
				return norm;
			}
		}
		++n;
	}

	return { 0, 1, 0 };
}
*/

unsigned TPolygon::Count(CLxUser_Point* p) const
{
	return VertexCount();
}

unsigned TPolygon::Count(CLxUser_Edge* e) const
{
	return VertexCount() + 1;
}

TPoint TPolygon::Get(CLxUser_Point* p, unsigned index) const
{
	LXtPointID id;
	Polygon.VertexByIndex(index, &id);
	p->Select(id);
	return TPoint(*p);
}

TEdge TPolygon::Get(CLxUser_Edge* p, unsigned index) const
{
	assert(false);
	return TEdge(*p);
}

void TPolygon::Init(CLxUser_Point* point)
{
	CLxUser_Mesh mesh;
	Polygon.Mesh(mesh);
	point->fromMesh(mesh);
}

void TPolygon::Init(CLxUser_Edge* edge)
{
	CLxUser_Mesh mesh;
	Polygon.Mesh(mesh);
	edge->fromMesh(mesh);
}

TVectorF TPolygon::Center()
{
	TVectorF res;
	res.clear();

	for (auto v : Vertexes()) {
		res += v.Pos();
	}
	res /= VertexCount();

	return res;
}

void TPolygon::Select()
{
	return SetMark(TMesh::ModeSelect);
}

bool TPolygon::Selected() const
{
	return TestMark(TMesh::ModeSelect);
}

void TPolygon::Delete()
{
	// auto log = TLog();
	// log << "TPolygon::Delete(" << ID() << ") = ";
	auto res = Polygon.Remove();
	// log << res;
}

bool TPolygon::operator==(const TPolygon& rhs) const
{
	return Polygon.ID() == rhs.Polygon.ID();
}

bool TPolygon::operator!=(const TPolygon& rhs) const
{
	return !(*this == rhs);
}

bool TPolygon::Test() const
{
	return Polygon.test();
}

TPolygon::TPointContainer TPolygon::Vertexes()
{
	return TPointContainer(*this);
}

TPolygon::TEdgeContainer TPolygon::Edges()
{
	return TEdgeContainer(*this);
}

TPolygon::TEdgeIterator::TEdgeIterator(TPolygon* polygon, CLxUser_Edge* edge)
	: TBase(polygon, edge)
{
	From->Polygon.VertexByIndex(Index++, &PrevPointId);
	From->Polygon.VertexByIndex(Index, &PointId);
}

TPolygon::TEdgeIterator& TPolygon::TEdgeIterator::operator++()
{
	From->Polygon.VertexByIndex(Index++, &PrevPointId);
	From->Polygon.VertexByIndex(Index != Count - 1 ? Index : 0, &PointId);

	return *this;
}

TEdge TPolygon::TEdgeIterator::operator*()
{
	UserData->SelectEndpoints(PrevPointId, PointId);

	return TEdge(*UserData);
}