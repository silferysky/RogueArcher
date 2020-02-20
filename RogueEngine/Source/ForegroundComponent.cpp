/* Start Header ************************************************************************/
/*!
\file           ForegroundComponent.cpp
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
#include "Precompiled.h"
#include "ForegroundComponent.h"

namespace Rogue
{
	std::string ForegroundComponent::Serialize()
	{
		std::ostringstream ss;
		return ss.str();
	}

	void ForegroundComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;		//s2 is used if two are needed

		while (std::getline(ss, s1, ';'))
		{
		}
	}

	void ForegroundComponent::DisplayOnInspector()
	{
	}
}