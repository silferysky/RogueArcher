/* Start Header ************************************************************************/
/*!
\file           IntegerPairHasher.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions defintions for IntegerPairHasher

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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