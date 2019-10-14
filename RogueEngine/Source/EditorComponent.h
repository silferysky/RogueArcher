#pragma once
#include "REEditor.h"
#include "IEditable.h"

namespace ImGuiComponent
{
	class ImGuiComponent : public IEditable
	{
	public:
		ImGuiComponent();
		~ImGuiComponent();
		void ImGuiComponentInit();
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}