#pragma once
#include <vector>
#include "Types.h"
//#include "ObjectFactory.h"
//#include <memory>

namespace Rogue
{
	class SceneManager
	{
		//std::unique_ptr<ObjectFactory> m_objectFactory;
		std::vector<Entity> m_activeEntities;

	public:
		SceneManager();
	};
}