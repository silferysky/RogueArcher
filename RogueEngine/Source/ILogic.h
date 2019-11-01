#pragma once

namespace Rogue
{
	class ILogic
	{
	public:
		//Virtual void logic update
		virtual void LogicInit() = 0;
		//Do not override this, it is already done in BaseAI. Anything just modify BaseAI
		virtual void LogicUpdate() = 0;
		virtual ~ILogic() = default;
	};
}