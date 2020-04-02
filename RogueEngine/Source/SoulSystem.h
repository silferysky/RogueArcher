/* Start Header ************************************************************************/
/*!
\file           SoulSystem.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for SoulSystem

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "EventListener.h"
#include "BaseSystem.h"
#include "AppEvent.h"

namespace Rogue
{
	class SoulSystem :
		public System, public EventListener
	{
	public:
		// Constructor
		SoulSystem();

		// System overrides
		void Init() override;
		void TrueInit();
		void Update() override;
		void Shutdown() override;
		void Receive(Event& ev) override;
	};
}