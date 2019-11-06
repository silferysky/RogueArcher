#pragma once
#include "IEditable.h"
#include "Main.h"
#include "Timer.h"

namespace Rogue
{
	class ImGuiProfiler : public IEditable
	{
	private:
	public:
		ImGuiProfiler() = default;
		~ImGuiProfiler() = default;
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}