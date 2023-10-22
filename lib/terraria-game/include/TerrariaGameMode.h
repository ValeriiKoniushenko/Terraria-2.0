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

#include "BaseGameMode.h"

#include <cstdlib>
#include <unordered_map>
#include <string>

class TerrariaGameMode : public BaseGameMode
{
public:
	struct GenerationRules
	{
		int chunckSize = 16;
		int chunckSmoothness = 3;
		int dirtHeight = 10;
		int height = 512;
		int countOfChuncksByY = height / chunckSize;
		int countOfChuncksByX = 32;
		int maxSizeOfRandomBiomes = 10;
		int minSizeOfRandomBiomes = 5;
		int caveChance = 2000;
		int maxCaveSize = 2000;
		int minCaveSize = 10;

		struct Ore
		{
			int density = 10; // less value - more ore
			int minAmount = 10;
			int maxAmount = 10;
			int minSpawnHeight = 10;
			int maxSpawnHeight = 20;
			std::string textureName;
		};

		struct Biome
		{
			std::string name;
			int smoothness = 0;
		};

		std::unordered_map<std::string, Ore> ores = {
			{ "iron_ore", { 100, 4, 20, 0, 128, "iron_ore" } },
			{ "diamond_ore", { 250, 1, 8, 48, 128, "diamond_ore" } },
			{ "diamond_ore1", { 150, 4, 16, 128, 4096, "diamond_ore" } },
			{ "andesite", { 70, 10, 80, 40, 4096, "andesite" } },
			{ "gravel", { 100, 5, 30, 0, 4096, "gravel" } },
			{ "obsidian", { 150, 4, 16, 128, 4096, "obsidian" } },
		};
		std::unordered_map<std::string, Biome> biomes =
		{
			{ "green plain", { "green plain", 30 } },
			{ "green hills", { "green hills", 3 } },
			{ "green mountains", { "green mountains", 1 } },
			{ "water", { "water", 55 } },
			{ "space", { "space", 3 } },
			{ "hell", { "hell", 5 } },
		};
	};

	float tickMultiplayer = 1000.f;
	GenerationRules generationRules;

private:

};