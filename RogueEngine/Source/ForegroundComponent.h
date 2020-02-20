/* Start Header ************************************************************************/
/*!
\file           ForegroundComponent.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           21 February 2020
\brief          This file contains the function definitions for ForegroundComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class ForegroundComponent
		: public BaseComponent
	{
	public:
		ForegroundComponent() = default;
		~ForegroundComponent() = default;

		//Serialize
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void DisplayOnInspector();
	};
}
