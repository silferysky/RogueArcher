#pragma once
#include "IEditable.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "glew.h"
#include "glfw3.h"
#include "REEngine.h"
#include "Timer.h"
namespace Rogue

{
	class ImGuiConsole : public IEditable
	{
	public:
		ImGuiConsole();
		~ImGuiConsole();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		REEngine engine;
	};
}