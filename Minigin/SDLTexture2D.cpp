#include <SDL.h>
#include <SDL_image.h>
#include "SDLTexture2D.h"
#include "SDLRenderer.h"
#include <stdexcept>

dae::SDLTexture2D::~SDLTexture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

glm::ivec2 dae::SDLTexture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* dae::SDLTexture2D::GetSDLTexture() const
{
	return m_pTexture;
}

dae::SDLTexture2D::SDLTexture2D(SDL_Texture* texture)	: m_pTexture{ texture } 
{
	assert(m_pTexture != nullptr);
}

