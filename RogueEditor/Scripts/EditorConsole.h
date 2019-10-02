#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace ImGuiConsole

{
	class ImGuiConsole
	{
	public:
		ImGuiConsole();
		~ImGuiConsole();
		void ImGuiConsoleInit();
	};
}