#pragma once
#include <string>

namespace Rogue
{
	struct CollisionTag
	{
		std::string m_name;
		int m_tag;

		CollisionTag() = default;
		CollisionTag(std::string name, int tag)
			:m_name{ name }, m_tag{ tag }{}
	};
}