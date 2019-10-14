#pragma once
#include "REEditor.h"
#include "IEditable.h"

namespace ImGuiEditorFile
{
	class ImGuiEditorFile : public IEditable
	{
	public:
		ImGuiEditorFile();
		~ImGuiEditorFile();
		void ImGuiEditorFileMenuInit();
		virtual void Update() override;
		virtual void Shutdown() override;
	};


}