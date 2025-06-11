#pragma once
#include <vec2.hpp>

class ITexture2D
{
public:
	virtual ~ITexture2D() = default;
	virtual glm::ivec2 GetSize() const = 0;
};