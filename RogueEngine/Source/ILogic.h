#pragma once

namespace Rogue
{
	class ILogic
	{
	public:
		//Virtual void logic update
		virtual void logicInit() = 0;
		//Do not override this, it is already done in BaseAI. Anything just modify BaseAI
		virtual void logicUpdate() = 0;
		virtual ~ILogic() = default;
	};
}