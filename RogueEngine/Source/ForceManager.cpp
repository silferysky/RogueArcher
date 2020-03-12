/* Start Header ************************************************************************/
/*!
\file           ForceManager.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ForceManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "ForceManager.h"
#include "Timer.h"
#include "Main.h"

namespace Rogue
{
	ForceInfo::ForceInfo(Entity entity, const Vec2& force, float lifetime, bool isActive) :
		m_entity{ entity }, m_age{ lifetime }, m_lifetime{ lifetime }, m_force{ force }, m_isActive{ isActive }
	{}

	void ForceManager::Init()
	{
		//m_forceInfos.reserve(16); // Reserve to an arbitruary good size to minimize simulation-time allocation
	}

	void ForceManager::UpdateAges()
	{
		std::for_each(std::begin(m_forceInfos), std::end(m_forceInfos), [](ForceInfo& forceInfo)
			{
				if (!forceInfo.m_isActive)
					return;

				forceInfo.m_age -= g_fixedDeltaTime;

				if (forceInfo.m_age < 0.0f)
					forceInfo.m_isActive = false;
			});

		// Remove-erase idiom
		// Move all safe elements to the front, returning iterator the "new end"
		auto newEnd = std::remove_if(m_forceInfos.begin(), m_forceInfos.end(), [](ForceInfo& force)
		{
			return force.m_isActive == false;
		});
		
		// Erase unwanted elements from new end to old end.
		m_forceInfos.erase(newEnd, m_forceInfos.end());


	}

	void ForceManager::RegisterForce(Entity entity, const Vec2& force, float lifetime)
	{
		//if (REAbs(force.x) < RE_EPSILON && REAbs(force.y) < RE_EPSILON)
		//	return;

		m_forceInfos.emplace_back(ForceInfo{ entity, force, lifetime, true });
		//PrintForceVector();
	}

	void ForceManager::ApplyAllForces()
	{
		for (const ForceInfo& forceInfo : m_forceInfos)
		{
			if (forceInfo.m_isActive == false)
				continue;

			if(auto bodyOption = g_engine.m_coordinator.TryGetComponent<RigidbodyComponent>(forceInfo.m_entity))
				bodyOption->get().addForce(forceInfo.m_force);
		}
	}

	// Remove all forces tied to entity
	void ForceManager::RemoveForce(Entity entity)
	{
		std::vector<ForceInfo>::iterator i;

		for (i = m_forceInfos.begin(); i != m_forceInfos.cend(); ++i)
		{
			if (i->m_isActive == false)
				continue;

			if (i->m_entity == entity)
			{
				i->m_isActive = false;
			}
		}
	}

	void ForceManager::ResetPhysics(Entity entity)
	{
		// Reset force, acceleration, velocity
		RemoveForce(entity);

		auto& body = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
		body.setAcceleration(Vec2());
		body.setVelocity(Vec2());
	}

	void ForceManager::PrintForceVector()
	{
		std::cout << "Size: " << m_forceInfos.size() << std::endl;

		for (ForceInfo& force : m_forceInfos)
		{
			std::cout
				<< "Entity: " << force.m_entity
				<< " Force: " << force.m_force
				<< " Age: " << force.m_age
				<< " Lifetime: " << force.m_lifetime
				<< " Is active: " << std::boolalpha << force.m_isActive
				<< "\n========================================="
				<< std::endl;
		}
	}
}