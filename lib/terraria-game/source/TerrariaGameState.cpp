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

#include "TerrariaGameState.h"

#include "TerrariaWorld.h"
#include "TextureManager.h"
#include "Window.h"

TerrariaGameState::TerrariaGameState()
	: cameraRightIA_("Camera to right", Keyboard::Key::D)
	, cameraLeftIA_("Camera to left", Keyboard::Key::A)
	, cameraTopIA_("Camera to top", Keyboard::Key::W)
	, cameraBottomIA_("Camera to bottom", Keyboard::Key::S)
	, cameraZoomUpIA_("Camera zoom up", Keyboard::Key::PgUp)
	, cameraZoomDownIA_("Camera zoom down", Keyboard::Key::PgDown)
{
}

void TerrariaGameState::initialize()
{
	cameraRightIA_.setFrequency(KeyboardInputAction::TimeT(1));
	cameraRightIA_.setIsRepeatable(true);
	cameraRightIA_.onAction.subscribe([this]() { camera_.move({4, 0}); });

	cameraLeftIA_.setFrequency(KeyboardInputAction::TimeT(1));
	cameraLeftIA_.setIsRepeatable(true);
	cameraLeftIA_.onAction.subscribe([this]() { camera_.move({-4, 0}); });

	cameraTopIA_.setFrequency(KeyboardInputAction::TimeT(1));
	cameraTopIA_.setIsRepeatable(true);
	cameraTopIA_.onAction.subscribe([this]() { camera_.move({0, -4}); });

	cameraBottomIA_.setFrequency(KeyboardInputAction::TimeT(1));
	cameraBottomIA_.setIsRepeatable(true);
	cameraBottomIA_.onAction.subscribe([this]() { camera_.move({0, 4}); });

	cameraZoomUpIA_.setFrequency(KeyboardInputAction::TimeT(1));
	cameraZoomUpIA_.setIsRepeatable(true);
	cameraZoomUpIA_.onAction.subscribe([this]() { camera_.zoom(-0.001f); });

	cameraZoomDownIA_.setFrequency(KeyboardInputAction::TimeT(1));
	cameraZoomDownIA_.setIsRepeatable(true);
	cameraZoomDownIA_.onAction.subscribe([this]() { camera_.zoom(0.001f); });

	shaderPack_.loadShaders("text", "assets/shaders/text.vert", "assets/shaders/text.frag");
	shaderPack_.loadShaders("widget", "assets/shaders/widget.vert", "assets/shaders/widget.frag");
	shaderPack_.loadShaders("instansed-widget", "assets/shaders/instanced-widget.vert", "assets/shaders/instanced-widget.frag");

	GetTextureManager().loadAllTextures();

	camera_.setSize({1000, 1000});
	GetWindow().setCamera(camera_);

	gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	map_.generate(gameMode->generationRules.countOfChuncksByX, gameMode->generationRules.countOfChuncksByY);
}

void TerrariaGameState::tick(float tick)
{
	map_.drawChunckWithNeighbours(map_.getCameraPositionAtMap(camera_), map_.getNeighboursCount(camera_), shaderPack_, &camera_);

	camera_.setTick(tick * gameMode->tickMultiplayer);
}
