#pragma once
#include <cstdio>
#include <cstdlib>

#include <fstream>
#include "Main.h"
#include <vector>
#include <unordered_map>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "REEditor.h"
namespace EditorManager
{
	class IEditable;
	class EditorManager
	{
	public:
		EditorManager();
		~EditorManager() = default;
		void StartWindow();
		void UpdateWindow();
		void CloseWindow();

		bool show_demo_window = true;
		bool show_another_window = false;
		bool show = true;
		bool Inspector = true;
		bool Dropbox = true;
		bool Console = true;
		GLFWwindow* window = nullptr;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		
		template<typename T>
		void AddEditorWindow(T* Window)
		{

		}
	private:
		std::vector<IEditable*> m_WindowsVector;
		std::unordered_map<std::string, IEditable*> m_WindowsMap;
		float RETime = 0.0f;

	};
}
	
