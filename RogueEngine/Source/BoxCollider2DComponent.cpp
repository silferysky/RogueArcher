#include <sstream>
#include <vector>
#include "BoxCollider2DComponent.h"
#include "Vector2D.h"
#include "Logger.h"

namespace Rogue
{
	BoxCollider2DComponent::BoxCollider2DComponent() :
		m_rotatable{ false }, m_aabb{}, m_obb{}//, m_shape{std::make_unique<BoxShape>()}
	{}

	BoxCollider2DComponent::BoxCollider2DComponent(const BoxCollider2DComponent& rhs) :
		m_rotatable{ rhs.m_rotatable }, m_aabb{ rhs.m_aabb }, m_obb{ rhs.m_obb }//,
	//	m_shape{ }
	{
		//if (m_rotatable)
		//	m_shape = std::make_unique<PolygonShape>(m_obb);
		//else
		//	m_shape = std::make_unique<BoxShape>(m_aabb);
	}

	BoxCollider2DComponent::BoxCollider2DComponent(BoxCollider2DComponent&& rhs) noexcept :
		m_rotatable{ false }, m_aabb{ AABB{} }, m_obb{ OBB{} }
	{
		std::swap(m_rotatable, rhs.m_rotatable);
		std::swap(m_aabb, rhs.m_aabb);
		std::swap(m_obb, rhs.m_obb);
	}

	BoxCollider2DComponent& BoxCollider2DComponent::operator=(const BoxCollider2DComponent& rhs)
	{
		if (this != &rhs)
		{
			m_rotatable = rhs.m_rotatable;

			m_obb.setSize(rhs.m_obb.getSize());
			std::vector<Vec2> vertexList{};

			vertexList.reserve(m_obb.getSize());

			m_obb.setModelVerts(vertexList);

		//	if (m_rotatable)
		//		m_shape = std::make_unique<PolygonShape>(m_obb);
		//	else
		//		m_shape = std::make_unique<BoxShape>(m_aabb);
		}
		return *this;
	}

	BoxCollider2DComponent& BoxCollider2DComponent::operator=(BoxCollider2DComponent&& rhs) noexcept
	{
		if (this != &rhs)
		{
			std::swap(m_rotatable, rhs.m_rotatable);
			std::swap(m_aabb, rhs.m_aabb);
			std::swap(m_obb, rhs.m_obb);
		}

		return *this;
	}

	std::string BoxCollider2DComponent::Serialize()
	{
		//Size, modelVertexList
		std::ostringstream ss;
		ss << m_obb.getSize() << ";";

		for (size_t i = 0; i < m_obb.getSize(); ++i)
		{
			ss << m_obb.modelVerts()[i].x << ";" << m_obb.modelVerts()[i].y << ";";
		}

		ss << m_aabb.getCenterOffSet().x << ";";
		ss << m_aabb.getCenterOffSet().y << ";";

		ss << m_aabb.getScaleOffSet().x << ";";
		ss << m_aabb.getScaleOffSet().y;

		return ss.str();
	}

	void BoxCollider2DComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1, s2;		//s2 is used if two are needed
		std::vector<Vec2> vertexList{};
		size_t size = 0;

		// For OBB
		if (std::getline(ss, s1, ';'))
		{
			size = static_cast<size_t>(stoi(s1));
		}

		for (size_t counter = 0; counter < size; counter++)
		{
			std::getline(ss, s1, ';');
			std::getline(ss, s2, ';');
			vertexList.emplace_back(Vec2(stof(s1), stof(s2)));
		}

		m_obb.setModelVerts(vertexList);
		m_obb.setSize(size);

		// For AABB
		float x = 0.0f;
		float y = 0.0f;

		if(std::getline(ss, s1, ';'))
			x = std::stof(s1);

		if (std::getline(ss, s1, ';'))
			y = std::stof(s1);

		m_aabb.setCenterOffSet(Vec2{ x, y });

		if (std::getline(ss, s1, ';'))
			x = std::stof(s1);

		if (std::getline(ss, s1, ';'))
			y = std::stof(s1);

		m_aabb.setScaleOffSet(Vec2{ x, y });
	}


	bool BoxCollider2DComponent::Rotatable() const
	{
		return m_rotatable;
	}

	void BoxCollider2DComponent::setRotatable(bool set)
	{
		//if (set)
		//{
		//	m_shape = std::make_unique<PolygonShape>(m_obb);
		//}
		//else
		//{
		//	m_shape = std::make_unique<BoxShape>(m_aabb);
		//}

		m_rotatable = set;
	}
}