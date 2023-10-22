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
#include "TerrariaWorld.h"
#include "TextureManager.h"
#include "InstancedWidget.h"

void Chunck::draw(ShaderPack& shaderPack, Camera* camera/* = nullptr*/)
{
	for (auto& block : widgets_)
	{
		block.second.draw(shaderPack, camera);
	}
}

void Chunck::generate(long long int xOffset, long long int yOffset)
{
	fillAir(xOffset, yOffset);
	generateMainMap(xOffset, yOffset);
	generateOres(xOffset, yOffset);
}

void Chunck::fillAir(long long int xOffset, long long int yOffset)
{
	auto& rules = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get())->generationRules;

	blocks_.resize(rules.chunckSize);
	for (auto& row : blocks_)
	{
		row.resize(rules.chunckSize);
		for (auto& block : row)
		{
			block.setTexture(getTexture("air"));
		}
	}
}

void Chunck::generateMainMap(long long int xOffset, long long int yOffset)
{
	auto& rules = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get())->generationRules;
	for (int x = 0; x < rules.chunckSize; ++x)
	{
		const float _y = floor(
			SimplexNoise::noise(static_cast<float>(x + xOffset * rules.chunckSize) / rules.chunckSize / rules.chunckSmoothness) *
			static_cast<float>(rules.chunckSize));

		const int y = static_cast<int>(_y) + -yOffset * rules.chunckSize;

		if (y >= 0 && y < rules.chunckSize)
		{
			blocks_[y][x].setTexture(getTexture("grass_block_side"));
			for (int yDirt = y + 1; yDirt < rules.chunckSize && yDirt < y + rules.dirtHeight; ++yDirt)
			{
				blocks_[yDirt][x].setTexture(getTexture("dirt"));
			}
			for (int yStone = y + rules.dirtHeight; yStone < rules.chunckSize; ++yStone)
			{
				blocks_[yStone][x].setTexture(getTexture("stone"));
			}
		}

		int yDirt = 0;
		if (y < 0 && y + rules.dirtHeight > 0)
		{
			for (; yDirt < rules.chunckSize && yDirt < y + rules.dirtHeight; ++yDirt)
			{
				blocks_[yDirt][x].setTexture(getTexture("dirt"));
			}
		}
		if (y < 0)
		{
			for (int yStone = yDirt; yStone < rules.chunckSize; ++yStone)
			{
				blocks_[yStone][x].setTexture(getTexture("stone"));
				if (rand() % rules.caveChance == 0)
				{
					caveCanBeGenerated = true;
				}
			}
		}
	}
}

void Chunck::calculateInstanceData(long long int xOffset, long long int yOffset)
{
	auto& rules = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get())->generationRules;

	for (std::size_t i = 0; i < blocks_.size(); ++i)
	{
		for (std::size_t j = 0; j < blocks_[i].size(); ++j)
		{
			auto* texture = blocks_[i][j].getTexture();

			glm::vec2 position = texture->getImage()->getSize() * glm::ivec2(j, i);
			const glm::vec2 chunckSizePx = {rules.chunckSize * texture->getImage()->getSize().x, rules.chunckSize * texture->getImage()->getSize().y};
			position += glm::vec2{xOffset * chunckSizePx.x, yOffset * chunckSizePx.y};

			widgets_[texture->getName()].getTransforms().emplace_back(position);
		}
	}
}

Texture& Chunck::getTexture(const std::string& name)
{
	widgets_.emplace(name, GetTextureManager().getInstancedWidget(name));
	return GetTextureManager().getTexture(name);
}


void Chunck::generateOres(long long int xOffset, long long int yOffset)
{
	auto& rules = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get())->generationRules;

	for (int y = 0; y < rules.chunckSize; ++y)
	{
		for (int x = 0; x < rules.chunckSize; ++x)
		{
			auto& block = blocks_[y][x];

			long long realY = yOffset * rules.chunckSize + y;

			if (block.getTexture()->getName() == "stone")
			{
				for (auto& ore : rules.ores)
				{
					walkGenerator(x, y, realY, ore.second);
				}
			}
		}
	}
}

void Chunck::walkGenerator(long long int x, long long int y, long long realY, const TerrariaGameMode::GenerationRules::Ore& ore)
{
	auto& rules = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get())->generationRules;

	if (rand() % ore.density == 0 && ore.minSpawnHeight <= realY && ore.maxSpawnHeight >= realY)
	{
		int maxAmount = (rand() % (ore.maxAmount - ore.minAmount)) + ore.minAmount;
		for (int amount = 0; amount < maxAmount; ++amount)
		{
			if (x < rules.chunckSize && x >= 0 && y < rules.chunckSize && y >= 0)
			{
				blocks_[y][x].setTexture(getTexture(ore.textureName));
			}

			switch(rand() % 4)
			{
				case 0: ++x; break;
				case 1: --x; break;
				case 2: ++y; break;
				case 3: --y; break;
			}
		}
	}
}

const std::vector<Block>& Chunck::operator[](int y) const
{
	return blocks_.at(y);
}

void Chunck::setBlockAt(int x, int y, const std::string& texture)
{
	blocks_[y][x].setTexture(getTexture(texture));
}
