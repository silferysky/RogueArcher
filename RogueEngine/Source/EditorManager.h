#pragma once
#include "Main.h"
#include "REEditor.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "IEditable.h"
#include "imgui_impl_win32.h"

namespace Rogue
{
	class EditorManager
	{
	public:
		EditorManager();
		~EditorManager() = default;
		void Init();
		void Update();
		void Shutdown();



		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		template<typename T>
		T* GetEditorWindow(std::string name)
		{
			auto Window = m_WindowsMap.find(name);
			return (T*)Window->second;
		}
		
		template<typename T>
		const T* GetEditorWindow(std::string name)
		{
			auto Window = m_WindowsMap.find(name);
			return (T*)Window->second;
		}

		template <typename T>
		void AddEditorWindow(std::string name)
		{
			m_WindowsVector.emplace_back(std::make_shared<T>());
			m_WindowsMap.emplace(name, std::make_shared<T>());
		}
	private:
		std::vector<std::shared_ptr<IEditable>> m_WindowsVector;
		std::unordered_map<std::string, std::shared_ptr<IEditable>> m_WindowsMap;
		float RETime = 0.0f;

	};

}

	
