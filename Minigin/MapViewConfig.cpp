#include "MapViewConfig.h"

#include "IRendererService.h"
#include "ServiceAllocator.h"

MapViewConfig::MapViewConfig(const std::vector<std::vector<Rectf>>& worldGrid, float cellSize)
{
	if (worldGrid.empty() || worldGrid[0].empty())
		return;

	const float mapHeight = std::ssize(worldGrid) * cellSize;
	const float mapWidth = std::size(worldGrid[0]) * cellSize;

	const auto& windowSize = ServiceAllocator::GetRenderer().GetWindowSize();

	// Scale
	const float scaleX = (windowSize.x * m_ScreenCoverage) / mapWidth;
	const float scaleY = (windowSize.y * m_ScreenCoverage) / mapHeight;

	m_Scale = std::min(scaleX, scaleY);

	// Offset
	m_Offset.x = (windowSize.x - mapWidth  * m_Scale) * .5f;
	m_Offset.y = (windowSize.y - mapHeight * m_Scale) * .5f;
}

Rectf MapViewConfig::ApplyView(const Rectf& src) const
{
	return Rectf{
		{
			src.LeftBottom.x * m_Scale + m_Offset.x,
			src.LeftBottom.y * m_Scale + m_Offset.y,

		},
		src.width * m_Scale,
		src.height * m_Scale
	};
}

glm::vec2 MapViewConfig::ApplyView(const glm::vec2& src) const
{
	return glm::vec2
	{
		src.x * m_Scale + m_Offset.x,
		src.y * m_Scale - m_Offset.y
	};
}