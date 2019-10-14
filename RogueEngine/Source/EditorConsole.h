#pragma once
#include "REEditor.h"
#include "IEditable.h"

namespace ImGuiConsole

{
	class ImGuiConsole : public IEditable
	{
	public:
		ImGuiConsole();
		~ImGuiConsole();
		void ImGuiConsoleInit();
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}