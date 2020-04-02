/* Start Header ************************************************************************/
/*!
\file           TileMapSystem.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           4 April,2020
\brief          This file contains the functions definitions for TileMapSystem

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
#include "TileMapComponent.h"

namespace Rogue
{
	class TileMapSystem :
		public System, public EventListener
	{
	public:
		// Basic System 
		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event& ev) override;
	};
}