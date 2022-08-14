#pragma once

#include <lx_mesh.hpp>

#include "mark.h"
#include "iterator.h"
#include "vector.h"
#include "edge.h"

#include <memory>

class TPoint;
// class TEdge;

class TPolygonId
{
public:
	explicit TPolygonId(int index);
	operator int() const;

private:
	int Index;
};

class TPolygon
{
public:
	using TUserData = CLxUser_Polygon;

	explicit TPolygon(CLxUser_Polygon& polygon);
	TPolygon(const TPolygon& rhs) = delete;
	TPolygon& operator=(const TPolygon& rhs) = delete;
	TPolygon(TPolygon&& rhs) = delete;

	LXtPolygonID ID() const;

	unsigned VertexCount() const;

	using TPointIterator = TIterator<TPolygon, TPoint>;
	using TPointContainer = TContainer<TPolygon, TPoint>;

	class TEdgeIterator : public TIterator<TPolygon, TEdge>
	{
	public:
		using TBase = TIterator<TPolygon, TEdge>;
		TEdgeIterator(TPolygon* polygon, CLxUser_Edge* edge);
		TEdgeIterator& operator++();
		TEdge operator*();

	private:
		LXtPointID PointId;
		LXtPointID PrevPointId;
	};

	using TEdgeContainer = TContainer<TPolygon, TEdge, TEdgeIterator>;

	TPointContainer Vertexes();
	TEdgeContainer Edges();

	void SetMark(TMarkMode mark);
	bool TestMark(TMarkMode mark) const;
	TPolygonId Index() const;

	TVectorD Normal() const;
	TVectorF NormalF() const;
	TVectorF CalcNormalF();

	unsigned Count(CLxUser_Point* p) const;
	unsigned Count(CLxUser_Edge* e) const;
	TPoint Get(CLxUser_Point* p, unsigned index) const;
	TEdge Get(CLxUser_Edge* p, unsigned index) const;
	void Init(CLxUser_Point* point);
	void Init(CLxUser_Edge* edge);

	TVectorF Center();

	void Select();
	bool Selected() const;

	void Delete();

	bool operator==(const TPolygon& rhs) const;
	bool operator!=(const TPolygon& rhs) const;

	bool Test() const;

private:
	CLxUser_Polygon& Polygon;


};