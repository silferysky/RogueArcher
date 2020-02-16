#pragma once

namespace Rogue
{
	template <typename TInteger>
	struct IntegerPairComparer
	{
		// If both entities of one pair are the same the entities of the other, regardless of order, they are considered the same.
		bool operator()(const std::pair<TInteger, TInteger>& pair1, const std::pair<TInteger, TInteger>& pair2) const
		{
			return ( pair1 == pair2 || (pair1.first == pair2.second && pair1.second == pair2.first) );
		}
	};
	
	template <typename TInteger>
	struct IntegerPairHasher
	{
		// Hash two integers using Cantor pairing
		size_t operator()(const std::pair<TInteger, TInteger>& pair) const
		{
			return std::hash<int>()( (pair.first + pair.second) * (pair.first + pair.second + 1) / 2 + pair.second );
		}
	};
}