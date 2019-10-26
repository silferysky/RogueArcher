#pragma once
#include "IEditable.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "glew.h"
#include "Main.h"
namespace Rogue
{
	class ImGuiEditorFile : public IEditable
	{
	public:
		ImGuiEditorFile();
		~ImGuiEditorFile();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};


}