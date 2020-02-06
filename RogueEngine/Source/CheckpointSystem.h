/* Start Header ************************************************************************/
/*!
\file           CheckpointSystem.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 Decembe, 2019
\brief          This file contains the function declarations for CheckpointSystem

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
#include "ComponentList.h"

namespace Rogue
{
	class Timer;

	class CheckpointSystem : public System, public EventListener
	{
	public:
		CheckpointSystem();
		~CheckpointSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		void Receive(Event* ev);
	};
}
