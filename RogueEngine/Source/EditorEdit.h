#pragma once
#include "IEditable.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "glew.h"
#include "glfw3.h"
namespace Rogue
{
	class ImGuiEditorEdit : public IEditable
	{
	public:
		ImGuiEditorEdit();
		~ImGuiEditorEdit();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}