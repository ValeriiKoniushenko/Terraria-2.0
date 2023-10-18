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
#include "SimplexNoise.h"
#include "TerrariaGameMode.h"
#include "TerrariaWorld.h"
#include "TextureManager.h"

void Chunck::draw(ShaderPack& shaderPack, Camera* camera /* = nullptr*/)
{
	for (auto& row : blocks_)
	{
		for (auto& block : row)
		{
			block.draw(shaderPack, camera);
		}
	}
}

void Chunck::generate(long long xOffset, long long yOffset)
{
	fillAir(xOffset, yOffset);
	generateVertices(xOffset, yOffset);
}

void Chunck::fillAir(long long int xOffset, long long int yOffset)
{
	auto& rules = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get())->generationRules;

	std::size_t i = 0;
	std::size_t j = 0;
	blocks_.resize(rules.chunckSize);

	for (auto& row : blocks_)
	{
		row.resize(rules.chunckSize);
		i = 0;
		for (auto& block : row)
		{
			auto& texture = GetTextureManager()["air"];
			auto size = texture.getImage()->getSize();
			const glm::vec2 chunckSizePx = {rules.chunckSize * size.x, rules.chunckSize * size.y};
			const glm::vec2 offset = {xOffset * chunckSizePx.x, yOffset * chunckSizePx.y};

			block.setPosition({offset.x + i++ * size.x, offset.y + j * size.y});
			block.setTexture(texture);
		}
		++j;
	}
}

void Chunck::generateVertices(long long int xOffset, long long int yOffset)
{
	auto& rules = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get())->generationRules;

	for (int x = 0; x < rules.chunckSize; ++x)
	{
		const float _y =
			floor(SimplexNoise::noise(static_cast<float>(x + xOffset * rules.chunckSize)
									  	/ rules.chunckSize / rules.chunckSmoothness)
				  						* static_cast<float>(rules.chunckSize));

		const int y = static_cast<int>(_y) + -yOffset * rules.chunckSize;

		if (y >= 0 && y < rules.chunckSize)
		{
			blocks_[y][x].setTexture(GetTextureManager()["grass_block_side"]);
			for (int yDirt = y + 1; yDirt < rules.chunckSize && yDirt < y + 5; ++yDirt)
			{
				blocks_[yDirt][x].setTexture(GetTextureManager()["dirt"]);
			}
			for (int yStone = y + rules.dirtHeight; yStone < rules.chunckSize; ++yStone)
			{
				blocks_[yStone][x].setTexture(GetTextureManager()["stone"]);
			}
		}

		int yDirt = 0;
		if (y < 0 && y + rules.dirtHeight > 0)
		{
			for (; yDirt < rules.chunckSize && yDirt < y + rules.dirtHeight; ++yDirt)
			{
				blocks_[yDirt][x].setTexture(GetTextureManager()["dirt"]);
			}
		}
		if (y < 0)
		{
			for (int yStone = yDirt; yStone < rules.chunckSize; ++yStone)
			{
				blocks_[yStone][x].setTexture(GetTextureManager()["stone"]);
			}
		}
	}
}
