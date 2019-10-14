#pragma once
#include "REEditor.h"
#include "IEditable.h"
namespace Rogue
{
	class ImGuiGameObject : public IEditable
	{
	public:
		ImGuiGameObject();
		~ImGuiGameObject();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}