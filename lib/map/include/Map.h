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

#pragma once

#include "NotCopyableAndNotMovable.h"
#include "Chunck.h"
#include "TerrariaGameMode.h"

#include <vector>

class ShaderPack;
class Texture;

class Map : public Utils::NotCopyableAndNotMovable
{
public:
	using MapT = std::vector<std::vector<Chunck>>;
	using Biome = TerrariaGameMode::GenerationRules::Biome;

	void generate(long long countOfChuncksByX, long long countOfChuncksByY);
	void drawChunck(long long x, long long y, ShaderPack& shaderPack, Camera* camera = nullptr);
	void drawChunckWithNeighbours(glm::ivec2 position, long long radius, ShaderPack& shaderPack, Camera* camera = nullptr);
	glm::ivec2 getCameraPositionAtMap(Camera& camera);
	int getNeighboursCount(Camera& camera);

private:
	void generateBiomes(long long countOfChuncksByX, long long countOfChuncksByY);
	void generateRandomBiomes(long long countOfChuncksByX, long long countOfChuncksByY);
	void generateCaves(long long countOfChuncksByX, long long countOfChuncksByY);
	void generateCave(long long int chunckX, long long int chunckY);
	void setTextureAt(int chunckX, int chunckY, int x, int y, int radius, Texture& texture);
	void generateBedrock(long long int countOfChuncksByX, long long int countOfChuncksByY);
	void generateAsteroids(long long int countOfChuncksByX, long long int countOfChuncksByY);
	void generateAsteroid(long long int chunckX, long long int chunckY);

private:
	MapT map_;
	std::vector<std::vector<Biome>> biomePerChunck_;
};