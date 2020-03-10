/* Start Header ************************************************************************/
/*!
\file           EditorHierarchyInfo.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorHierarchyInfo

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
		std::string m_archetypeName;
		bool m_selected;
		Entity m_Entity;

		Entity m_parent;
		std::vector<Entity> m_children;

		HierarchyInfo(Entity entity = MAX_ENTITIES,
			std::string_view name = "",
			std::string_view tag = "",
			std::string_view archetypeName = "",
			Entity parentEnt = MAX_ENTITIES) :
			
			m_Entity{ entity }, 
			m_objectName{ name }, 
			m_tag{ tag }, 
			m_archetypeName{archetypeName}, 
			m_parent{ parentEnt }, 
			m_children{ },
			m_selected{ false }
		{}
	};

	class DirectoryInfo
	{
	public:
		std::string m_fileName;
		std::string m_filePath;
		std::string m_fileType;
		bool m_selected = false;
		int m_level = 0;

		DirectoryInfo() = default;
		DirectoryInfo(std::string_view path, std::string_view name, int level, std::string_view type) :
			m_filePath(path),
			m_fileName(name),
			m_level(level),
			m_fileType(type)
		{}
	};
}