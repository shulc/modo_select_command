#pragma once

#include <lx_mesh.hpp>
#include <lx_layer.hpp>

#include <lxu_matrix.hpp>

#include <functional>

#include "mark.h"
#include "vector.h"

class CLxUser_Mesh;
class TPoint;
class TEdge;
class TPolygon;

class TMesh
{
public:
	TMesh(CLxUser_Mesh& mesh, CLxUser_LayerScan& layerScan, unsigned index);
	TMesh(const TMesh& rhs) = delete;
	TMesh& operator=(const TMesh& rhs) = delete;
	TMesh(TMesh&& rhs) = delete;
	void EachPoint(const std::function<void(TPoint&)>& lambda, TMarkMode mode = TMarkMode{});
	void EachEdge(const std::function<void(TEdge&)>& lambda, TMarkMode mode = TMarkMode{});
	void EachPolygon(const std::function<void(TPolygon&)>& lambda, TMarkMode mode = TMarkMode{});

	TMarkMode MarkMode(TMarkMode::ESelect set, TMarkMode::ESelect clear);

	void SetChange();
	void Update();

	CLxUser_Polygon InitPolygon();
	CLxUser_Edge InitEdge();
	CLxUser_Point InitPoint();

	CLxUser_Edge GetEdge(LXtEdgeID e);
	CLxUser_Edge GetEdge(LXtPointID v0, LXtPointID v1);
	CLxUser_Point GetPoint(LXtPointID v);
	CLxUser_Polygon GetPolygon(LXtPolygonID p);

	LXtPointID CreatePoint(const TVectorF& co);
	LXtPolygonID CreatePolygon(std::vector<LXtPointID> points, bool flip = false);

	CLxMatrix4 GetTransform();

	static TMarkModeList GetSetMarkMode();
	static TMarkModeList GetClearMarkMode();

	static TMarkMode ModeNone;
	static TMarkMode ModeHide;
	static TMarkMode ModeHalo;
	static TMarkMode ModeLock;
	static TMarkMode ModeSelect;
	static TMarkMode ModeDelta;
	static TMarkMode ModeUser0;
	static TMarkMode ModeUser1;
	static TMarkMode ModeUser2;
	static TMarkMode ModeUser3;
	static TMarkMode ModeUser4;
	static TMarkMode ModeUser5;
	static TMarkMode ModeUser6;
	static TMarkMode ModeUser7;

	static TMarkMode ModeClearNone;
	static TMarkMode ModeClearHide;
	static TMarkMode ModeClearHalo;
	static TMarkMode ModeClearLock;
	static TMarkMode ModeClearSelect;
	static TMarkMode ModeClearDelta;
	static TMarkMode ModeClearUser0;
	static TMarkMode ModeClearUser1;
	static TMarkMode ModeClearUser2;
	static TMarkMode ModeClearUser3;
	static TMarkMode ModeClearUser4;
	static TMarkMode ModeClearUser5;
	static TMarkMode ModeClearUser6;
	static TMarkMode ModeClearUser7;


	static void InitModes();

private:
	CLxUser_MeshService Service;
	CLxUser_Mesh Mesh;
	CLxUser_LayerScan& LayerScan;
	unsigned Index;
};