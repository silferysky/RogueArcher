#pragma once
#include "IEditable.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "glew.h"
#include "glfw3.h"
namespace Rogue
{
	class ImGuiAssets : public IEditable
	{
	public:
		ImGuiAssets();
		~ImGuiAssets();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}
