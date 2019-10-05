#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace ImGuiComponent
{
	class ImGuiComponent
	{
	public:
		ImGuiComponent();
		~ImGuiComponent();
		void ImGuiComponentInit();
	};
}