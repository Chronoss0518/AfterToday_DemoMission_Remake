#pragma once

constexpr unsigned char DIRECTION_MAX_COUNT = 6;

enum class Direction : unsigned char
{
	Front,
	Back,
	Right,
	Left,
	Up,
	Down
};
