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

#include "Map.h"

#include "Camera.h"
#include "TerrariaWorld.h"
#include "TextureManager.h"
#include "Window.h"

void Map::generate(long long countOfChuncksByX, long long countOfChuncksByY)
{
	if (countOfChuncksByX <= 0 || countOfChuncksByY <= 0)
	{
		throw std::runtime_error("Can't generate a map with zero chuncks by the axis X or Y.");
	}

	generateBiomes(countOfChuncksByX, countOfChuncksByY);

	map_.resize(countOfChuncksByY);

	for (int i = 0; i < map_.size(); ++i)
	{
		map_[i].resize(countOfChuncksByX);

		for (int j = 0; j < map_[i].size(); ++j)
		{
			map_[i][j].generate(j, i - map_.size() / 2);
		}
	}

	generateHell(countOfChuncksByX, countOfChuncksByY);
	generateAsteroids(countOfChuncksByX, countOfChuncksByY);
	generateCaves(countOfChuncksByX, countOfChuncksByY);
	generateBedrock(countOfChuncksByX, countOfChuncksByY);

	for (int i = 0; i < map_.size(); ++i)
	{
		for (int j = 0; j < map_[i].size(); ++j)
		{
			map_[i][j].calculateInstanceData(j, i - map_.size() / 2);
		}
	}
}

void Map::drawChunck(long long x, long long y, ShaderPack& shaderPack, Camera* camera)
{
	if (x < 0 || y < 0 || y >= map_.size() || x >= map_[y].size())
	{
		return;
	}

	map_[y][x].draw(shaderPack, camera);
}

void Map::drawChunckWithNeighbours(glm::ivec2 position, long long radius, ShaderPack& shaderPack, Camera* camera)
{
	for (long long i = position.y - radius; i <= position.y + radius; ++i)
	{
		for (long long j = position.x - radius; j <= position.x + radius; ++j)
		{
			drawChunck(j, i, shaderPack, camera);
		}
	}
}

glm::ivec2 Map::getCameraPositionAtMap(Camera& camera)
{
	auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	glm::vec2 cameraPositionAtMap = camera.toGlobalCoordinates(camera.getPosition()) / 2.f;
	cameraPositionAtMap += glm::vec2(GetWindow().getSize().width, GetWindow().getSize().height) / camera.getZoom() / 2.f;
	cameraPositionAtMap /= gameMode->generationRules.chunckSize;
	cameraPositionAtMap /= GetTextureManager().getTexture("air").getImage()->getSize().x;
	cameraPositionAtMap.y += floor(gameMode->generationRules.countOfChuncksByY / 2.f);

	return cameraPositionAtMap;
}

int Map::getNeighboursCount(Camera& camera)
{
	auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	int neighboursCount =
		GetWindow().getSize().width / camera.getZoom() / GetTextureManager().getTexture("air").getImage()->getSize().x;
	neighboursCount /= gameMode->generationRules.chunckSize;
	if (neighboursCount < 1)
	{
		neighboursCount = 1;
	}

	return neighboursCount;
}

void Map::generateBiomes(long long countOfChuncksByX, long long countOfChuncksByY)
{
	auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	std::size_t mapPartSize = gameMode->generationRules.countOfChuncksByY / 3;	  // count of regions(space, earth, hell)

	biomePerChunck_.resize(countOfChuncksByY);
	for (std::size_t i = 0; i < biomePerChunck_.size(); ++i)
	{
		biomePerChunck_[i].resize(countOfChuncksByX);
		for (std::size_t j = 0; j < biomePerChunck_[i].size(); ++j)
		{
			Biome biome;

			if (i < mapPartSize)
			{
				biome = gameMode->generationRules.biomes["space"];
			}
			else if (i < mapPartSize * 2)
			{
				biome = gameMode->generationRules.biomes["green plain"];
			}
			else
			{
				biome = gameMode->generationRules.biomes["hell"];
			}

			biomePerChunck_[i][j] = biome;
		}
	}
}

void Map::generateRandomBiomes(long long int countOfChuncksByX, long long int countOfChuncksByY)
{
	auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	int biomePoint = rand() % countOfChuncksByX;
	std::size_t mapPartSize = gameMode->generationRules.countOfChuncksByY / 3;	  // count of regions(space, earth, hell)

	/*for(std::size_t i = 0; i < biomePerChunck_.size(); ++i)
	{

		for (std::size_t j = 0; j < biomePerChunck_[i].size(); ++j)
		{
			Biome biome;

			if (i < mapPartSize)
				biome = gameMode->generationRules.biomes["space"];
			else if (i < mapPartSize * 2)
				biome = gameMode->generationRules.biomes["green plain"];
			else
				biome = gameMode->generationRules.biomes["hell"];

			biomePerChunck_[i][j] = biome;
		}
	}*/
}

