/* Start Header ************************************************************************/
/*!
\file           MaskingSystem.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 Decembe, 2019
\brief          This file contains the function declarations for MaskingSystem

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
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
	class MaskingComponent;
	
	class MaskingSystem : public System, public EventListener
	{
		void UpdateTexture(MaskingComponent* masking, SpriteComponent* sprite);
	public:
		MaskingSystem();
		~MaskingSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		void ResetTextures();

		void Receive(Event& ev);
	};
}