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

#include "Entity.h"

#include "TerrariaGameMode.h"
#include "TerrariaWorld.h"
#include "glm/geometric.hpp"

glm::vec2 Entity::getPosition() const
{
	return position_;
}

void Entity::setPosition(glm::vec2 position)
{
	position_ = position;
}

glm::vec2 Entity::getImpulse() const
{
	return impulse_;
}

void Entity::setImpulse(glm::vec2 impulse)
{
	impulse_ = impulse;
}

void Entity::update(float tick)
{
	static auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());
	if (!gameMode)
	{
		return;
	}

	impulse_ *= gameMode->airResistance;
	if (glm::length(impulse_) < 0.001)
	{
		impulse_ = glm::vec2(0.f);
	}
}
