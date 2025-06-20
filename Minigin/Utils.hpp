#pragma once
#include "DataTypes.hpp"

inline bool AlmostZero(float value)
{
	return (value < 0.01f && value > -0.01f);
}

inline bool IsPointInRectf(const glm::vec2& point, const Rectf& rect)
{
    return point.x >= rect.LeftBottom.x					&&
           point.x <= rect.LeftBottom.x + rect.width	&&
           point.y >= rect.LeftBottom.y					&&
           point.y <= rect.LeftBottom.y + rect.height;
}

namespace HashUtils
{
	template <int length>
	struct sdbm_hash
	{
		consteval static unsigned int _calculate(const char* const text, unsigned int& value)
		{
			const unsigned int character = sdbm_hash<length - 1>::_calculate(text, value);
			value = character + (value << 6) + (value << 16) - value;
			return text[length - 1];
		}

		consteval static unsigned int calculate(const char* const text)
		{
			unsigned int value = 0;
			const auto character = _calculate(text, value);
			return character + (value << 6) + (value << 16) - value;
		}
	};

	template <>
	struct sdbm_hash<1>
	{
		consteval static int _calculate(const char* const text, unsigned int&) { return text[0]; }
	};

	template <size_t N>
	consteval unsigned int make_sdbm_hash(const char (&text)[N])
	{
		return sdbm_hash<N - 1>::calculate(text);
	}
	
}