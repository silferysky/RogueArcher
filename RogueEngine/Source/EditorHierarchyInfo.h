#pragma once
#include <string>
#include "Types.h"

namespace Rogue
{
	class HierarchyInfo
	{
	public:
		std::string m_objectName;
		std::string m_tag;
		bool m_selected = false;
		Entity m_Entity = 0;

		HierarchyInfo() = default;
		HierarchyInfo(Entity entity, std::string name)
			: m_Entity{ entity }, m_objectName{ name }{}
	};
}