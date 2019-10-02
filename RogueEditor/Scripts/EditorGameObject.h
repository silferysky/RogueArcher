#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace ImGuiGameObject
{
	class ImGuiGameObject
	{
	public:
		ImGuiGameObject();
		~ImGuiGameObject();
		void ImGuiGameObjectInit();
	};
}