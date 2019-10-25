#pragma once
#include "IEditable.h"
#include "Timer.h"
#include "Main.h"

namespace Rogue
{
	class ImGuiConsole : public IEditable
	{
	private:
		float m_check = 0.0f;
	public:
		ImGuiConsole();
		~ImGuiConsole() = default;
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}