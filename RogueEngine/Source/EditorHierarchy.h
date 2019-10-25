#pragma once
#include "IEditable.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "glew.h"
#include <vector>
#include "REEngine.h"

namespace Rogue
{
	struct HierarchyInfo
	{
		std::string m_objectName;
		bool m_selected = false;

	};
	class ImGuiEditorHierarchy : public IEditable
	{
	private:
		std::vector <std::string> m_currentActiveObjects;
		int count = 0;
	public:
		ImGuiEditorHierarchy();
		~ImGuiEditorHierarchy();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}