#pragma once
#include <string>
#include "Types.h"

namespace Rogue
{
	class HierarchyInfo
	{
	public:
		std::string m_objectName;
		bool m_selected = false;
		Entity m_Entity = 0;
	};
}