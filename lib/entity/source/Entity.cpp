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

#include <iostream>

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
	updateFreeFall(tick);
	updateImpulse(tick);
}

bool Entity::isInteractWithMap(glm::vec2 position) const
{
	static auto* gameState = dynamic_cast<TerrariaGameState*>(GetTerrariaWorld().gameState.get());

	if (gameState)
	{
		return gameState->map_.isInteractWithBlockAt(position);
	}

	return false;
}

float Entity::getDistanceToNeighbourBlock(glm::vec2 position, Map::Direction direction) const
{
	static auto* gameState = dynamic_cast<TerrariaGameState*>(GetTerrariaWorld().gameState.get());

	if (gameState)
	{
		return gameState->map_.getDistanceToNeighbourBlock(position, direction);
	}

	return false;
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
	const float offsetY = static_cast<float>(gameMode->generationRules.countOfChuncksByY / 2 * gameMode->textureSize * gameMode->generationRules.chunckSize);
	auto pos = position_ + glm::vec2(0, -offsetY);

	widget_.setPosition((pos));

	widget_.draw(shaderPack, camera);
}

void Entity::updateFreeFall(float tick)
{
	if (static auto* gameState = dynamic_cast<TerrariaGameState*>(GetTerrariaWorld().gameState.get()); gameState)
	{
		auto time = static_cast<float>((std::chrono::system_clock::now() - fallWasStartedAt_).count()) / 10'000'000.f;
		static const float g = 9.8;
		impulse_.y += g * time * tick * 10'000.f;
	}
}

float Entity::calculateDistanceToGround(const Map& map) const
{
	if (const std::optional<Map::Point> mapPoint_ = map.getMapPointFromGlobalPoint(position_); mapPoint_.has_value())
	{
		if (static auto* gameState = dynamic_cast<TerrariaGameState*>(GetTerrariaWorld().gameState.get()); gameState)
		{
			if (static auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get()); gameMode)
			{
				auto position = position_;
				std::optional<Map::Point> mapPoint = mapPoint_;
				while (mapPoint.has_value() && gameState->map_.getBlock(mapPoint.value()).getTexture()->getName() == "air")
				{
					position.y += gameMode->textureSize;
					mapPoint = gameState->map_.getMapPointFromGlobalPoint(position);
				}
				return (position.y - position_.y - gameMode->textureSize * 2) / gameMode->textureSize;
			}
		}
	}
	return -1.f;
}

void Entity::updateImpulse(float tick)
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
	auto impulse = impulse_ * tick;

	if (glm::length(impulse_) < 0.001)
	{
		impulse_ = glm::vec2(0.f);
	}

	if (impulse.x != 0.f)
	{
		auto positionTransactionX = position_;
		positionTransactionX.x += impulse.x;

		if (!isInteractWithMap(positionTransactionX))
		{
			position_.x = positionTransactionX.x; // commit transaction
		}
		else
		{
			impulse_.x = getDistanceToNeighbourBlock(position_, impulse.x < 0 ? Map::Direction::Left : Map::Direction::Right);
			std::cout << impulse_.x << std::endl;
		}
	}

	if (impulse.y != 0.f)
	{
		auto positionTransactionY = position_;
		positionTransactionY.y += impulse.y;

		if (!isInteractWithMap(positionTransactionY))
		{
			position_.y = positionTransactionY.y;	 // commit transaction
		}
		else
		{
			impulse_.y = getDistanceToNeighbourBlock(position_, impulse.y < 0 ? Map::Direction::Up : Map::Direction::Down);
		}
	}
}

bool Entity::isGroundBelow(const Map& map) const
{
	if (static auto* gameState = dynamic_cast<TerrariaGameState*>(GetTerrariaWorld().gameState.get()); gameState)
	{
		if (static auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get()); gameMode)
		{
			const auto checkPosition = position_ + glm::vec2(0.f, widget_.getSize().height + 1);
			auto point = map.getMapPointFromGlobalPoint(checkPosition);
			if (!point.has_value())
			{
				return false;
			}

			if (auto* texture = gameState->map_.getBlock(point.value()).getTexture())
			{
				if (texture->getName() == "air")
				{
					return false;
				}
			}

			return true;
		}
	}
	return false;
}
