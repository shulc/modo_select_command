#include "point.h"
#include "edge.h"
#include "polygon.h"

std::ostream& operator<<(std::ostream& os, const TPointId& id)
{
	os << id.Index;
	return os;
}

TPointId::TPointId(unsigned index)
	: Index(index)
{
}

TPointId::operator unsigned() const
{
	return Index;
}

TPointId::TPointId(const TPointId& rhs)
	: Index(rhs.Index)
{
}

TPointId& TPointId::operator=(const TPointId& rhs)
{
	Index = rhs.Index;
	return *this;
}

bool TPointId::operator==(const TPointId& rhs) const
{
	return Index == rhs.Index;
}

bool TPointId::operator!=(const TPointId& rhs) const
{
	return !(*this == rhs);
}

TPoint::TPoint(CLxUser_Point& point)
	: Point(point)
{
}

void TPoint::SetMark(TMarkMode mark)
{
	Point.SetMarks(mark.Mode);
}

bool TPoint::TestMark(TMarkMode mark) const
{
	return Point.TestMarks(mark.Mode) == LXe_OK;
}

TPointId TPoint::Index() const
{
	unsigned index;
	Point.Index(&index);
	return TPointId(index);
}

LXtPointID TPoint::ID() const
{
	return Point.ID();
}

bool TPoint::operator==(const TPoint& rhs) const
{
	return Index() == rhs.Index();
}

bool TPoint::operator!=(const TPoint& rhs) const
{
	return !(*this == rhs);
}

unsigned TPoint::NumEdges() const
{
	unsigned count;
	Point.EdgeCount(&count);
	return count;
}

unsigned TPoint::Count(CLxUser_Edge* e) const
{
	return NumEdges();
}

unsigned TPoint::Count(CLxUser_Polygon* p) const
{
	unsigned count;
	Point.PolygonCount(&count);
	return count;
}

TEdge TPoint::Get(CLxUser_Edge* e, unsigned index) const
{
	LXtEdgeID id;
	Point.EdgeByIndex(index, &id);
	e->Select(id);
	return TEdge(*e);
}

TPolygon TPoint::Get(CLxUser_Polygon* p, unsigned index) const
{
	LXtPolygonID id;
	Point.PolygonByIndex(index, &id);
	p->Select(id);
	return TPolygon(*p);
}

void TPoint::Init(CLxUser_Polygon* polygon)
{
	CLxUser_Mesh mesh;
	Point.Mesh(mesh);
	polygon->fromMesh(mesh);
}

void TPoint::Init(CLxUser_Edge* edge)
{
	CLxUser_Mesh mesh;
	Point.Mesh(mesh);
	edge->fromMesh(mesh);
}

TPoint::TEdgeContainer TPoint::Edges()
{
	return TEdgeContainer(*this);
}

TPoint::TPolygonContainer TPoint::Polygons()
{
	return TPolygonContainer(*this);
}

TVectorF TPoint::Pos() const
{
	TVectorF pos;
	Point.Pos(pos);
	return pos;
}

void TPoint::Pos(const TVectorF& pos)
{
	TVectorD posDouble(pos);
	Point.SetPos(posDouble);
}

bool TPoint::Test() const
{
	return Point.test();
}
