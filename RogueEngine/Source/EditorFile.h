#pragma once
#include "REEditor.h"
#include "IEditable.h"

namespace Rogue
{
	class ImGuiEditorFile : public IEditable
	{
	public:
		ImGuiEditorFile();
		~ImGuiEditorFile();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};


}