void Map::generateCaves(long long int countOfChuncksByX, long long int countOfChuncksByY)
{
	for (std::size_t i = 0; i < map_.size(); ++i)
	{
		for (std::size_t j = 0; j < map_[i].size(); ++j)
		{
			if (map_[i][j].caveCanBeGenerated)
			{
				generateCave(j, i);
			}
		}
	}
}

void Map::generateCave(long long int chunckX, long long int chunckY)
{
	int x = 0, y = 0;

	auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	int size = rand() % (gameMode->generationRules.maxCaveSize - gameMode->generationRules.minCaveSize) +
			   gameMode->generationRules.minCaveSize;
	while (size >= 0)
	{
		setTextureAt(chunckX, chunckY, x, y, 1, GetTextureManager().getTexture("air"));

		int nextPosition = rand() % 4;
		switch(nextPosition)
		{
			case 0: ++x; break;
			case 1: --x; break;
			case 2: ++y; break;
			case 3: --y; break;
		}

		if (x >= gameMode->generationRules.chunckSize)
		{
			++chunckX;
			x = 0;
		}
		if (x < 0)
		{
			--chunckX;
			x = gameMode->generationRules.chunckSize - 1;
		}
		if (y >= gameMode->generationRules.chunckSize)
		{
			++chunckY;
			y = 0;
		}
		if (y < 0)
		{
			--chunckY;
			y = gameMode->generationRules.chunckSize - 1;
		}

		if (chunckX < 0)
		{
			chunckX = 0;
		}
		if (chunckX >= gameMode->generationRules.countOfChuncksByX)
		{
			chunckX = gameMode->generationRules.countOfChuncksByX - 1;
		}
		if (chunckY < 0)
		{
			chunckY = 0;
		}
		if (chunckY >= gameMode->generationRules.countOfChuncksByY)
		{
			chunckY = gameMode->generationRules.countOfChuncksByY - 1;
		}

		--size;
	}
}

void Map::setTextureAt(int chunckX, int chunckY, int x, int y, int radius, Texture& texture)
{
	auto& generationRules = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get())->generationRules;
	auto chunckSize = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get())->generationRules.chunckSize;

	for (int i = y - radius; i <= y + radius; ++i)
	{
		for (int j = x - radius; j <= x + radius; ++j)
		{
			int fakeI = i;
			int fakeJ = j;
			if (i < 0)
			{
				fakeI += chunckSize;
				--chunckY;
			}
			if (i >= chunckSize)
			{
				fakeI = 0;
				++chunckY;
			}
			if (j < 0)
			{
				fakeJ += chunckSize;
				--chunckX;
			}
			if (j >= chunckSize)
			{
				fakeJ = 0;
				++chunckX;
			}

			if (chunckX < 0)
				chunckX = 0;
			if (chunckX >= generationRules.countOfChuncksByX)
				chunckX = generationRules.countOfChuncksByX - 1;
			if (chunckY < 0)
				chunckY = 0;
			if (chunckY >= generationRules.countOfChuncksByY)
				chunckY = generationRules.countOfChuncksByY - 1;

			map_[chunckY][chunckX].setBlockAt(fakeJ, fakeI, texture.getName());

			if (i < 0)
			{
				++chunckY;
			}
			if (i >= chunckSize)
			{
				--chunckY;
			}
			if (j < 0)
			{
				++chunckX;
			}
			if (j >= chunckSize)
			{
				--chunckX;
			}
		}
	}
}

void Map::generateBedrock(long long int countOfChuncksByX, long long int countOfChuncksByY)
{
	auto& generationRules = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get())->generationRules;

	for (std::size_t i = 0; i < countOfChuncksByX; ++i)
	{
		for (std::size_t j = 0; j < generationRules.chunckSize; ++j)
		{
			map_[countOfChuncksByY - 1][i].setBlockAt(j, generationRules.chunckSize - 1, "bedrock");
		}
	}
}

