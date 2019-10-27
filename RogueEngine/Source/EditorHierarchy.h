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
		bool m_selected = false;
		Entity m_Entity = 0;
		float x = 0.0f;
		float y = 0.0f;
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
		std::vector <HierarchyInfo>& m_getActiveObjects();
	};
#define SETSTRING(str, i1, cmp) str << "Entity" << i1 << cmp
#define SETSSTOSTR(ss) stdstr = ss.str(); cstr = stdstr.c_str()
#define CLEARSTR(s) s.clear(); s.str("")
#define CLEARNSETSTR(s, i1, cmp) CLEARSTR(s); SETSTRING(s, i1, cmp); SETSSTOSTR(s)
}