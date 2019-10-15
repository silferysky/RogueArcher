#pragma once
#include "REEditor.h"
#include "IEditable.h"
namespace Rogue

{
	class ImGuiProject : public IEditable
	{
	public:
		ImGuiProject();
		~ImGuiProject();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}