#pragma once
#include <cstdio>
#include <cstdlib>

#include <fstream>
#include "Main.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
namespace ImGuiLayer
{
	class ImguiLayer
	{
	public:
		ImguiLayer();
		~ImguiLayer() = default;
		void StartWindow();
		void UpdateWindow();
		void CloseWindow();

		void Begin();
		void End();

		bool show_demo_window = true;
		bool show_another_window = false;
		bool show = true;
		bool Inspector = true;
		bool Dropbox = true;
		bool Console = true;
		GLFWwindow* window = nullptr;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	private:

		float RETime = 0.0f;

	};
}
	
