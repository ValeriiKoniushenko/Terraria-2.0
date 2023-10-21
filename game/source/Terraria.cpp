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

#include "Clock.h"
#include "Initer.h"
#include "TerrariaGameState.h"
#include "TerrariaWorld.h"
#include "TextureManager.h"
#include "UpdateableCollector.h"
#include "UtilsFunctions.h"
#include "Window.h"
#include "World.h"

Terraria::Terraria()
{
	Initer::init({.glfwVersion = {3, 3}, .windowSize = {1000, 1000}, .title = "My game"});
}

void Terraria::start()
{
	auto* gameState = dynamic_cast<TerrariaGameState*>(GetTerrariaWorld().gameState.get());
	gameState->initialize();

	Clock clock;
	while (!GetWindow().shouldClose())
	{
		clock.start();
		GetWindow().clearColor({0.2f, 0.3f, 0.3f});
		GetWindow().clear(GL_COLOR_BUFFER_BIT);

		gameState->tick(clock.getGap());

		GetUpdateableCollector().updateAll();
		GetWorld().update();
		GetWindow().swapBuffers();
		GetWindow().pollEvent();
		clock.stop();
	}

	GetTerrariaWorld().clear();
}
