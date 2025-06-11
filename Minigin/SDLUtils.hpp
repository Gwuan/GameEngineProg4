#pragma once
#include "DataTypes.hpp"
#include "SDL.h"
#include "IRendererService.h"

namespace SDLConverters
{
	inline SDL_RendererFlip FlipToSDL(const TextureFlip& flip)
	{
		switch (flip)
		{
		case TextureFlip::None:
			return SDL_FLIP_NONE;
		case TextureFlip::Horizontal:
			return SDL_FLIP_HORIZONTAL;
		case TextureFlip::Vertical:
			return SDL_FLIP_VERTICAL;

		default:
			return SDL_FLIP_NONE;
		}
	}

	inline SDL_Rect RectfToSDL(const Rectf& rect)
    {  
       SDL_Rect sdlRect;  

		sdlRect.x = static_cast<int>(rect.LeftBottom.x);  
       sdlRect.y = static_cast<int>(rect.LeftBottom.y);  
       sdlRect.w = static_cast<int>(rect.width);  
       sdlRect.h = static_cast<int>(rect.height);

       return sdlRect;  
    }
}
