// MIT License
//
// Copyright (c) 2023 Valerii Koniushenko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "TextureManager.h"

#include "TerrariaGameMode.h"
#include "TerrariaWorld.h"

#include <filesystem>

TextureManager& GetTextureManager()
{
	return TextureManager::instance();
}

void TextureManager::loadAllTextures()
{
	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

	static auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	for (const auto& dirEntry : recursive_directory_iterator("assets/textures/"))
	{
		if (!std::filesystem::is_directory(dirEntry))
		{
			Image image;
			image.loadImage(dirEntry.path().string());
			image.setInternalChannel(Gl::Texture::Channel::RGBA);
			images_.emplace(dirEntry.path().stem().string(), std::move(image));

			Texture texture(Gl::Texture::Target::Texture2D, true, true);
			texture.setImage(images_[dirEntry.path().stem().string()]);
			texture.setMagAndMinFilter(Gl::Texture::MagFilter::Linear, Gl::Texture::MinFilter::LinearMipmapLinear);
			textures_.emplace(dirEntry.path().stem().string(), std::move(texture));

			InstancedWidget widget;
			widget.setTexture(textures_[dirEntry.path().stem().string()]);
			widget.setSize({static_cast<float>(gameMode->textureSize), static_cast<float>(gameMode->textureSize)});
			widgets_.emplace(dirEntry.path().stem().string(), std::move(widget));
		}
	}
}

Texture& TextureManager::getTexture(const std::string& blockName)
{
	return textures_[blockName];
}

InstancedWidget& TextureManager::getInstancedWidget(const std::string& blockName)
{
	return widgets_[blockName];
}
