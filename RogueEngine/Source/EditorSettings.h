#pragma once
#include "IEditable.h"
#include "glew.h"

namespace Rogue
{
	class ImGuiEditorSettings:public IEditable
	{
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		bool show_demo_window = false;
		bool show_another_window = false;
		bool show = false;
	};
}