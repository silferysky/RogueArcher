/* Start Header ************************************************************************/
/*!
\file           Editor.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for editor

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Editor.h"
#include "EditorManager.h"
#include "REEditor.h"
#include "REEngine.h"


namespace Rogue
{
	class Editor : public System, public EventListener
	{
	public:
		Editor() :
			System(SystemID::id_EDITOR)
		{}

		void Init() override;
		void Update() override;
		void Receive(Event* ev) override final;
		void Shutdown() override;
	private:
		EditorController Controller;
		std::vector<Entity>& m_currentVector = g_engine.m_coordinator.GetActiveObjects();
	};
}
