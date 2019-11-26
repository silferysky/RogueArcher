#pragma once
#include "IEditable.h"
#include "Types.h"

namespace Rogue
{
	class ImGuiEditorViewport : public IEditable
	{
	private:
	ImGuizmo::OPERATION m_CurrentGizmoOperation;
	ImGuizmo::MODE m_CurrentGizmoMode;
	std::vector<Entity>& m_currentVector;
	public:
		ImGuiEditorViewport();
		~ImGuiEditorViewport() = default;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		void ShowGizmo(Entity &selectedentity);
	};
}