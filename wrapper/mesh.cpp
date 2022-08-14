#include "mesh.h"
#include "point.h"
#include "edge.h"
#include "polygon.h"

#include <memory>
#include <cassert>
#include <vector>

namespace {
	template<typename T, typename TInternal>
	class TVisitor : public CLxVisitor {
	public:
		TVisitor(const std::function<void(T&)>& l, TInternal& item)
			: Lambda(l)
			, Item(item)
		{
		}

		void evaluate() final {
			T item(Item);
			Lambda(item);
		}

	private:
		const std::function<void(T&)>& Lambda;
		TInternal& Item;
	};

	using TPointVisitor = TVisitor<TPoint, CLxUser_Point>;
	using TEdgeVisitor = TVisitor<TEdge, CLxUser_Edge>;
	using TPolygonVisitor = TVisitor<TPolygon, CLxUser_Polygon>;
} // anonymous namespace 


TMesh::TMesh(CLxUser_Mesh& mesh, CLxUser_LayerScan& layerScan, unsigned index)
	: Mesh(mesh)
	, LayerScan(layerScan)
	, Index(index)
{
}

void TMesh::EachPolygon(const std::function<void(TPolygon&)>& lambda, TMarkMode mode)
{
	auto polygon = InitPolygon();
	TPolygonVisitor vis(lambda, polygon);

	polygon.Enumerate(mode.Mode, vis, 0);
}

void TMesh::EachPoint(const std::function<void(TPoint&)>& lambda, TMarkMode mode)
{
	auto point = InitPoint();
	TPointVisitor vis(lambda, point);

	point.Enumerate(mode.Mode, vis, 0);
}

void TMesh::EachEdge(const std::function<void(TEdge&)>& lambda, TMarkMode mode)
{
	auto edge = InitEdge();
	TEdgeVisitor vis(lambda, edge);

	edge.Enumerate(mode.Mode, vis, 0);
}

TMarkMode TMesh::MarkMode(TMarkMode::ESelect set, TMarkMode::ESelect clear)
{
	return TMarkMode(Service, set, clear);
}

void TMesh::SetChange()
{
	LayerScan.SetMeshChange(Index, LXf_MESHEDIT_GEOMETRY);
}

void TMesh::Update()
{
	LayerScan.Update();
}

CLxUser_Polygon TMesh::InitPolygon()
{
	CLxUser_Polygon p;
	p.fromMesh(Mesh);
	return p;
}

CLxUser_Edge TMesh::InitEdge()
{
	CLxUser_Edge e;
	e.fromMesh(Mesh);
	return e;
}

CLxUser_Point TMesh::InitPoint()
{
	CLxUser_Point p;
	p.fromMesh(Mesh);
	return p;
}

CLxUser_Edge TMesh::GetEdge(LXtEdgeID edge)
{
	CLxUser_Edge e;
	e.fromMesh(Mesh);
	e.Select(edge);

	return e;
}

CLxUser_Edge TMesh::GetEdge(LXtPointID v0, LXtPointID v1)
{
	CLxUser_Edge e;
	e.fromMesh(Mesh);
	if (e.SelectEndpoints(v0, v1) == LXe_OK) {
		return e;
	}
	return nullptr;
}

CLxUser_Point TMesh::GetPoint(LXtPointID v)
{
	CLxUser_Point p;
	p.fromMesh(Mesh);
	p.Select(v);
	return p;
}

CLxUser_Polygon TMesh::GetPolygon(LXtPolygonID v)
{
	CLxUser_Polygon p;
	p.fromMesh(Mesh);
	p.Select(v);
	return p;
}

LXtPointID TMesh::CreatePoint(const TVectorF& co)
{
	CLxUser_Point point;
	point.fromMesh(Mesh);
	LXtPointID v;
	TVectorD cod(co);
	point.New(cod, &v);
	return v;
}

