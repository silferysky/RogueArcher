#pragma once
#include "REEditor.h"
#include "IEditable.h"

namespace Rogue
{
	class ImGuiAssets : public IEditable
	{
	public:
		ImGuiAssets();
		~ImGuiAssets();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}
