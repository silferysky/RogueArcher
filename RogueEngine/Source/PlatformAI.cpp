#include "Precompiled.h"
#include "PlatformAI.h"
#include "EventDispatcher.h"
#include "GameEvent.h"

namespace Rogue
{
	PlatformAI::PlatformAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: PatrolAI(entity, logicComponent, statsComponent),
		r{ 1.0f }, g{ 1.0f }, b{ 1.0f }, a{ 1.0f },
		transitionSpeed{ 0.5f },
		transiting{ false },
		changeDelay{ m_patrolDelay / 1.5f }
	{
		//HierarchyInfo& info = g_engine.m_coordinator.GetHierarchyInfo(entity);
		//for (auto child : info.m_children)
		//{
		//	if (g_engine.m_coordinator.ComponentExists<ChildComponent>(child))
		//	{
		//		g_engine.m_coordinator.GetComponent<ChildComponent>(child).SetIsFollowing(false);
		//	}
		//}
	}

	void PlatformAI::AIPatrolUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (PLAYER_STATUS.GetPlayerEntity() == MAX_ENTITIES)
			return;

		PatrolAI::AIPatrolUpdate();

		//Since new waypoint just selected, it means now it is going opposite way
		Vec2 firstPos = *m_waypoints.begin();
		Vec2 secondPos = *(m_waypoints.begin() + 1);

		HierarchyInfo& info = g_engine.m_coordinator.GetHierarchyInfo(m_entity);

