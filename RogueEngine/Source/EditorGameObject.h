/* Start Header ************************************************************************/
/*!
\file           EditorGameObject.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorGameObject

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "IEditable.h"
#include "REEngine.h"

namespace Rogue
{
	class ImGuiGameObject : public IEditable
	{
	std::vector<Entity>& m_currentVector = g_engine.m_coordinator.GetActiveObjects();
	public:
		ImGuiGameObject();
		~ImGuiGameObject();
		static ImGuiGameObject& instance()
		{
			static ImGuiGameObject instance;
			return instance;
		}
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
	};
}