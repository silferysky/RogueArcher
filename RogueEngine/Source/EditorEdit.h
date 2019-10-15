#pragma once
#include "REEditor.h"
#include "IEditable.h"

namespace Rogue
{
	class ImGuiEditorEdit : public IEditable
	{
	public:
		ImGuiEditorEdit();
		~ImGuiEditorEdit();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}