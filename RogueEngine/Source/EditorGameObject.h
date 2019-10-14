#pragma once
#include "REEditor.h"
#include "IEditable.h"
namespace ImGuiGameObject
{
	class ImGuiGameObject : public IEditable
	{
	public:
		ImGuiGameObject();
		~ImGuiGameObject();
		void ImGuiGameObjectInit();
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}