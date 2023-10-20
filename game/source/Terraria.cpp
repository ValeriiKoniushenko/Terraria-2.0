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

#include "Terraria.h"

#include "Chunck.h"
#include "Clock.h"
#include "Initer.h"
#include "Shader.h"
#include "TerrariaGameMode.h"
#include "TerrariaWorld.h"
#include "TextureManager.h"
#include "UpdateableCollector.h"
#include "UtilsFunctions.h"
#include "WidgetReflector.h"
#include "Window.h"
#include "World.h"
#include "WorldVariables.h"

#include <iostream>

Terraria::Terraria()
	: cameraRightIA("Camera to right", Keyboard::Key::D)
	, cameraLeftIA("Camera to left", Keyboard::Key::A)
	, cameraTopIA("Camera to top", Keyboard::Key::W)
	, cameraBottomIA("Camera to bottom", Keyboard::Key::S)
	, cameraZoomUpIA("Camera zoom up", Keyboard::Key::PgUp)
	, cameraZoomDownIA("Camera zoom down", Keyboard::Key::PgDown)
{
	cameraRightIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraRightIA.setIsRepeatable(true);
	cameraRightIA.onAction.subscribe(
		[this]()
		{
			camera.move({1, 0});
		});

	cameraLeftIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraLeftIA.setIsRepeatable(true);
	cameraLeftIA.onAction.subscribe(
		[this]()
		{
			camera.move({-1, 0});
		});

	cameraTopIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraTopIA.setIsRepeatable(true);
	cameraTopIA.onAction.subscribe([this]() { camera.move({0, -1}); });

	cameraBottomIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraBottomIA.setIsRepeatable(true);
	cameraBottomIA.onAction.subscribe([this]() { camera.move({0, 1}); });

	cameraZoomUpIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraZoomUpIA.setIsRepeatable(true);
	cameraZoomUpIA.onAction.subscribe([this]() { camera.zoom(-0.001f); });

	cameraZoomDownIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraZoomDownIA.setIsRepeatable(true);
	cameraZoomDownIA.onAction.subscribe([this]() { camera.zoom(0.001f); });

	Initer::init({.glfwVersion = {3, 3}, .windowSize = {1000, 1000}, .title = "My game"});

	shaderPack.loadShaders("text", "assets/shaders/text.vert", "assets/shaders/text.frag");
	shaderPack.loadShaders("widget", "assets/shaders/widget.vert", "assets/shaders/widget.frag");

	GetTextureManager().loadAllTextures();

	camera.setSize({1000, 1000});
	// camera.setOrigin({1000 / 2, 1000 / 2});
	GetWindow().setCamera(camera);
}

void Terraria::start()
{
	auto* gameMode = dynamic_cast<TerrariaGameMode*>(GetTerrariaWorld().gameMode.get());

	// map.generate(gameMode->generationRules.countOfChuncksByX, gameMode->generationRules.countOfChuncksByY);
	// map.prepareAllChuncks(shaderPack);

	while (!GetWindow().shouldClose())
	{
		Clock clock(true);
		GetWindow().clearColor({0.2f, 0.3f, 0.3f});
		GetWindow().clear(GL_COLOR_BUFFER_BIT);

		glm::vec2 cameraPositionAtMap = camera.toGlobalCoordinates(camera.getPosition()) / 2.f;
		cameraPositionAtMap += glm::vec2(GetWindow().getSize().width, GetWindow().getSize().height) / camera.getZoom() / 2.f;
		cameraPositionAtMap /= gameMode->generationRules.chunckSize;
		cameraPositionAtMap /= GetTextureManager()["air"].getImage()->getSize().x;
		cameraPositionAtMap.y += floor(gameMode->generationRules.countOfChuncksByY / 2.f);

		// map.drawChunckWithNeighbours(static_cast<long long>(cameraPositionAtMap.x), static_cast<long long>(cameraPositionAtMap.y), shaderPack, &camera);

		GetUpdateableCollector().updateAll();
		GetWorld().update();
		GetWindow().swapBuffers();
		GetWindow().pollEvent();
		camera.setTick(clock.stop() * gameMode->tickMultiplayer);
	}
}
