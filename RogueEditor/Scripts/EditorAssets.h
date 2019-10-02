#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace ImGuiAssets
{
	class ImGuiAssets
	{
	public:
		ImGuiAssets();
		~ImGuiAssets();
		void ImGuiAssetsInit();
	};
}
