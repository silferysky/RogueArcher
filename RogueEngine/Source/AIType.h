/* Start Header ************************************************************************/
/*!
\file           AIType.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for AIType

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

namespace Rogue
{
	//Denotes what the AI can do
	enum class AIType
	{
		AI_Static,
		AI_Patrol,
		AI_Finder,
		AI_Trigger,
		AI_Platform,
		Obj_Transition,
		Obj_AppearOnCollide,
		Obj_DisappearOnCollide,
		UI_TeleCharge1,
		UI_TeleCharge2,
		UI_TeleCharge3,
		Gameplay_DeathBox,
		Gameplay_Checkpoint,
		Gameplay_SoulCollectible,
		Gameplay_TeleAnimation,
		Gameplay_AnimateOnExa,
		Gameplay_AnimateOnEla,
		Gameplay_ActivateOnExa,
		Gameplay_ActivateOnEla,
		Gameplay_ActivateOnDeathExa,
		Gameplay_ActivateOnDeathEla,
		Lights_Flicker,
		UI_HoverOverButton,
		UI_ProgressBar,
		Gameplay_Ending,
		Obj_Fade,
		Trigger_Zoom,
		EmojiScript,
		TriggerEmojiAndZoom,
		Vegetation_Pillbug,
		Vegetation_Breakable,
		Coral_Clam,
		Coral_BubbleTransporter,
		Coral_ElementBubble,
		Crystal_ElementBeetle,
		Crystal_Mole,

		AI_Last
	};

	//Denotes what states the AI can be in
	enum class AIState
	{
		AIState_Idle,
		AIState_Chase,
		AIState_Patrol,
		AIState_Last
	};
}