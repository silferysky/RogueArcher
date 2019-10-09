#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace ImGuiEditorHierarchy
{
	class ImGuiEditorHierarchy
	{
	public:
		ImGuiEditorHierarchy();
		~ImGuiEditorHierarchy();
		void ImGuiEditorHierarchyInit();
	};
}