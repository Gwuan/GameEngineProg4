#pragma once

#include <SDL.h>
#include "DataTypes.hpp"
#include "IRendererService.h"

namespace SDLConverters
{
	inline SDL_RendererFlip FlipToSDL(const TextureFlip& flip)
	{
		switch (flip)
		{
			case TextureFlip::None:			return SDL_FLIP_NONE;
			case TextureFlip::Horizontal:	return SDL_FLIP_HORIZONTAL;
			case TextureFlip::Vertical:		return SDL_FLIP_VERTICAL;
			default:						return SDL_FLIP_NONE;
		}
	}

	inline SDL_Rect RectfToSDL(const Rectf& rect)
    {
		return SDL_Rect
		{
			static_cast<int>(rect.LeftBottom.x),
			static_cast<int>(rect.LeftBottom.y),  
			static_cast<int>(rect.width),  
			static_cast<int>(rect.height)
		};
    }

	inline SDL_FRect RectfToSDLF(const Rectf& rect)
    {  
       return SDL_FRect
		{
			rect.LeftBottom.x,
			rect.LeftBottom.y,
			rect.width,
			rect.height
		};  
    }
}
