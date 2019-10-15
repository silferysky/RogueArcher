#pragma once
#include "REEditor.h"
#include "IEditable.h"

namespace Rogue
{
	class ImGuiComponent : public IEditable
	{
	public:
		ImGuiComponent();
		~ImGuiComponent();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}