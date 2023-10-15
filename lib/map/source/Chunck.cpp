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

#include "Chunck.h"

#include "ShaderPack.h"
#include "TextureManager.h"

void Chunck::draw(ShaderPack& shaderPack)
{
	for(auto& row : blocks_)
	{
		for (auto& block : row)
		{
			block.draw(shaderPack);
		}
	}
}

void Chunck::generate()
{
	std::size_t i = 0;
	std::size_t j = 0;
	blocks_.resize(chuckSize);

	for(auto& row : blocks_)
	{
		row.resize(chuckSize);
		i = 0;
		for (auto& block : row)
		{
			auto& texture = GetTextureManager()["stone"];
			block.setPosition({i++ * texture.getImage()->getSize().x, j * texture.getImage()->getSize().y});
			block.setTexture(texture);
		}
		++j;
	}
}
