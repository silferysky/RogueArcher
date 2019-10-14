#pragma once

#include "REEditor.h"
#include "IEditable.h"
namespace Rogue
{
	class ImGuiInspector : public IEditable
	{
	public:
		ImGuiInspector();
		~ImGuiInspector();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
};
