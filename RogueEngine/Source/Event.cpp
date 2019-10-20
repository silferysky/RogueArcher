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
