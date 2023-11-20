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

#include "CopyableAndMoveable.h"
#include "Rect.h"
#include "Widget.h"
#include "glm/vec2.hpp"
#include "Delegate.h"
#include "Map.h"

#include <chrono>

class ShaderPack;
class Camera;

class Entity : public Utils::CopyableAndMoveable
{
public:
	[[nodiscard]] glm::vec2 getPosition() const;
	void setPosition(glm::vec2 position);

	[[nodiscard]] glm::vec2 getImpulse() const;
	void setImpulse(glm::vec2 impulse);
	void addImpulse(glm::vec2 impulse);

	void update(float tick);

	void setTexture(Texture& texture);

	void draw(ShaderPack& shaderPack, Camera* camera = nullptr);

	[[nodiscard]] float calculateDistanceToGround(const Map& map) const;
	[[nodiscard]] bool isGroundBelow(const Map& map) const;

public: // Delegates
	LambdaMulticastDelegate<void()> startFalling;
	LambdaMulticastDelegate<void()> endFalling;

protected:
	[[nodiscard]] virtual bool isInteractWithMap(glm::vec2 position) const;
	[[nodiscard]] virtual float getDistanceToNeighbourBlock(glm::vec2 position, Map::Direction direction) const;
	virtual void updateFreeFall(float tick);
	virtual void updateImpulse(float tick);

protected:
	Widget widget_;
	glm::vec2 position_{};
	glm::vec2 impulse_{};
	Utils::FRect rect_;
	bool isFalling_ = false;
	bool isFallingInit_ = false;
	std::chrono::system_clock::time_point fallWasStartedAt_ = std::chrono::system_clock::now();
};