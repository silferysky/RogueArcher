#pragma once
#include "REEditor.h"
#include "IEditable.h"

namespace Rogue

{
	class ImGuiConsole : public IEditable
	{
	public:
		ImGuiConsole();
		~ImGuiConsole();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}