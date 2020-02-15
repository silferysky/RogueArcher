/* Start Header ************************************************************************/
/*!
\file           EditorViewport.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorViewport

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
		static ImGuiEditorViewport& instance()
		{
			static ImGuiEditorViewport instance;
			return instance;
		}
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		void ShowGizmo(Entity &selectedentity);
	};
}