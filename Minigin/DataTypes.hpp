#pragma once
#include <array>
#include <glm.hpp>

struct Rectf final
{
	glm::vec2 LeftBottom = {};
	float width = 0.f;
	float height = 0.f;

	std::array<glm::vec2, 4> GetVertices() const
	{
		std::array<glm::vec2, 4> result;

		result[0] = this->LeftBottom;
		result[1] = {
			this->LeftBottom.x + this->width,
			this->LeftBottom.y
		};
		result[2] = {
			this->LeftBottom.x + this->width,
			this->LeftBottom.y + this->height
		};
		result[3] = {
			this->LeftBottom.x,
			this->LeftBottom.y + this->height
		};

		return result;
	}

	glm::vec2 GetCenterPoint() const
	{
		return glm::vec2{
			this->LeftBottom.x + (this->width / 2),
			this->LeftBottom.y + (this->height / 2)
		};
	}
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
