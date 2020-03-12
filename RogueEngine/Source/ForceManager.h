/* Start Header ************************************************************************/
/*!
\file           ForceManager.h
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

		ForceInfo(Entity entity, const Vec2& force, float lifetime = 1 / 60.0f, bool isActive = false);
	};

	class ForceManager
	{
	public:
		using ForceVector = std::vector<ForceInfo>;
		const float c_stopFactor = 10.0f;
	private:
		ForceVector m_forceInfos;
	public:
		static ForceManager& instance()
		{
			static ForceManager instance;
			return instance;
		}

		void Init();
		void ApplyAllForces(); 
		void UpdateAges();

		void RegisterForce(Entity entity, const Vec2& force, float lifetime = 1 / 60.0f);
		void RemoveForce(Entity entity);
		void ResetPhysics(Entity entity);
		void PrintForceVector();
	};

}