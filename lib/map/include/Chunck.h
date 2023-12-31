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

#include "NotCopyableButMovable.h"
#include "Block.h"
#include "TerrariaGameMode.h"
#include "InstancedWidget.h"
#include "StopMotionAnimation.h"

#include <vector>
#include <unordered_map>
#include <memory>

class ShaderPack;

class Chunck : public Utils::NotCopyableButMovable
{
public:
	using Container = std::vector<std::vector<Block>>;

	void draw(ShaderPack& shaderPack, Camera* camera = nullptr);
	void generate(long long xOffset, long long yOffset);
	void calculateInstanceData(long long int xOffset, long long int yOffset);
	void setBlockAt(int x, int y, const std::string& texture, bool isAnimation = false);
	const std::vector<Block>& operator[](int y) const;
	std::vector<Block>& operator[](int y);

public:
	bool caveCanBeGenerated = false;

private:
	void fillAir(long long xOffset, long long yOffset);
	void generateMainMap(long long xOffset, long long yOffset);
	void generateOres(long long xOffset, long long yOffset);
	void walkGenerator(long long x, long long y, long long realY, const TerrariaGameMode::GenerationRules::Ore& ore);

	Texture& getTexture(const std::string& name, bool isAnimation = false);

private:
	Container blocks_;
	std::unordered_map<std::string, InstancedWidget> widgets_;
	std::unordered_map<std::string, StopMotionAnimation<InstancedWidget>> animations_;
};