#pragma once

#include <lxmesh.h>

#include <array>

class CLxUser_MeshService;

class TMarkMode
{
public:
	enum class ESelect
	{
		None,
		Hide,
		Halo,
		Lock,
		Select,
		Delta,
		User0,
		User1,
		User2,
		User3,
		User4,
		User5,
		User6,
		User7,
		MAX,
	};

	TMarkMode() = default;
	TMarkMode(CLxUser_MeshService&, ESelect set, ESelect clear);
	LXtMarkMode Mode{};

private:
	const char* GetName(ESelect select);

};

using TMarkModeList = std::array<TMarkMode, static_cast<size_t>(TMarkMode::ESelect::MAX)>;