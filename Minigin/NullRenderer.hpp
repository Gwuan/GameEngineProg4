#pragma once
#include "IRendererService.h"

class NullRenderer : public IRendererService
{
public:
	void Init(SDL_Window*) override {}
	void Render() const override {}
	void DebugRender() const override {}
	void Destroy() override {}
	std::shared_ptr<ITexture2D> LoadTexture(const char*) override { return nullptr; }
	std::shared_ptr<ITexture2D> CreateFontTexture(const char* , std::shared_ptr<dae::Font> ,
		const ColorRGBA& ) override {return nullptr;}
	void RenderTexture(const ITexture2D& , float , float ) const override {}
	void RenderTexture(const ITexture2D& , const Rectf& ) const override {}
	void RenderTextureRegion(const ITexture2D& , const Rectf& , float , float ) const override {}
	void RenderTextureRegion(const ITexture2D& , const Rectf& , const Rectf& ,
		const TextureFlip& ) const override {}
	void RenderLine(const glm::vec2& , const glm::vec2& , const ColorRGBA& ) const override {}
};