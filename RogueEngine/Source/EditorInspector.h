#pragma once

#include "REEditor.h"
#include "IEditable.h"
namespace ImGuiInspector
{
	class ImGuiInspector : public IEditable
	{
	public:
		ImGuiInspector();
		~ImGuiInspector();
		void InitInspector();
		virtual void Update() override;
		virtual void Shutdown() override;
	};
};
