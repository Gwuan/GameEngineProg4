#pragma once
#include <glm.hpp>

struct Rectf final
{
	glm::vec2 LeftBottom = {};
	float width = 0.f;
	float height = 0.f;
};

struct ColorRGBA final
{
	uint8_t r = 0u;
	uint8_t g = 0u;
	uint8_t b = 0u;
	uint8_t a = UINT8_MAX;
};

struct ColorRGB final
{
	uint8_t r = 0u;
	uint8_t g = 0u;
	uint8_t b = 0u;

	ColorRGBA ConvertToRGBA() const
	{
		return ColorRGBA{r, g, b, UINT8_MAX};
	}
};

namespace Colors
{
	constexpr ColorRGBA Red	{UINT8_MAX, 0, 0};
	constexpr ColorRGBA Green	{0, UINT8_MAX, 0};
	constexpr ColorRGBA Blue	{0, 0, UINT8_MAX};
	constexpr ColorRGBA White	{UINT8_MAX, UINT8_MAX, UINT8_MAX};
	constexpr ColorRGBA Black	{0, 0, 0};
}
