#include <sstream>
#include <vector>
#include "BoxCollider2DComponent.h"
#include "Vector2D.h"
#include "Logger.h"

namespace Rogue
{
	std::string BoxCollider2DComponent::Serialize()
	{
		//Size, modelVertexList
		std::ostringstream ss;
		ss << m_obb.getSize() << ";";

		for (size_t i = 0; i < m_obb.getSize(); ++i)
		{
			ss << m_obb.modelVerts()[i].x << ";" << m_obb.modelVerts()[i].y << ";";
		}

		return ss.str();
	}

	void BoxCollider2DComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1, s2;		//s2 is used if two are needed
		std::vector<Vec2> vertexList{};
		int size = 0;

		if (std::getline(ss, s1, ';'))
		{
			size = static_cast<size_t>(stoi(s1));
		}

		for (size_t counter = 0; counter < size; counter++)
		{
			//In this case, 1st value is only non double value read
			//if (counter > 0)
			//	std::getline(ss, s2, ';');

			//switch (counter)
			//{
			//case 0:
			//	size = static_cast<size_t>(stoi(s1));
			//	break;
			//default:
			//	vertexList.push_back(Vec2(stof(s1), stof(s2)));
			//	//vertexList.push_back(Vec2());
			//	break;
			//}

			std::getline(ss, s1, ';');
			std::getline(ss, s2, ';');
			vertexList.push_back(Vec2(stof(s1), stof(s2)));
		}

		m_obb.setModelVerts(vertexList);
		m_obb.setSize(size);
	}

	void BoxCollider2DComponent::operator=(const BoxCollider2DComponent& rhs)
	{
		m_obb.setSize(rhs.m_obb.getSize());
		std::vector<Vec2> vertexList{};

		for (size_t sz = 0; sz < m_obb.getSize(); ++sz)
		{
			vertexList.push_back(Vec2());
		}
		m_obb.setModelVerts(vertexList);
	}
}