#pragma once

class ILogic
{
public:
	virtual void logicUpdate() = 0;
	virtual ~ILogic() = default;
};