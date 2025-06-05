#pragma once
#include <SDL.h>
#include <vec2.hpp>

#include "Singleton.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void DebugRender();
		void RenderUI();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		void RenderTextureRegion(const Texture2D& texture, const SDL_Rect& sourceRect, float x, float y) const;
		void RenderTextureRegion(const Texture2D& texture, const SDL_Rect& sourceRect, float x, float y, float width, float height) const;

		void RenderLine(const glm::vec2& start, const glm::vec2& end) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

