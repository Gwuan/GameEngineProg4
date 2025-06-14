#pragma once
#include <vec2.hpp>

namespace glm {
    inline bool operator<(const ivec2& lhs, const ivec2& rhs) {
        return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
    }
}

struct AINode final
{
	glm::ivec2 pos{};
	AINode* parent = nullptr;

	AINode(glm::ivec2 p, AINode* parent = nullptr)
            : pos(p), parent(parent) {}
};
