#include "Precompiled.h"
#include "ForceManager.h"
#include "Timer.h"
#include "Main.h"

namespace Rogue
{
	ForceInfo::ForceInfo(Entity entity, const Vec2& force, float lifetime, bool isActive) :
		m_entity{ entity }, m_age{ 0.0f }, m_lifetime{ lifetime }, m_force{ force }, m_isActive{ isActive }
	{}

	void ForceManager::Init()
	{
		// About 2MB worth of allocation
		m_forceInfos.reserve(128); // Reserve to an arbitruary good size to minimize simulation-time allocation

		m_currTime = g_engine.m_coordinator.GetCurrTime();
		m_prevTime = g_engine.m_coordinator.GetCurrTime();
	}

	void ForceManager::UpdateAges()
	{
		m_currTime = g_engine.m_coordinator.GetCurrTime();
		Timer::FloatSec delta = m_currTime - m_prevTime;
		m_prevTime = m_currTime;

		float duration = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(delta).count()) /
			Timer::s_microsecondsPerSecond;

		ForceVector::iterator i = m_forceInfos.begin();

		while(i != m_forceInfos.cend())
		{
			if (!i->m_isActive)
				continue;

			i->m_age += duration;

			if (i->m_age >= i->m_lifetime)
				i->m_isActive = false;

			++i;
		}

		// Remove-erase idiom
		// Move all safe elements to the front, returning iterator the "new end"
		auto newEnd = std::remove_if(m_forceInfos.begin(), m_forceInfos.end(), [](ForceInfo& force)
		{
			return force.m_age >= force.m_lifetime;
		});

		// Erase unwanted elements from new end to old end.
		m_forceInfos.erase(newEnd, m_forceInfos.end());
	}

	void ForceManager::RegisterForce(Entity entity, const Vec2& force, float lifetime)
	{
		m_forceInfos.emplace_back(ForceInfo{ entity, force, lifetime, true });
	}

	void ForceManager::AddForce(Entity entity, RigidbodyComponent& body) const
	{
		for (const ForceInfo& forceInfo : m_forceInfos)
		{
			if (forceInfo.m_entity == entity)
			{
				if (forceInfo.m_isActive == false)
					continue;

				body.addForce(forceInfo.m_force);
			}
		}
	}

	void ForceManager::RemoveForce(Entity entity)
	{
		// If there is more than 1 force that belongs to the same entity,
		// the oldest force will be removed (set inactive).
		
		std::vector<ForceInfo>::iterator i;

		for (i = m_forceInfos.begin(); i != m_forceInfos.cend(); ++i)
		{
			if (i->m_isActive == false)
				continue;

			if (i->m_entity == entity)
			{
				i->m_isActive = false;
				return;
			}
		}
	}
}