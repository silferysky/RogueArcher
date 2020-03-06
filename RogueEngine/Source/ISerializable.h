/* Start Header ************************************************************************/
/*!
\file           ISerializable.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\brief          This file contains the function definitions for ISerializable

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include <string>

namespace Rogue
{
	class ISerializable
	{
	public:
		virtual std::string Serialize() = 0;
		virtual void Deserialize(std::string_view deserializeStr) = 0;
		virtual void DisplayOnInspector() {}
		virtual ~ISerializable() {}
	};
}