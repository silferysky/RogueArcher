#pragma once
#include "IEditable.h"

namespace Rogue
{
	class ImGuiEditorViewport : public IEditable
	{
	public:
		ImGuiEditorViewport() = default;
		~ImGuiEditorViewport() = default;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}