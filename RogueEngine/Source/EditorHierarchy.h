#pragma once
#include "IEditable.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "glew.h"
#include <vector>
#include "REEngine.h"
#include "Main.h"

namespace Rogue
{
	struct HierarchyInfo
	{
		std::string m_objectName;
		bool m_selected = true;
		Entity m_Entity = 0;
	};

	class ImGuiEditorHierarchy : public IEditable
	{
	private:
		std::vector <HierarchyInfo> m_currentActiveObjects;
		unsigned int iterator = 0;
	public:
		ImGuiEditorHierarchy();
		~ImGuiEditorHierarchy();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}