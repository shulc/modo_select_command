#pragma once

#include <lx_mesh.hpp>

#include "mark.h"
#include "iterator.h"
#include "vector.h"
#include "log.h"

class TPointId
{
public:
	explicit TPointId(unsigned index);
	operator unsigned() const;
	TPointId(const TPointId& rhs);
	TPointId& operator=(const TPointId& rhs);

	bool operator==(const TPointId& rhs) const;
	bool operator!=(const TPointId& rhs) const;

public:
	unsigned Index;
};


class TEdge;
class TPolygon;

class TPoint
{
public:
	using TUserData = CLxUser_Point;

	explicit TPoint(CLxUser_Point& point);
	TPoint(const TPoint& rhs) = delete;
	TPoint & operator=(const TPoint & rhs) = delete;
	TPoint(TPoint&& rhs) = delete;

	void SetMark(TMarkMode mark);
	bool TestMark(TMarkMode mark) const;

	TPointId Index() const;
	LXtPointID ID() const;

	bool operator==(const TPoint& rhs) const;
	bool operator!=(const TPoint& rhs) const;


	using TEdgeIterator = TIterator<TPoint, TEdge>;
	using TEdgeContainer = TContainer<TPoint, TEdge>;
	using TPolygonIterator = TIterator<TPoint, TPolygon>;
	using TPolygonContainer = TContainer<TPoint, TPolygon>;

	unsigned NumEdges() const;
	unsigned Count(CLxUser_Edge* e) const;
	unsigned Count(CLxUser_Polygon* p) const;
	TEdge Get(CLxUser_Edge* e, unsigned index) const;
	TPolygon Get(CLxUser_Polygon* p, unsigned index) const;
	void Init(CLxUser_Polygon* polygon);
	void Init(CLxUser_Edge* edge);

	TEdgeContainer Edges();
	TPolygonContainer Polygons();

	TVectorF Pos() const;
	void Pos(const TVectorF& pos);

	bool Test() const;

private:
	CLxUser_Point& Point;
};