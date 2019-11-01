#pragma once

namespace Rogue
{
	//Denotes what the AI can do
	enum class AIType
	{
		AI_Patrol,
		AI_Finder,
		AI_Static,
		AI_Last
	};

	//Denotes what states the AI can be in
	enum class AIState
	{
		AIState_Chase,
		AIState_Patrol,
		AIState_Idle,
		AIState_Last
	};
}