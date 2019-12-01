/*Start Header*************************************************************************/
/*!
\file           Event.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Event

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "Event.h"

namespace Rogue
{
	void Event::SetSystemReceivers(std::bitset<(int)SystemID::id_LASTSYS> bitset)
	{
		m_systemsToReceive = bitset;
	}

	void Event::SetSystemReceivers(int bit, bool state)
	{
		m_systemsToReceive.set(bit, state);
	}

	void Event::SetSystemReceivers()
	{
		m_systemsToReceive.set();
	}

	std::bitset<(int)SystemID::id_LASTSYS> Event::GetSystemReceivers() const
	{
		return m_systemsToReceive;
	}
}