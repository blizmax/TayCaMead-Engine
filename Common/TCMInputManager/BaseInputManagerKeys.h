#pragma once

#define INT(a) static_cast<int>(a)
#include <cstdint>

enum class SMouseInput : uint8_t
{
	LMB = 0,
	RMB,
	MMD,
	NO_MOUSE_INPUT,
	COUNT
};

enum class SKey : uint8_t
{
	KEY_0 = 0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,

	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	NO_KEY_INPUT,
	COUNT
};

enum class SSpecialKey : uint8_t
{
	KEY_BACKSPACE = 0,
	KEY_TAB,
	KEY_ENTER,
	KEY_SHIFT,
	KEY_CONTROL,
	KEY_ALT,
	KEY_ESCAPE,
	KEY_SPACEBAR,
	KEY_LEFT,
	KEY_UP,
	KEY_RIGHT,
	KEY_DOWN,
	NO_SPECIAL_KEY_INPUT,
	COUNT
};
