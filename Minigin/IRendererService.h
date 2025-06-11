#pragma once
#include <memory>
#include <SDL_ttf.h>
#include <SDL_video.h>

#include "Datatypes.hpp"
#include "ITexture2D.h"
#include "SDLTexture2D.h"
#include "SpriteAnimation.h"

namespace dae
{
	class Font;
}

enum class TextureFlip : uint8_t
{
	None,
	Horizontal,
	Vertical
};

class IRendererService
{
public:
	virtual~ IRendererService() = default;

	virtual void Init(SDL_Window* pWindow) = 0;
	virtual void Render() const = 0;
	virtual void DebugRender() const = 0;
	virtual void Destroy() = 0;

	virtual std::shared_ptr<ITexture2D> LoadTexture(const char* filepath) = 0;
	virtual std::shared_ptr<ITexture2D> CreateFontTexture(const char* text, std::shared_ptr<dae::Font> font, const ColorRGBA& color = Colors::White) = 0;

	virtual void RenderTexture(const ITexture2D& texture, float x, float y) const = 0;
	virtual void RenderTexture(const ITexture2D& texture, const Rectf& dst) const = 0;

	virtual void RenderTextureRegion(const ITexture2D& texture, const Rectf& srcRect, float x, float y) const = 0;
	virtual void RenderTextureRegion(const ITexture2D& texture, const Rectf& srcRect, const Rectf& dstRect, const TextureFlip& flip) const = 0;

	virtual void RenderLine(const glm::vec2& start, const glm::vec2& end, const ColorRGBA& color = Colors::Red) const = 0;

	const ColorRGBA& GetBackgroundColor() const { return m_BackgroundColor; }
	void SetBackgroundColor(const ColorRGBA& color) { m_BackgroundColor = color; }

protected:
	SDL_Window* m_pWindow = nullptr;
	ColorRGBA m_BackgroundColor = Colors::Black;
};