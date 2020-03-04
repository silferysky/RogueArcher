/* Start Header ************************************************************************/
/*!
\file           AnimationSystem.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for AnimationSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
		void UpdateTexture(AnimationComponent* animate, SpriteComponent* sprite);
		void UpdateTextureReversed(AnimationComponent* animate, SpriteComponent* sprite);
	public:
		AnimationSystem();
		~AnimationSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		//void ResetTextures();
		void InitAnimations();

		void Receive(Event& ev);
	};
}