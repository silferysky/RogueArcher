/* Start Header ************************************************************************/
/*!
\file           LogicSystem.cpp
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
#include "Precompiled.h"

#include "Main.h"
#include "LogicSystem.h"
#include "EventDispatcher.h"
#include "GameEvent.h"
#include "KeyEvent.h"
#include "Timer.h"

//AI Types
#include "FinderAI.h"
#include "PlatformAI.h" //Also includes PatrolAI
#include "TriggerAI.h"
#include "TransitionObject.h"
#include "TeleCharge1.h"
#include "TeleCharge2.h"
#include "TeleCharge3.h"
#include "HoverOverButton.h"
#include "ProgressBar.h"
#include "AppearOnCollide.h"
#include "DisappearOnCollide.h"
#include "DeathBox.h"
#include "Checkpoint.h"
#include "SoulCollectible.h"
#include "TeleAnimation.h"
#include "AnimateOnEla.h"
#include "AnimateOnExa.h"
#include "ActivateOnEla.h"
#include "ActivateOnExa.h"
#include "ActivateOnDeathEla.h"
#include "ActivateOnDeathExa.h"
#include "LightsFlicker.h"
#include "EmojiScript.h"
#include "EmojiAndZoomScript.h" //Trigger zoom also
#include "GamePlayEnding.h"
#include "VegetationPillbug.h"
#include "VegetationBreakable.h"
#include "CoralClam.h"
#include "CoralBubbleTransporter.h"
#include "CoralElementBubble.h"
#include "CrystalElementBeetle.h"
#include "CrystalMole.h"

namespace Rogue
{
	LogicSystem::LogicSystem()
		: System(SystemID::id_LOGICSYSTEM)
	{
		m_entityLogicMap = std::map<Entity, std::vector<std::shared_ptr<ScriptComponent>>>();
	}

	LogicSystem::~LogicSystem()
	{
		for (auto& scriptVec : m_entityLogicMap)
		{
			scriptVec.second.clear();
		}
		m_entityLogicMap.clear();
	}

	void LogicSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_LOGICSYSTEM, LogicSystem::Receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<LogicComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<StatsComponent>());

		g_engine.m_coordinator.SetSystemSignature<LogicSystem>(signature);

		m_entityLogicMap.clear();
	}

	void LogicSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Logic System");

		//Check if any new AI needs to be added
		if (m_entities.size() > m_entityLogicMap.size())
		{
			GenerateScripts();
		}
		else if (m_entities.size() < m_entityLogicMap.size()) //Check if any AI needs to be deleted
		{
			RemoveExcessAI();
		}

		//Logic system specifically only wants to update AddExcessAI and RemoveExcessAI even while game is not running or game is paused
		//if (!g_engine.m_coordinator.GetGameState() || g_engine.m_coordinator.GetPauseState())
		//{
		//	g_engine.m_coordinator.EndTimeSystem("Logic System");
		//	return;
		//}

		//Pre AI Update

		//Reset all LogicComponent's active state bit
		for (std::pair<Entity, std::vector<std::shared_ptr<ScriptComponent>>> it : m_entityLogicMap)
		{
			for (std::shared_ptr<ScriptComponent> ai : it.second)
				ai->GetLogicComponent()->ResetActiveStateBit();
		}

		//Update all active state bits
		for (std::pair<Entity, std::vector<std::shared_ptr<ScriptComponent>>> it : m_entityLogicMap)
		{
			for (std::shared_ptr<ScriptComponent> ai : it.second)
				ai->AIActiveStateUpdate();
		}

		//AI Update
		for (std::pair<Entity, std::vector<std::shared_ptr<ScriptComponent>>> it : m_entityLogicMap)
		{
			for (std::shared_ptr<ScriptComponent> ai : it.second)
				ai->LogicUpdate();
		}
		g_engine.m_coordinator.EndTimeSystem("Logic System");
	}

	void LogicSystem::Receive(Event& ev)
	{
		switch (ev.GetEventCat())
		{
		case EventCategory::EventCatCollision:
		case EventCategory::EventCatTrigger:
		{
			auto& event = dynamic_cast<EntCollisionOrTrigger<BoxCollider2DComponent, BoxCollider2DComponent>&>(ev);
			Entity object = event.GetThis().m_entity;
			Entity triggered = event.GetOther().m_entity;
			for (Entity m : m_entities)
			{
				if (m == object || m == triggered)
					if (m_entityLogicMap[m].size()) // I think u can skip this step
					{
						for (auto& ai : m_entityLogicMap[m])
						{
							ai->HandleCollision(event);
							if (!m_entityLogicMap[m].size()) // And this?
								return;
						}
					}
			}
			return;
		}
		}
	}

	void LogicSystem::AddLogicInterface(Entity entity, std::shared_ptr<ScriptComponent> logicInterface)
	{
		m_entityLogicMap[entity].push_back(logicInterface);
		//m_entityLogicMap.emplace(std::pair < Entity, std::shared_ptr<BaseAI>>(entity, logicInterface));
		//m_entityLogicMap.insert({ entity, logicInterface });
	}

	void LogicSystem::RemoveLogicInterface(Entity entity)
	{
		m_entityLogicMap.erase(entity);
	}

	void LogicSystem::ClearLogicInterface()
	{
		for (auto& elements : m_entityLogicMap)
		{
			elements.second.clear();
		}

		m_entityLogicMap.clear();
	}

	void LogicSystem::GenerateScripts()
	{
		for (auto& entities : m_entities)
		{
			if (!m_entityLogicMap[entities].size())
			{
				//Logic component will exist if it is in m_entities
				auto& logicComponent = g_engine.m_coordinator.GetComponent<LogicComponent>(entities);
				auto& statsComponent = g_engine.m_coordinator.GetComponent<StatsComponent>(entities);

				switch (logicComponent.GetLogicType())
				{
				case AIType::AI_Finder:
				{
					FinderAI newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<FinderAI>(newAI));
					break;
				}
				case AIType::AI_Patrol:
				{
					PatrolAI newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<PatrolAI>(newAI));
					break;
				}
				case AIType::AI_Trigger:
				{
					TriggerAI newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<TriggerAI>(newAI));
					break;
				}
				case AIType::AI_Platform:
				{
					PlatformAI newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<PlatformAI>(newAI));
					break;
				}
				case AIType::Obj_Transition:
				{
					TransitionObject newAI(entities, logicComponent, statsComponent, statsComponent.GetTransitionLevel());
					AddLogicInterface(entities, std::make_shared<TransitionObject>(newAI));
					break;
				}
				case AIType::Obj_AppearOnCollide:
				{
					AppearOnCollide newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<AppearOnCollide>(newAI));
					break;
				}
				case AIType::Obj_DisappearOnCollide:
				{
					DisappearOnCollide newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<DisappearOnCollide>(newAI));
					break;
				}
				case AIType::UI_TeleCharge1:
				{
					TeleCharge1 newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<TeleCharge1>(newAI));
					break;
				}
				case AIType::UI_TeleCharge2:
				{
					TeleCharge2 newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<TeleCharge2>(newAI));
					break;
				}
				case AIType::UI_TeleCharge3:
				{
					TeleCharge3 newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<TeleCharge3>(newAI));
					break;
				}
				case AIType::UI_HoverOverButton:
				{
					HoverOverButton newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<HoverOverButton>(newAI));
					break;
				}
				case AIType::UI_ProgressBar:
				{
					ProgressBar newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<ProgressBar>(newAI));
					break;
				}
				case AIType::Gameplay_DeathBox:
				{
					DeathBox newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<DeathBox>(newAI));
					break;
				}
				case AIType::Gameplay_Checkpoint:
				{
					Checkpoint newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<Checkpoint>(newAI));
					break;
				}
				case AIType::Gameplay_SoulCollectible:
				{
					SoulCollectible newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<SoulCollectible>(newAI));
					break;
				}
				case AIType::Gameplay_TeleAnimation:
				{
					TeleAnimation newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<TeleAnimation>(newAI));
					break;
				}
				case AIType::Gameplay_AnimateOnExa:
				{
					AnimateOnExa newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<AnimateOnExa>(newAI));
					break;
				}
				case AIType::Gameplay_AnimateOnEla:
				{
					AnimateOnEla newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<AnimateOnEla>(newAI));
					break;
				}
				case AIType::Gameplay_ActivateOnExa:
				{
					ActivateOnExa newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<ActivateOnExa>(newAI));
					break;
				}
				case AIType::Gameplay_ActivateOnEla:
				{
					ActivateOnEla newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<ActivateOnEla>(newAI));
					break;
				}
				case AIType::Gameplay_ActivateOnDeathExa:
				{
					ActivateOnDeathExa newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<ActivateOnDeathExa>(newAI));
					break;
				}
				case AIType::Gameplay_ActivateOnDeathEla:
				{
					ActivateOnDeathEla newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<ActivateOnDeathEla>(newAI));
					break;
				}
				case AIType::Gameplay_Ending:
				{
					GamePlayEnding newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<GamePlayEnding>(newAI));
					break;
				}
				case AIType::Lights_Flicker:
				{
					LightsFlicker newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<LightsFlicker>(newAI));
					break;
				}
				case AIType::TriggerEmojiAndZoom:
				{
					EmojiAndZoomScript newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<EmojiAndZoomScript>(newAI));
					break;
				}
				case AIType::Trigger_Zoom:
				{
					TriggerZoom newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<TriggerZoom>(newAI));
					break;
				}
				case AIType::EmojiScript:
				{
					EmojiScript newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<EmojiScript>(newAI));
					break;
				}
				case AIType::Vegetation_Pillbug:
				{
					VegetationPillbug newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<VegetationPillbug>(newAI));
					break;
				}
				case AIType::Vegetation_Breakable:
				{
					VegetationBreakable newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<VegetationBreakable>(newAI));
					break;
				}
				case AIType::Coral_Clam:
				{
					CoralClam newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<CoralClam>(newAI));
					break;
				}
				case AIType::Coral_BubbleTransporter:
				{
					CoralBubbleTransporter newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<CoralBubbleTransporter>(newAI));
					break;
				}
				case AIType::Coral_ElementBubble:
				{
					CoralElementBubble newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<CoralElementBubble>(newAI));
					break;
				}
				case AIType::Crystal_ElementBeetle:
				{
					CrystalElementBeetle newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<CrystalElementBeetle>(newAI));
					break;
				}
				case AIType::Crystal_Mole:
				{
					CrystalMole newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<CrystalMole>(newAI));
					break;
				}
				case AIType::AI_Static:
				default:
				{
					ScriptComponent newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<ScriptComponent>(newAI));
					break;
				}


				}
			}
		}
	}

	void LogicSystem::RemoveExcessAI()
	{
		for (auto entityLogicIt = m_entityLogicMap.begin(); entityLogicIt != m_entityLogicMap.end(); ++entityLogicIt)
		{
			auto it = m_entities.find(entityLogicIt->first);
			if (it == m_entities.end())
			{
				entityLogicIt = m_entityLogicMap.erase(entityLogicIt);
			}

			if (!m_entityLogicMap.size())
				break;
		}
	}

	void LogicSystem::DeleteScripts()
	{
		for (auto& scripts : m_entityLogicMap)
		{
			scripts.second.clear();
		}
		m_entityLogicMap.clear();
	}

	void LogicSystem::TriggerNextDoor()
	{
		for (Entity entity : m_entities)
		{
			StatsComponent& stats = g_engine.m_coordinator.GetComponent<StatsComponent>(entity);

			//If not patrolling, set patrolling
			if (!stats.GetIsPatrolling())
			{
				(*m_entityLogicMap[entity].begin())->GetStatsComponent()->SetIsPatrolling(true);
				stats.SetIsPatrolling(true);
				if (g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(entity))
					g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(entity).SetCollisionMode(CollisionMode::e_trigger);
				break;
			}
		}
	}

	void LogicSystem::CreateMoveEvent(Entity ent, Vec2 vec)
	{
		EntMoveEvent moveEvent(ent, false, vec);
		moveEvent.SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
		EventDispatcher::instance().AddEvent(moveEvent);
	}

	void LogicSystem::Shutdown()
	{}

}