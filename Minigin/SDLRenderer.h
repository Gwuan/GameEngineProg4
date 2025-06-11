#pragma once
#include <SDL.h>
#include "Datatypes.hpp"
#include "Singleton.h"
#include <vec2.hpp>

#include "IRendererService.h"
#include "ITexture2D.h"

namespace dae
{
	class SDLTexture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class SDLRenderer final : public IRendererService // public Singleton<Renderer>
	{

	public:
		void Init(SDL_Window* window) override;
		void Render() const override;
		void DebugRender() const override;
		void Destroy() override;


		virtual ~SDLRenderer() override;

		std::shared_ptr<ITexture2D> LoadTexture(const char* filepath) override;
		std::shared_ptr<ITexture2D> CreateFontTexture(const char* text, std::shared_ptr<dae::Font> font, const ColorRGBA& color) override;

		void RenderTexture(const ITexture2D& texture, float x, float y) const override;
		void RenderTexture(const ITexture2D& texture, const Rectf& dst) const override;

		void RenderTextureRegion(const ITexture2D& texture, const Rectf& src, float dstX, float dstY) const override;
		void RenderTextureRegion(const ITexture2D& texture, const Rectf& src, const Rectf& dst, const TextureFlip& flip) const;

		void RenderLine(const glm::vec2& start, const glm::vec2& end, const ColorRGBA& color = Colors::Red) const;

	private:
		static int GetOpenGLDriverIndex();

		bool m_IsImguiDestroyed = false;
		SDL_Renderer* m_RendererBackend = nullptr;
	};

}