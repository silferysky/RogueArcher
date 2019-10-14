#pragma once

class ILogic
{
public:
	//Virtual void logic update
	virtual void logicInit() = 0;
	//Do not override this, it is already done in BaseAI. Anything just modify BaseAI
	virtual void logicUpdate() = 0;
	virtual ~ILogic() = default;

	//All of the AI types to update
	virtual void AIChaseUpdate() {}
	virtual void AIPatrolUpdate() {}
	virtual void AILookUpdate() {}
	virtual void AIIdleUpdate() {}
};