#pragma once

namespace Client
{

	enum class UI_USE_CONDITION
	{
		UI_USE_NONE = 0,
		UI_USE_ALPHA = 1 << 1,
		UI_USE_SIZE = 1 << 2,
		UI_USE_TRANSFORM = 1 << 3,
		UI_USE_ROTATE = 1 << 4,
		UI_USE_ALL = 1 << 5
	};

	enum class UI_EFFECT_CONDITION
	{
		UI_NONE = 0,
		UI_FROM_ORIGIN_POS = 1 << 1,
		UI_FROM_ORIGIN_ALPHA = 1 << 2,
		UI_FROM_ORIGIN_SIZE = 1 << 3,
		UI_FROM_ORIGIN_ROTATE = 1 << 4,
	};

	enum class UI_EFFECT_ANIM_STATE
	{
		PLAY,
		REPEAT,
		STOP
	};

}