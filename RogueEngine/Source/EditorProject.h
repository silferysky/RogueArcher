#pragma once
#include "REEditor.h"
#include "IEditable.h"
namespace ImGuiProject

{
	class ImGuiProject : public IEditable
	{
	public:
		ImGuiProject();
		~ImGuiProject();
		void ImGuiProjectInit();
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}