/* Start Header ************************************************************************/
/*!
\file           ParticleSystem.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ParticleSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "EventListener.h"
#include "BaseSystem.h"

namespace Rogue
{
	class ParticleSystem
		: public System, public EventListener
	{
	public:
		ParticleSystem();
		~ParticleSystem() = default;

		//Basic System 
		void Init() override;
		void Update() override;
		void Receive(Event& ev) override;
		void Shutdown() override;
	};
}