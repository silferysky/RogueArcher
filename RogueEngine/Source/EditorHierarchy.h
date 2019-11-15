#pragma once
#include "IEditable.h"
#include "REEngine.h"
#include "Main.h"
#include "EditorHierarchyInfo.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"

namespace Rogue
{
	class ImGuiEditorHierarchy : public IEditable
	{
	private:
		std::vector<HierarchyInfo>& m_currentVector;
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