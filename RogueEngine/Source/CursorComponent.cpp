/* Start Header ************************************************************************/
/*!
\file           CursorComponent.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CursorComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "CursorComponent.h"


namespace Rogue
{
	const Vec2& CursorComponent::GetPosOffset()
	{
		return m_positionOffset;
	}

	void CursorComponent::SetPosOffset(const Vec2& offset)
	{
		m_positionOffset = offset;
	}

	std::string CursorComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_positionOffset.x << ";" << m_positionOffset.y;

		return ss.str();
	}

	void CursorComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;			//s2 is used if two are needed
		//int counter = 0;		//Needed to take in for multiple values
		//int sets = 2;			//Sets represents the number of "sets" that must be taken in simultaneously. Aka vec2 or more than 1	parameter to set

		Vec2 pos;

		if (std::getline(ss, s1, ';'))
			if(!s1.compare(""))
				pos.x = std::stof(s1);
		
		if (std::getline(ss, s1, ';'))
			if (!s1.compare(""))
				pos.y = std::stof(s1);
	}
}