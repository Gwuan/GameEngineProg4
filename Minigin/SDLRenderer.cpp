#include "SDLRenderer.h"

#include <SDL_image.h>
#include <stdexcept>
#include <iostream>

#include "SceneManager.h"
#include "SDLTexture2D.h"
#include "DataTypes.hpp"
#include "Font.h"
#include "SDLUtils.hpp"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

void dae::SDLRenderer::Init(SDL_Window* window)
{
	m_pWindow = window;

	m_RendererBackend = SDL_CreateRenderer(m_pWindow, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (!m_RendererBackend) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(m_pWindow, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::SDLRenderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_RendererBackend, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_RendererBackend);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	SceneManager::GetInstance().Render();

#ifdef NDEBUG
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(m_renderer);
#endif
}

void dae::SDLRenderer::DebugRender() const
{
	SceneManager::GetInstance().DebugRender();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(m_RendererBackend);
}

void dae::SDLRenderer::Destroy()
{
	if (!m_IsImguiDestroyed)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		m_IsImguiDestroyed = true;
	}

	if (m_RendererBackend)
	{
		SDL_DestroyRenderer(m_RendererBackend);
		m_RendererBackend = nullptr;
	}
}

dae::SDLRenderer::~SDLRenderer()
{
	Destroy();
}

std::shared_ptr<ITexture2D> dae::SDLRenderer::LoadTexture(const char* filepath)
{
	SDL_Texture* texture = IMG_LoadTexture(m_RendererBackend, filepath);
	if (!texture)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	return std::make_shared<SDLTexture2D>(texture);
}

std::shared_ptr<ITexture2D> dae::SDLRenderer::CreateFontTexture(const char* text, std::shared_ptr<dae::Font> font,
                                                                const ColorRGBA& color)
{
	const SDL_Color sdlColor{ color.r, color.g, color.b, color.a };
	SDL_Surface* surf = TTF_RenderText_Blended(font->GetFont(), text, sdlColor);
	if (!surf)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_RendererBackend, surf);
	SDL_FreeSurface(surf);

	if (!texture)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}

	return std::make_shared<SDLTexture2D>(texture);
}

void dae::SDLRenderer::RenderTexture(const ITexture2D& texture, const float x, const float y) const
{
	const auto* sdlTexture = dynamic_cast<const SDLTexture2D*>(&texture);
	if (!sdlTexture) return

	;
	const auto textureSize = texture.GetSize();

	SDL_FRect dst
	{
		x,
		y,
		static_cast<float>(textureSize.x),
		static_cast<float>(textureSize.y)
	};

	SDL_RenderCopyF(m_RendererBackend, sdlTexture->GetSDLTexture(), nullptr, &dst);
}

void dae::SDLRenderer::RenderTexture(const ITexture2D& texture, const Rectf& dst) const
{
    auto sdlTexture = dynamic_cast<const SDLTexture2D*>(&texture);
	if (!sdlTexture) return;

	const auto dstSDL = SDLConverters::RectfToSDLF(dst);
	SDL_RenderCopyF(m_RendererBackend, sdlTexture->GetSDLTexture(), nullptr, &dstSDL);
}

void dae::SDLRenderer::RenderTextureRegion(const ITexture2D& texture, const Rectf& src, float dstX, float dstY) const
{
    auto sdlTexture = dynamic_cast<const SDLTexture2D*>(&texture);
	if (!sdlTexture) return;

	const auto srcSDL = SDLConverters::RectfToSDL(src);
	const auto texSize = texture.GetSize();

	SDL_FRect dst
	{
		dstX,
		dstY,
		static_cast<float>(texSize.x),
		static_cast<float>(texSize.y)
	};

	SDL_RenderCopyF(m_RendererBackend, sdlTexture->GetSDLTexture(), &srcSDL, &dst);
}

void dae::SDLRenderer::RenderTextureRegion(const ITexture2D& texture, const Rectf& src, const Rectf& dst, const TextureFlip& flip) const
{
    auto sdlTexture = dynamic_cast<const SDLTexture2D*>(&texture);
	if (!sdlTexture) return;

	const auto& srcSDl = SDLConverters::RectfToSDL(src);  
	const auto& dstSDL = SDLConverters::RectfToSDLF(dst);  

	SDL_RenderCopyExF(m_RendererBackend, sdlTexture->GetSDLTexture(), &srcSDl, &dstSDL, 0, nullptr, SDLConverters::FlipToSDL(flip));
}

void dae::SDLRenderer::RenderLine(const glm::vec2& start, const glm::vec2& end, const ColorRGBA& color) const
{
	SDL_Color tempColor;
	SDL_GetRenderDrawColor(m_RendererBackend, &tempColor.r, &tempColor.g, &tempColor.b, &tempColor.a);

	SDL_SetRenderDrawColor(m_RendererBackend, color.r, color.g, color.b, color.a);
		SDL_RenderDrawLineF(m_RendererBackend, start.x, start.y, end.x, end.y);;
	SDL_SetRenderDrawColor(m_RendererBackend, tempColor.r, tempColor.g, tempColor.g, tempColor.a);
}

int dae::SDLRenderer::GetOpenGLDriverIndex()
{
	const auto driverCount = SDL_GetNumRenderDrivers();

	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
		{
			if (!strcmp(info.name, "opengl"))
				return i;
		}
	}

	return -1;
}