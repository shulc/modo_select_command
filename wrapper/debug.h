#pragma once

#include "vector.h"

#include <vector>

class CLxUser_StrokeDraw;

class TStroke {
public:
	TVectorF PosA;
	TVectorF PosB;
	TColor Color;
	double Alpha = 1.0;

	void Draw(CLxUser_StrokeDraw& strokeDraw) const;
};

class TEdge;

class TDebug {
private:
	TDebug() {}

public:
	TDebug(const TDebug&) = delete;
	void operator=(const TDebug&) = delete;

	std::vector<TStroke> Strokes;

	void Draw(CLxUser_StrokeDraw& strokeDraw);

	void Add(TEdge& edge, const TColor& color);

	static TDebug& GetInstance();
};

// #define DEBUG_ENABLE

// #ifdef DEBUG_ENABLE
#define DEBUG TDebug::GetInstance()
// #else
// #define DEBUG  / ## /
// #endif // DEBUG_ENABLE

