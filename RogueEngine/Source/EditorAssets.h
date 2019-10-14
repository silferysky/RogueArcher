#pragma once
#include "REEditor.h"
#include "IEditable.h"

namespace ImGuiAssets
{
	class ImGuiAssets : public IEditable
	{
	public:
		ImGuiAssets();
		~ImGuiAssets();
		void ImGuiAssetsInit();
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}
