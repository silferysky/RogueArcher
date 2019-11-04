#include <sstream>
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

	void CursorComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
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