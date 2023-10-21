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
	: cameraRightIA("Camera to right", Keyboard::Key::D)
	, cameraLeftIA("Camera to left", Keyboard::Key::A)
	, cameraTopIA("Camera to top", Keyboard::Key::W)
	, cameraBottomIA("Camera to bottom", Keyboard::Key::S)
	, cameraZoomUpIA("Camera zoom up", Keyboard::Key::PgUp)
	, cameraZoomDownIA("Camera zoom down", Keyboard::Key::PgDown)
{
}

void TerrariaGameState::initialize()
{
	cameraRightIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraRightIA.setIsRepeatable(true);
	cameraRightIA.onAction.subscribe([this]() { camera.move({4, 0}); });

	cameraLeftIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraLeftIA.setIsRepeatable(true);
	cameraLeftIA.onAction.subscribe([this]() { camera.move({-4, 0}); });

	cameraTopIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraTopIA.setIsRepeatable(true);
	cameraTopIA.onAction.subscribe([this]() { camera.move({0, -4}); });

	cameraBottomIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraBottomIA.setIsRepeatable(true);
	cameraBottomIA.onAction.subscribe([this]() { camera.move({0, 4}); });

	cameraZoomUpIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraZoomUpIA.setIsRepeatable(true);
	cameraZoomUpIA.onAction.subscribe([this]() { camera.zoom(-0.001f); });

	cameraZoomDownIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraZoomDownIA.setIsRepeatable(true);
	cameraZoomDownIA.onAction.subscribe([this]() { camera.zoom(0.001f); });

	shaderPack.loadShaders("text", "assets/shaders/text.vert", "assets/shaders/text.frag");
	shaderPack.loadShaders("widget", "assets/shaders/widget.vert", "assets/shaders/widget.frag");
	shaderPack.loadShaders("instansed-widget", "assets/shaders/instanced-widget.vert", "assets/shaders/instanced-widget.frag");

	GetTextureManager().loadAllTextures();

	camera.setSize({1000, 1000});
	GetWindow().setCamera(camera);

	gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	map.generate(gameMode->generationRules.countOfChuncksByX, gameMode->generationRules.countOfChuncksByY);
}

void TerrariaGameState::tick(float tick)
{
	map.drawChunckWithNeighbours(map.getCameraPositionAtMap(camera), map.getNeighboursCount(camera), shaderPack, &camera);

	camera.setTick(tick * gameMode->tickMultiplayer);
}
