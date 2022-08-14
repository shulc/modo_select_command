#include "debug.h"
#include "log.h"
#include "edge.h"
#include "point.h"

#include <lx_draw.hpp>

void TStroke::Draw(CLxUser_StrokeDraw& strokeDraw) const
{
#ifndef DEBUG_ENABLE
	strokeDraw.Begin(LXiSTROKE_LINES, Color, Alpha);
	strokeDraw.Vert(PosA[0], PosA[1], PosA[2], LXiSTROKE_ABSOLUTE);
	strokeDraw.Vert(PosB[0], PosB[1], PosB[2], LXiSTROKE_ABSOLUTE);

	// TLog() << "Draw: " << PosA << " - " << PosB;
#endif
}

void TDebug::Draw(CLxUser_StrokeDraw& strokeDraw)
{
#ifndef DEBUG_ENABLE
	for (const auto& stroke : Strokes) {
		stroke.Draw(strokeDraw);
	}
#endif
}

void TDebug::Add(TEdge& edge, const TColor& color)
{
#ifndef DEBUG_ENABLE
	const auto pointIDs = edge.EndpointsID();
	CLxUser_Point p0, p1;
	edge.Init(&p0);
	edge.Init(&p1);
	p0.Select(pointIDs[0]);
	p1.Select(pointIDs[1]);

	Strokes.push_back({ TPoint(p0).Pos(), TPoint(p1).Pos(), color });
#endif
}

TDebug& TDebug::GetInstance()
{
	static TDebug debug;

	return debug;
}