		//oss << "\nIsTransiting " << transiting;
		//Transitioning colours
		if (transiting)
		{
			if (g_engine.m_coordinator.ComponentExists<SpriteComponent>(m_entity))
			{
				SpriteComponent& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity);
				float localR = sprite.getFilter().r, localG = sprite.getFilter().g, localB = sprite.getFilter().b, localA = sprite.getFilter().a;

				if (sprite.getFilter().r - r > 0.001f || sprite.getFilter().r - r < -0.001f)
				{
					if (sprite.getFilter().r > r)
						localR -= transitionSpeed * g_deltaTime * g_engine.GetTimeScale();
					else
						localR += transitionSpeed * g_deltaTime * g_engine.GetTimeScale();
				}
				if (sprite.getFilter().g - g > 0.001f || sprite.getFilter().g - g < -0.001f)
				{
					if (sprite.getFilter().g > g)
						localG -= transitionSpeed * g_deltaTime * g_engine.GetTimeScale();
					else
						localG += transitionSpeed * g_deltaTime * g_engine.GetTimeScale();
				}
				if (sprite.getFilter().b - b > 0.001f || sprite.getFilter().b - b < -0.001f)
				{
					if (sprite.getFilter().b > b)
						localB -= transitionSpeed * g_deltaTime * g_engine.GetTimeScale();
					else
						localB += transitionSpeed * g_deltaTime * g_engine.GetTimeScale();
				}
				if (sprite.getFilter().a - a > 0.001f || sprite.getFilter().a - a < -0.001f)
				{
					if (sprite.getFilter().a > a)
						localA -= transitionSpeed * g_deltaTime * g_engine.GetTimeScale();
					else
						localA += transitionSpeed * g_deltaTime * g_engine.GetTimeScale();
				}
				sprite.setFilter(glm::vec4(localR, localG, localB, localA));
				//oss << "\nR: " << localR << "\nG: " << localG << "\nB: " << localB << "\nA: " << localA;

				Entity toChangeSprite = MAX_ENTITIES;
				for (auto child : info.m_children)
				{
					g_engine.m_coordinator.GetComponent<SpriteComponent>(child).setFilter(glm::vec4(localR, localG, localB, localA));
				}
			}
		}


		Entity toChangeSprite = MAX_ENTITIES;
		for (auto child : info.m_children)
		{
			if (g_engine.m_coordinator.GetHierarchyInfo(child).m_tag == "Change")
			{
				toChangeSprite = child;
				break;
			}
		}

		//std::ostringstream oss;
		//if (g_engine.m_coordinator.ComponentExists<AnimationComponent>(toChangeSprite) && toChangeSprite != MAX_ENTITIES)
		//{
		//	oss << "ANIM: " << g_engine.m_coordinator.GetComponent<AnimationComponent>(toChangeSprite).getCurrentFrame();
		//}
		//RE_INFO(oss.str());

		//If m_delay == m_patrolDelay, it means a new waypoint is just selected
		if (m_delay == m_patrolDelay)
		{

			if (!info.m_children.size())
				return;

			Entity toChangeSprite = MAX_ENTITIES;

			for (auto child : info.m_children)
			{
				if (g_engine.m_coordinator.GetHierarchyInfo(child).m_tag == "Change")
				{
					toChangeSprite = child;
					break;
				}
			}

			if (toChangeSprite == MAX_ENTITIES)
				return;

			//Changing buds
			//EntChangeSpriteEvent& ev;
			//EntChangeRGBAEvent& rgbaEv;
			if ((m_currentPointIndex && firstPos.y > secondPos.y) || (!m_currentPointIndex && firstPos.y < secondPos.y))
			{
				//ev = new EntChangeRGBAEvent(m_entity, 1.0f, 0.0f, 1.0f, a);
				r = 0.71f;
				g = 0.62f;
				b = 1.0f;
				//a = 1.0f;
				transiting = true;

				//ev = new EntChangeSpriteEvent(toChangeSprite, "Resources\\Assets\\FlowerBud.png");
				/* if (!g_engine.m_coordinator.ComponentExists<AnimationComponent>(toChangeSprite))
				{
					AnimationComponent animation{};
					animation.Deserialize("4;0;0.3f;0;0;");
					g_engine.m_coordinator.AddComponent(toChangeSprite, animation);
				} */
				g_engine.m_coordinator.GetComponent<AnimationComponent>(toChangeSprite).setIsAnimating(true);
				g_engine.m_coordinator.GetComponent<AnimationComponent>(toChangeSprite).setIsNotReversed(true);
				g_engine.m_coordinator.GetComponent<AnimationComponent>(toChangeSprite).setCurrentFrame(0);
			}
			else
			{
				//ev = new EntChangeRGBAEvent(m_entity, 1.0f, 1.0f, 0.0f, a);
				r = 1.0f;
				g = 0.99f;
				b = 0.62f;
				//a = 1.0f;
				transiting = true;
				//ev = new EntChangeSpriteEvent(toChangeSprite, "Resources\\Assets\\FlowerBud.png");
				/* if (!g_engine.m_coordinator.ComponentExists<AnimationComponent>(toChangeSprite))
				{
					AnimationComponent animation{};
					animation.Deserialize("4;0;0.3f;0;0;");
					g_engine.m_coordinator.AddComponent(toChangeSprite, animation);
				} */
				g_engine.m_coordinator.GetComponent<AnimationComponent>(toChangeSprite).setIsAnimating(true);
				g_engine.m_coordinator.GetComponent<AnimationComponent>(toChangeSprite).setIsNotReversed(false);
				g_engine.m_coordinator.GetComponent<AnimationComponent>(toChangeSprite).setCurrentFrame(4);
			}
			//ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			//rgbaEv.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			//EventDispatcher::instance().AddEvent(ev);
			//EventDispatcher::instance().AddEvent(rgbaEv);

			//Manually preventing second occurance of this event
			m_delay -= 0.001f;
		}
		else if (m_delay - changeDelay > -0.01f && m_delay - changeDelay < 0.01f)
		{
			if ((m_currentPointIndex && firstPos.y > secondPos.y) || (!m_currentPointIndex && firstPos.y < secondPos.y))
			{

				if (g_engine.m_coordinator.ComponentExists<ColliderComponent>(m_entity))
				{
					ColliderComponent& collider = g_engine.m_coordinator.GetComponent<ColliderComponent>(m_entity);
					int lightPos = -1;
					int darkPos = -1;

					lightPos = LayerManager::instance().GetLayerCategory("Light");
					darkPos = LayerManager::instance().GetLayerCategory("Dark");

					collider.ChangeLayer(darkPos);
					collider.SetMask(lightPos);
					collider.SetMask(darkPos, false);
				}
			}
			else
			{

				if (g_engine.m_coordinator.ComponentExists<ColliderComponent>(m_entity))
				{
					ColliderComponent& collider = g_engine.m_coordinator.GetComponent<ColliderComponent>(m_entity);
					int lightPos = -1;
					int darkPos = -1;

					lightPos = LayerManager::instance().GetLayerCategory("Light");
					darkPos = LayerManager::instance().GetLayerCategory("Dark");

					collider.ChangeLayer(lightPos);
					collider.SetMask(darkPos);
					collider.SetMask(lightPos, false);
				}
			}
			//ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			//EventDispatcher::instance().AddEvent(ev);

			//Manually preventing second occurance of this event
			m_delay -= 0.001f;
		}
	}
}