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
		HierarchyInfo(Entity entity, std::string_view name,std::string_view tag = "")
			: m_Entity{ entity }, m_objectName{ name }, m_tag{ tag }{}
	};

	class DirectoryInfo
	{
	public:
		std::string m_objectName;
		bool m_selected = false;
		int m_level = 0;
		DirectoryInfo() = default;
		DirectoryInfo(std::string_view name,int level) : m_objectName(name),m_level(level) {}
	};
}