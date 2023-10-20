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

void Map::generate(long long countOfChuncksByX, long long countOfChuncksByY)
{
	if (countOfChuncksByX <= 0 || countOfChuncksByY <= 0)
	{
		throw std::runtime_error("Can't generate a map with zero chuncks by the axis X or Y.");
	}

	map.resize(countOfChuncksByY);

	for (int i = 0; i < map.size(); ++i)
	{
		map[i].resize(countOfChuncksByX);

		for (int j = 0; j < map[i].size(); ++j)
		{
			map[i][j].generate(j, i - map.size() / 2);
		}
	}
}

void Map::drawChunck(long long x, long long y, ShaderPack& shaderPack, Camera* camera)
{
	if (x < 0 || y < 0 || y >= map.size() || x >= map[y].size())
	{
		return;
	}

	map[y][x].draw(shaderPack, camera);
}

void Map::drawChunckWithNeighbours(long long int x, long long int y, long long radius, ShaderPack& shaderPack, Camera* camera)
{
	for (long long i = y - radius; i <= y + radius; ++i)
	{
		for (long long j = x - radius; j <= x + radius; ++j)
		{
			drawChunck(j, i, shaderPack, camera);
		}
	}
}