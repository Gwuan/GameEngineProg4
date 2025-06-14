#pragma once
#include <vector>

#include "DataTypes.hpp"

class MapViewConfig final
{
public:
	explicit MapViewConfig(const std::vector<std::vector<Rectf>>& worldGrid, float cellSize);
	~MapViewConfig() = default;

	MapViewConfig(const MapViewConfig&) = delete;
	MapViewConfig(MapViewConfig&&) noexcept = delete;
	MapViewConfig& operator=(const MapViewConfig&) = delete;
	MapViewConfig& operator=(MapViewConfig&&) noexcept = delete;

	Rectf ApplyView(const Rectf& src) const;
	glm::vec2 ApplyView(const glm::vec2& src) const;

	float GetScale()		const { return this->m_Scale;  }
	glm::vec2 GetOffset()	const { return this->m_Offset; }

private:
	glm::vec2 m_Offset = {};
	float m_Scale = 1.f;

	static constexpr float m_ScreenCoverage = 0.9f;
};