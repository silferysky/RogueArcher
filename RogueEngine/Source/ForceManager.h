#pragma once
#include <vector>
#include "Vector2D.h"
#include "types.h"
#include "Timer.h"

namespace Rogue
{
	class RigidbodyComponent;

	struct ForceInfo
	{
		Entity m_entity;

		float m_age;
		float m_lifetime;

		Vec2 m_force;
		bool m_isActive;

		ForceInfo(Entity entity, const Vec2& force, float lifetime, bool isActive = false);
	};

	class ForceManager
	{
		std::vector<ForceInfo> m_forceInfos;
		Timer::ChronoTime m_prevTime;
		Timer::ChronoTime m_currTime;
	public:
		static ForceManager& instance()
		{
			static ForceManager instance;
			return instance;
		}

		void Init();
		void UpdateAges();

		void RegisterForce(Entity entity, const Vec2& force, float lifetime);
		void AddForce(Entity entity, RigidbodyComponent& body) const;
		void RemoveForce(Entity entity);
	};

}