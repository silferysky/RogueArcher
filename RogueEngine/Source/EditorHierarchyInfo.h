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
		float m_scaleX = 0.0f;
		float m_scaleY = 0.0f;
		float m_rotateX = 0.0f;
		float m_rotateY = 0.0f;
		float m_translateX = 0.0f;
		float m_translateY = 0.0f;
	};
}