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

#include "GladWrapper.h"
#include "Image.h"
#include "Shader.h"
#include "Texture.h"
#include "UpdateableCollector.h"
#include "WidgetReflector.h"
#include "Window.h"
#include "World.h"
#include "WorldVariables.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "Font.h"
#include "Initer.h"
#include "Rect.h"
#include "ShaderPack.h"
#include "StopMotionAnimation.h"
#include "TextBox.h"
#include "UtilsFunctions.h"
#include "TextureManager.h"
#include "Chunck.h"

#include <iostream>

Terraria::Terraria() :
	cameraRightIA("Camera to right", Keyboard::Key::D)
	, cameraLeftIA("Camera to left", Keyboard::Key::A)
	, cameraTopIA("Camera to top", Keyboard::Key::W)
	, cameraBottomIA("Camera to bottom", Keyboard::Key::S)
	, cameraZoomUpIA("Camera zoom up", Keyboard::Key::PgUp)
	, cameraZoomDownIA("Camera zoom down", Keyboard::Key::PgDown)
{
	cameraRightIA.setFrequency(KeyboardInputAction::TimeT(10));
	cameraRightIA.setIsRepeatable(true);
	cameraRightIA.onAction.subscribe([this](){
			camera.move({10,0});
		});

	cameraLeftIA.setFrequency(KeyboardInputAction::TimeT(10));
	cameraLeftIA.setIsRepeatable(true);
	cameraLeftIA.onAction.subscribe([this](){
			camera.move({-10,0});
		});

	cameraTopIA.setFrequency(KeyboardInputAction::TimeT(10));
	cameraTopIA.setIsRepeatable(true);
	cameraTopIA.onAction.subscribe([this](){
			camera.move({0,-10});
		});

	cameraBottomIA.setFrequency(KeyboardInputAction::TimeT(10));
	cameraBottomIA.setIsRepeatable(true);
	cameraBottomIA.onAction.subscribe([this](){
			camera.move({0,10});
		});

	cameraZoomUpIA.setFrequency(KeyboardInputAction::TimeT(10));
	cameraZoomUpIA.setIsRepeatable(true);
	cameraZoomUpIA.onAction.subscribe([this](){
			camera.zoom(-0.01f);
		});

	cameraZoomDownIA.setFrequency(KeyboardInputAction::TimeT(10));
	cameraZoomDownIA.setIsRepeatable(true);
	cameraZoomDownIA.onAction.subscribe([this](){
			camera.zoom(0.01f);
		});

	Initer::init({.glfwVersion = {3, 3}, .windowSize = {1366, 768}, .title = "My game"});

	shaderPack.loadShaders("text", "assets/shaders/text.vert", "assets/shaders/text.frag");
	shaderPack.loadShaders("widget", "assets/shaders/widget.vert", "assets/shaders/widget.frag");

	GetTextureManager().loadAllTextures();

	camera.setSize({1366, 768});
}
void Terraria::start()
{
	Chunck chunck1;
	chunck1.generate(0,0);
	Chunck chunck2;
	chunck2.generate(1,0);

	while (!GetWindow().shouldClose())
	{
		GetWindow().clearColor({0.2f, 0.3f, 0.3f});
		GetWindow().clear(GL_COLOR_BUFFER_BIT);

		chunck1.draw(shaderPack, &camera);
		chunck2.draw(shaderPack, &camera);

		GetUpdateableCollector().updateAll();
		GetWorld().update();
		GetWindow().swapBuffers();
		GetWindow().pollEvent();
	}
}
