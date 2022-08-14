#include "mark.h"

#include <lx_mesh.hpp>

TMarkMode::TMarkMode(CLxUser_MeshService& service, ESelect set, ESelect clear)
{
	service.ModeCompose(GetName(set), GetName(clear), &Mode);
}

const char* TMarkMode::GetName(ESelect select)
{
	switch (select) {
	case ESelect::None:
		return nullptr;
	case ESelect::Hide:
		return LXsMARK_HIDE;
	case ESelect::Halo:
		return LXsMARK_HALO;
	case ESelect::Lock:
		return LXsMARK_LOCK;
	case ESelect::Select:
		return LXsMARK_SELECT;
	case ESelect::Delta:
		return LXsMARK_DELTA;
	case ESelect::User0:
		return LXsMARK_USER_0;
	case ESelect::User1:
		return LXsMARK_USER_1;
	case ESelect::User2:
		return LXsMARK_USER_2;
	case ESelect::User3:
		return LXsMARK_USER_3;
	case ESelect::User4:
		return LXsMARK_USER_4;
	case ESelect::User5:
		return LXsMARK_USER_6;
	case ESelect::User6:
		return LXsMARK_USER_6;
	case ESelect::User7:
		return LXsMARK_USER_7;
	}
	return nullptr;
}