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