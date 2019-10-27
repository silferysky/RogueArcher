#pragma once
#include "IEditable.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "glew.h"
#include "EditorHierarchy.h"
#include "EditorManager.h"
namespace Rogue
{
	class ImGuiInspector : public IEditable
	{
	public:
		ImGuiInspector();
		~ImGuiInspector();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
};
