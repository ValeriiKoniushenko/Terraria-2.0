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

#include "Shader.h"
#include "UpdateableCollector.h"
#include "Window.h"
#include "World.h"
#include "Initer.h"
#include "WidgetReflector.h"
#include "UtilsFunctions.h"
#include "TextureManager.h"
#include "Chunck.h"
#include "Clock.h"
#include "WorldVariables.h"

#include <iostream>

Terraria::Terraria() :
	cameraRightIA("Camera to right", Keyboard::Key::D)
	, cameraLeftIA("Camera to left", Keyboard::Key::A)
	, cameraTopIA("Camera to top", Keyboard::Key::W)
	, cameraBottomIA("Camera to bottom", Keyboard::Key::S)
	, cameraZoomUpIA("Camera zoom up", Keyboard::Key::PgUp)
	, cameraZoomDownIA("Camera zoom down", Keyboard::Key::PgDown)
{
	cameraRightIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraRightIA.setIsRepeatable(true);
	cameraRightIA.onAction.subscribe([this](){
			camera.move({1,0});
		});

	cameraLeftIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraLeftIA.setIsRepeatable(true);
	cameraLeftIA.onAction.subscribe([this](){
			camera.move({-1,0});
		});

	cameraTopIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraTopIA.setIsRepeatable(true);
	cameraTopIA.onAction.subscribe([this](){
			camera.move({0,-1});
		});

	cameraBottomIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraBottomIA.setIsRepeatable(true);
	cameraBottomIA.onAction.subscribe([this](){
			camera.move({0,1});
		});

	cameraZoomUpIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraZoomUpIA.setIsRepeatable(true);
	cameraZoomUpIA.onAction.subscribe([this](){
			camera.zoom(-0.001f);
		});

	cameraZoomDownIA.setFrequency(KeyboardInputAction::TimeT(1));
	cameraZoomDownIA.setIsRepeatable(true);
	cameraZoomDownIA.onAction.subscribe([this](){
			camera.zoom(0.001f);
		});

	Initer::init({.glfwVersion = {3, 3}, .windowSize = {3440, 1440}, .title = "My game"});

	shaderPack.loadShaders("text", "assets/shaders/text.vert", "assets/shaders/text.frag");
	shaderPack.loadShaders("widget", "assets/shaders/widget.vert", "assets/shaders/widget.frag");

	GetTextureManager().loadAllTextures();

	camera.setSize({3440, 1440});
	camera.setOrigin({3440 / 2, 1440 / 2});
	GetWindow().setCamera(camera);
}

void Terraria::start()
{
	constexpr std::size_t size = 3;

	std::vector<std::vector<Chunck>> chuncks;
	chuncks.resize(size);
	for (auto& y : chuncks)
	{
		y.resize(size);
	}

	for (int i = 0; i < chuncks.size(); ++i)
	{
		for (int j = 0; j < chuncks[i].size(); ++j)
		{
			chuncks[i][j].generate(j, i - chuncks[i].size() / 2);
		}
	}

	constexpr float tickMultiplayer = 1000.f; // TODO: move to XXX-State
	while (!GetWindow().shouldClose())
	{
		Clock clock(true);
		GetWindow().clearColor({0.2f, 0.3f, 0.3f});
		GetWindow().clear(GL_COLOR_BUFFER_BIT);

		for (auto& y : chuncks)
		{
			for (auto& x : y)
			{
				x.draw(shaderPack, &camera);
			}
		}

		GetUpdateableCollector().updateAll();
		GetWorld().update();
		GetWindow().swapBuffers();
		GetWindow().pollEvent();
		camera.setTick(clock.stop() * tickMultiplayer);
	}
}
