#pragma once
#include "REEditor.h"
#include "IEditable.h"

namespace ImGuiEditorEdit
{
	class ImGuiEditorEdit : public IEditable
	{
	public:
		ImGuiEditorEdit();
		~ImGuiEditorEdit();
		void ImGuiEditorEditInit();
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}