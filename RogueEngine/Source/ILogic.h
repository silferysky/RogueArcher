/* Start Header ************************************************************************/
/*!
\file           ILogic.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ILogic

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