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

#pragma once

#include "BaseGameState.h"
#include "Camera.h"
#include "InputAction.h"
#include "Map.h"
#include "ShaderPack.h"
#include "InstancedWidget.h"
#include "StopMotionAnimation.h"

class TerrariaGameMode;

class TerrariaGameState : public BaseGameState
{
public:
	TerrariaGameState();

	void initialize();
	void tick(float tick);

private:
	TerrariaGameMode* gameMode = nullptr;

	InstancedWidget widget;
	InstancedWidget widget1;
	StopMotionAnimation<InstancedWidget> anim;
	StopMotionAnimation<InstancedWidget> anim1;

	Map map_;
	Camera camera_;
	KeyboardInputAction cameraRightIA_;
	KeyboardInputAction cameraLeftIA_;
	KeyboardInputAction cameraTopIA_;
	KeyboardInputAction cameraBottomIA_;
	KeyboardInputAction cameraZoomUpIA_;
	KeyboardInputAction cameraZoomDownIA_;
	ShaderPack shaderPack_;
};