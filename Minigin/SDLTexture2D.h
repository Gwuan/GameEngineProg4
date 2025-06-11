#pragma once
#include <string>

#include "ITexture2D.h"

struct SDL_Texture;
namespace dae
{
	class SDLTexture2D final : public ITexture2D
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit SDLTexture2D(SDL_Texture* texture);
		~SDLTexture2D() override;

		glm::ivec2 GetSize() const override;

		SDLTexture2D(const SDLTexture2D &) = delete;
		SDLTexture2D(SDLTexture2D &&) = delete;
		SDLTexture2D & operator= (const SDLTexture2D &) = delete;
		SDLTexture2D & operator= (const SDLTexture2D &&) = delete;
	private:
		SDL_Texture* m_pTexture;
	};
}
