#pragma once
#include "SystemManager.h"
#include "EventListener.h"
#include "GL/glew.h"
#include "GLHelper.hpp"
#include "ComponentList.h"

namespace Rogue
{
	class Timer;
	class SpriteComponent;

	class AnimationSystem : public System, public EventListener
	{
		void UpdateTexture(AnimationComponent* animComp);
	public:
		AnimationSystem();
		~AnimationSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		void Receive(Event* ev);
	};
}