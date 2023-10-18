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

#include "Block.h"

#include "Texture.h"
#include "ShaderPack.h"
#include "Camera.h"

Block::Block(Block&& other) noexcept
{
	*this = std::move(other);
}

Block& Block::operator=(Block&& other) noexcept
{
	health_ = other.health_;
	widget_ = std::move(other.widget_);

	other.health_ = {};

	return *this;
}

void Block::setPosition(glm::vec2 position)
{
	widget_.setPosition(position);
}

[[nodiscard]] glm::vec2 Block::getPosition() const
{
	return widget_.getPosition();
}

void Block::setTexture(Texture& texture)
{
	widget_.setTexture(texture);
	widget_.calculateFitTextureSize();
}

[[nodiscard]] const Texture& Block::getTexture() const
{
	return widget_.getTexture();
}

[[nodiscard]] Texture& Block::getTexture()
{
	return widget_.getTexture();
}

void Block::setHealth(float health)
{
	health_ = health;
}

[[nodiscard]] float Block::getHealth() const
{
	return health_;
}

void Block::draw(ShaderPack& shaderPack, Camera* camera/* = nullptr*/)
{
	widget_.draw(shaderPack, camera);
}
