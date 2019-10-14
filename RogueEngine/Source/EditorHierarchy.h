#pragma once
#include "REEditor.h"
#include "IEditable.h"
namespace ImGuiEditorHierarchy
{
	class ImGuiEditorHierarchy : public IEditable
	{
	public:
		ImGuiEditorHierarchy();
		~ImGuiEditorHierarchy();
		void ImGuiEditorHierarchyInit();
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}