#pragma once

#include <lx_mesh.hpp>

#include "mark.h"
#include "iterator.h"
#include "vector.h"

#include <array>
#include <memory>

class TEdgeId
{
public:
	explicit TEdgeId(unsigned index);
	operator unsigned() const;

private:
	unsigned Index;
};

class TPolygon;
class TPoint;
class TMesh;

class TEdge
{
public:
	using TUserData = CLxUser_Edge;

	explicit TEdge(CLxUser_Edge& edge);
	TEdge(const TEdge& rhs) = delete;
	TEdge& operator=(const TEdge& rhs) = delete;
	TEdge(TEdge&& rhs) = delete;

	void SetMark(TMarkMode mark);
	bool TestMark(TMarkMode mark) const;
	TEdgeId Index() const;
	LXtEdgeID ID() const;
	std::array<LXtPointID, 2> EndpointsID() const;

	using TPolygonIterator = TIterator<TEdge, TPolygon>;
	using TPolygonContainer = TContainer<TEdge, TPolygon>;

	unsigned PolygonCount() const;
	unsigned Count(CLxUser_Polygon* p) const;
	TPolygon Get(CLxUser_Polygon* p, unsigned index);

	bool CheckNGon();
	bool IsManifold() const;
	float CalcFaceAngle();
	float Length() const;
	TVectorF ToVector() const;

	TPolygonContainer Polygons();

	void Init(CLxUser_Polygon* polygon) const;
	void Init(CLxUser_Point* point) const;

	bool Test() const;
private:
	CLxUser_Edge& Edge;
};