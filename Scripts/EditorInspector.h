#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace ImGuiInspector
{
	class ImGuiInspector
	{
	public:
		ImGuiInspector();
		~ImGuiInspector();
		void InitInspector();
	};
};
