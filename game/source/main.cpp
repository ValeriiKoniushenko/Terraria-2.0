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

#include "Logger.h"
#include "Terraria.h"

#include <stdexcept>

int main()
{
#if __DEBUG
	Terraria program;
	program.start();
#else
	try
	{
		Terraria program;
		program.start();
	}
	catch (std::runtime_error& error)
	{
		spdlog::get("core")->critical("STD Error: {}", error.what());
		MessageBoxA(nullptr, error.what(), "STD Error", MB_OK);
		return 1;
	}
	catch (std::exception& error)
	{
		spdlog::get("core")->critical("Unknown STD Error");
		MessageBoxA(nullptr, error.what(), "Unknown STD Error", MB_OK);
		return 2;
	}
	catch (...)
	{
		spdlog::get("core")->critical("Unknown Error");
		MessageBoxA(nullptr, "An unknown error. To know details go to log.", "Unknown Error", MB_OK);
		return 3;
	}
#endif

	return 0;
}