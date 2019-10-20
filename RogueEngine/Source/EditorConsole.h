#pragma once
#include "IEditable.h"
#include "Timer.h"
#include "REEngine.h"

namespace Rogue
{
	class ImGuiConsole : public IEditable
	{
	public:
		ImGuiConsole();
		~ImGuiConsole() = default;
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}