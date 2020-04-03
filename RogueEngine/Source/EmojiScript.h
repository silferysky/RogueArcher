/* Start Header ************************************************************************/
/*!
\file           EmojiScript.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for EmojiScript

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "ScriptComponent.h"
#include <queue>

namespace Rogue
{
	class EmojiScript
		: public ScriptComponent
	{
	public:
		EmojiScript(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		void LogicInit() override;

		void AIIdleUpdate() override;

		void OnTriggerEnter(Entity otherEnt) override;

	private:
		bool m_activated;
		bool m_finished;
		std::queue<std::string> m_emojiTextures;
		float m_delayBetweenEmojis;
		float m_timer;
	};
}