LXtPolygonID TMesh::CreatePolygon(std::vector<LXtPointID> points, bool flip)
{
	CLxUser_Polygon polygon;
	polygon.fromMesh(Mesh);
	polygon.SelectByIndex(0);
	LXtPolygonID id;
	LXtID4 type;
	polygon.Type(&type);

	if (flip) {
		std::reverse(begin(points), end(points));
	}

	auto createResult = polygon.New(type, &points[0], points.size(), 0, &id);
	return id;
}

CLxMatrix4 TMesh::GetTransform()
{
	CLxMatrix4 matrix;
	LayerScan.MeshTransform(Index, matrix);
	return matrix;
}

TMarkModeList TMesh::GetSetMarkMode()
{
	CLxUser_MeshService meshService;
	return {
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::Hide, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::Halo, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::Lock, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::Select, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::Delta, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::User0, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::User1, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::User2, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::User3, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::User4, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::User5, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::User6, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::User7, TMarkMode::ESelect::None),
	};
}

TMarkModeList TMesh::GetClearMarkMode()
{
	CLxUser_MeshService meshService;
	return {
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::None),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::Hide),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::Halo),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::Lock),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::Select),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::Delta),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::User0),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::User1),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::User2),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::User3),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::User4),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::User5),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::User6),
		TMarkMode(meshService, TMarkMode::ESelect::None, TMarkMode::ESelect::User7),
	};
}

template<std::size_t... I, typename U>
constexpr auto f(const U& arr, std::index_sequence<I...>) {
	return std::make_tuple(arr[I]...);
}

template<typename T, std::size_t N>
constexpr auto f(const T(&arr)[N]) {
	return f(arr, std::make_index_sequence<N>{});
}

template<typename T, std::size_t N>
constexpr auto f(const std::array<T, N>& arr) {
	return f(arr, std::make_index_sequence<N>{});
}


void TMesh::InitModes()
{
	std::tie(
		ModeNone,
		ModeHide,
		ModeHalo,
		ModeLock,
		ModeSelect,
		ModeDelta,
		ModeUser0,
		ModeUser1,
		ModeUser2,
		ModeUser3,
		ModeUser4,
		ModeUser5,
		ModeUser6,
		ModeUser7
	) = f(TMesh::GetSetMarkMode());

	std::tie(
		ModeClearNone,
		ModeClearHide,
		ModeClearHalo,
		ModeClearLock,
		ModeClearSelect,
		ModeClearDelta,
		ModeClearUser0,
		ModeClearUser1,
		ModeClearUser2,
		ModeClearUser3,
		ModeClearUser4,
		ModeClearUser5,
		ModeClearUser6,
		ModeClearUser7
	) = f(TMesh::GetClearMarkMode());
}


TMarkMode TMesh::ModeNone = {};
TMarkMode TMesh::ModeHide = {};
TMarkMode TMesh::ModeHalo = {};
TMarkMode TMesh::ModeLock = {};
TMarkMode TMesh::ModeSelect = {};
TMarkMode TMesh::ModeDelta = {};
TMarkMode TMesh::ModeUser0 = {};
TMarkMode TMesh::ModeUser1 = {};
TMarkMode TMesh::ModeUser2 = {};
TMarkMode TMesh::ModeUser3 = {};
TMarkMode TMesh::ModeUser4 = {};
TMarkMode TMesh::ModeUser5 = {};
TMarkMode TMesh::ModeUser6 = {};
TMarkMode TMesh::ModeUser7 = {};

TMarkMode TMesh::ModeClearNone = {};
TMarkMode TMesh::ModeClearHide = {};
TMarkMode TMesh::ModeClearHalo = {};
TMarkMode TMesh::ModeClearLock = {};
TMarkMode TMesh::ModeClearSelect = {};
TMarkMode TMesh::ModeClearDelta = {};
TMarkMode TMesh::ModeClearUser0 = {};
TMarkMode TMesh::ModeClearUser1 = {};
TMarkMode TMesh::ModeClearUser2 = {};
TMarkMode TMesh::ModeClearUser3 = {};
TMarkMode TMesh::ModeClearUser4 = {};
TMarkMode TMesh::ModeClearUser5 = {};
TMarkMode TMesh::ModeClearUser6 = {};
TMarkMode TMesh::ModeClearUser7 = {};