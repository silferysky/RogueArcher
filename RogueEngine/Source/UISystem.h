#pragma once
#include "SystemManager.h"
#include "EventListener.h"
#include "glm.hpp"

namespace Rogue
{
	class UISystem : public System, public EventListener
	{
		glm::vec3 m_oldCameraPos;
		glm::vec3 m_newCameraPos;
	public:
		UISystem();
		~UISystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		void Receive(Event* ev);
	};
}