void Map::generateAsteroids(long long int countOfChuncksByX, long long int countOfChuncksByY)
{
	auto& generationRules = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get())->generationRules;

	for (std::size_t i = 0; i < countOfChuncksByY / 3; ++i)
	{
		for (std::size_t j = 0; j < countOfChuncksByX; ++j)
		{
			if (rand() % generationRules.asteroidChance == 0)
			{
				generateAsteroid(j, i);
			}
		}
	}
}

void Map::generateAsteroid(long long int chunckX, long long int chunckY)
{
	int x = 0, y = 0;

	auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	int size = rand() % (gameMode->generationRules.maxAsteroidSize - gameMode->generationRules.minAsteroidSize) +
			   gameMode->generationRules.minAsteroidSize;
	while (size >= 0)
	{
		setTextureAt(chunckX, chunckY, x, y, 1, GetTextureManager().getTexture("stone"));

		int nextPosition = rand() % 6;
		switch(nextPosition)
		{
			case 0: ++x; break;
			case 1: ++x; break;
			case 2: --x; break;
			case 3: --x; break;
			case 4: ++y; break;
			case 5: --y; break;
		}

		if (x >= gameMode->generationRules.chunckSize)
		{
			++chunckX;
			x = 0;
		}
		if (x < 0)
		{
			--chunckX;
			x = gameMode->generationRules.chunckSize - 1;
		}
		if (y >= gameMode->generationRules.chunckSize)
		{
			++chunckY;
			y = 0;
		}
		if (y < 0)
		{
			--chunckY;
			y = gameMode->generationRules.chunckSize - 1;
		}

		if (chunckX < 0)
		{
			chunckX = 0;
		}
		if (chunckX >= gameMode->generationRules.countOfChuncksByX)
		{
			chunckX = gameMode->generationRules.countOfChuncksByX - 1;
		}
		if (chunckY < 0)
		{
			chunckY = 0;
		}
		if (chunckY >= gameMode->generationRules.countOfChuncksByY)
		{
			chunckY = gameMode->generationRules.countOfChuncksByY - 1;
		}

		--size;
	}
}

void Map::generateHell(long long int countOfChuncksByX, long long int countOfChuncksByY)
{
	auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	for (int i = countOfChuncksByY - countOfChuncksByY / 3; i < countOfChuncksByY; ++i)
	{
		for (int j = 0; j < countOfChuncksByX; ++j)
		{

			for (int x = 0; x < gameMode->generationRules.chunckSize; ++x)
			{
				if (i == countOfChuncksByY - countOfChuncksByY / 3)
				{
					int limit = rand() % gameMode->generationRules.hellGradient;
					for (int y = 0; y < limit; ++y)
					{
						map_[i][j].setBlockAt(x, gameMode->generationRules.chunckSize - y - 1, "netherrack", true);
					}
				}
				else
				{
					for (int y = 0; y < gameMode->generationRules.chunckSize; ++y)
					{
						map_[i][j].setBlockAt(x, y, "netherrack", true);
					}
				}
			}
		}
	}
}

bool Map::isInteractWithBlockAt(glm::vec2 position)
{
	if (isInteractWithPointAt(position))
		return true;

	if (isInteractWithPointAt(position + glm::vec2(512, 0)))
		return true;

	if (isInteractWithPointAt(position + glm::vec2(0, 512)))
		return true;

	if (isInteractWithPointAt(position + glm::vec2(512, 512)))
		return true;

	return false;
}

bool Map::isInteractWithPointAt(glm::vec2 position)
{
	static auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	const glm::ivec2 globalBlockPosition = position / glm::vec2(gameMode->textureSize, gameMode->textureSize);
	const glm::ivec2 chunckPosition = globalBlockPosition / glm::ivec2(gameMode->generationRules.chunckSize);
	const glm::ivec2 localBlockPosition = globalBlockPosition % glm::ivec2(gameMode->generationRules.chunckSize);

	if (chunckPosition.x < 0 || chunckPosition.y < 0 || chunckPosition.x >= gameMode->generationRules.countOfChuncksByX ||
		chunckPosition.y >= gameMode->generationRules.countOfChuncksByY || localBlockPosition.x < 0 || localBlockPosition.y < 0)
	{
		return true;
	}

	auto& block = map_[chunckPosition.y][chunckPosition.x][localBlockPosition.y][localBlockPosition.x];
	if (block.getTexture()->getName() == "air")
	{
		return false;
	}

	return true;
}
