/* Start Header ************************************************************************/
/*!
\file           LogicSystem.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for LogicSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "ScriptComponent.h"
#include "EventListener.h"
#include <memory>
#include <vector>

namespace Rogue
{
	class LogicSystem :
		public System, public EventListener
	{
	public:
		//Logic System constructors/destructors
		LogicSystem();
		~LogicSystem();

		//Basic System 
		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event& ev) override;

		//For entity logic
		void AddLogicInterface(Entity entity, std::shared_ptr<ScriptComponent> logicInterface);
		void RemoveLogicInterface(Entity);
		void ClearLogicInterface();

		void GenerateScripts();
		void RemoveExcessAI();

		//void GenerateScript();
		void DeleteScripts();

		//For Trigger AI
		void TriggerNextDoor();

		//void SeekNearestWaypoint(Entity ent);

		void CreateMoveEvent(Entity ent, Vec2 vec);

	private:
		std::map<Entity, std::vector<std::shared_ptr<ScriptComponent>>> m_entityLogicMap;
	};

}