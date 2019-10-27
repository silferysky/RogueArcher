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
	class ImGuiEditorHierarchy : public IEditable
	{
	private:
		unsigned int iterator = 0;
		std::vector<HierarchyInfo>& vector = g_engine.m_coordinator.GetActiveObjects();
	public:
		ImGuiEditorHierarchy();
		~ImGuiEditorHierarchy();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		
	};
#define SETSTRING(str, i1, cmp) str << "Entity" << i1 << cmp
#define SETSSTOSTR(ss) stdstr = ss.str(); cstr = stdstr.c_str()
#define CLEARSTR(s) s.clear(); s.str("")
#define CLEARNSETSTR(s, i1, cmp) CLEARSTR(s); SETSTRING(s, i1, cmp); SETSSTOSTR(s)
}