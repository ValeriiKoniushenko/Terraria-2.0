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
#include "TerrariaGameState.h"
#include "InstancedWidget.h"
#include "ShaderPack.h"
#include "Camera.h"

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

	if (impulse_.x == 0.f && impulse_.y == 0.f)
	{
		return;
	}

	impulse_ *= gameMode->airResistance;
	if (glm::length(impulse_) < 0.001)
	{
		impulse_ = glm::vec2(0.f);
	}

	auto positionTransactionX = position_;
	positionTransactionX.x += impulse_.x * tick;

	if (!isInteractWithMap(positionTransactionX))
	{
		position_.x = positionTransactionX.x; // commit transaction
	}

	auto positionTransactionY = position_;
	positionTransactionY.y += impulse_.y * tick;

	if (!isInteractWithMap(positionTransactionY))
	{
		position_.y = positionTransactionY.y; // commit transaction
	}
}

bool Entity::isInteractWithMap(glm::vec2 position) const
{
	static auto* gameState = dynamic_cast<TerrariaGameState*>(GetTerrariaWorld().gameState.get());

	return gameState->map_.isInteractWithBlockAt(position);
}

void Entity::addImpulse(glm::vec2 impulse)
{
	setImpulse(impulse + impulse_);
}

void Entity::setTexture(Texture& texture)
{
	widget_.setTexture(texture);
}

void Entity::draw(ShaderPack& shaderPack, Camera* camera/* = nullptr*/)
{
	static auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());
	static auto* gameState = dynamic_cast<TerrariaGameState*>(GetTerrariaWorld().gameState.get());

	widget_.setSize({static_cast<float>(gameMode->textureSize),static_cast<float>(gameMode->textureSize)});
	float offsetY = gameMode->generationRules.countOfChuncksByY / 2 * gameMode->textureSize * gameMode->generationRules.chunckSize;
	auto pos = position_ + glm::vec2(0, -offsetY);

	widget_.setPosition((pos));

	widget_.draw(shaderPack, camera);
}
