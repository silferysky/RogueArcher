#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace ImGuiEditorEdit
{
	class ImGuiEditorEdit
	{
	public:
		ImGuiEditorEdit();
		~ImGuiEditorEdit();
		void ImGuiEditorEditInit();
	};
}