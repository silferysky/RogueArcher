#include <sstream>
#include "ColliderComponent.h"

namespace Rogue
{
	ColliderComponent::ColliderComponent(const std::shared_ptr<Shape> ptr) :
		m_shape{ ptr }
	{}

	ColliderComponent::ColliderComponent(const ColliderComponent& rhs) :
		m_shape{ nullptr }
	{
		switch (rhs.m_shape->GetType())
		{
		case Shape::Type::e_box:
			m_shape.reset(new BoxShape);
			*m_shape = *rhs.m_shape;
			break;

		case Shape::Type::e_circle:
			m_shape.reset(new CircleShape);
			*m_shape = *rhs.m_shape;
			break;

		case Shape::Type::e_polygon:
			m_shape.reset(new PolygonShape);
			*m_shape = *rhs.m_shape;
			break;
			
		default:
			m_shape = nullptr;
		}
	}

	ColliderComponent::ColliderComponent(ColliderComponent&& rhs) noexcept :
		m_shape{ nullptr }
	{
		std::swap(m_shape, rhs.m_shape);
	}

	ColliderComponent& ColliderComponent::operator=(const ColliderComponent& rhs)
	{
		if (this != &rhs)
		{
			switch (rhs.m_shape->GetType())
			{
			case Shape::Type::e_box:
				m_shape.reset(new BoxShape);
				*m_shape = *rhs.m_shape;
				break;

			case Shape::Type::e_circle:
				m_shape.reset(new CircleShape);
				*m_shape = *rhs.m_shape;
				break;

			case Shape::Type::e_polygon:
				m_shape.reset(new PolygonShape);
				*m_shape = *rhs.m_shape;
				break;

			default:
				m_shape = nullptr;
			}
		}
		return *this;
	}

	ColliderComponent& ColliderComponent::operator=(ColliderComponent&& rhs) noexcept
	{
		if (this != &rhs)
		{
			// Swap with rhs shared ptr.
			m_shape.swap(rhs.m_shape);
		}

		return *this;
	}

	std::string ColliderComponent::Serialize()
	{
		std::ostringstream ss;

		switch (m_shape->GetType())
		{
		case Shape::Type::e_box:
			ss << "BOX";

		case Shape::Type::e_circle:
			ss << "CIRCLE";

		case Shape::Type::e_polygon:
			ss << "POLYGON";

		default:
			ss << "NONE";
		}

		return ss.str();
	}

	void ColliderComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;

		std::getline(ss, s1, ';');

		if (s1 == "BOX")
			m_shape = std::make_shared<BoxShape>();
		else if (s1 == "CIRCLE")
			m_shape = std::make_shared<CircleShape>();
		else if (s1 == "POLYGON")
			m_shape = std::make_shared<PolygonShape>();
	}


	std::shared_ptr<Shape> ColliderComponent::GetShape() const
	{
		return m_shape;
	}

	void ColliderComponent::SetShape(const std::shared_ptr<Shape>& pShape)
	{
		switch (pShape->GetType())
		{
		case Shape::Type::e_box:
			m_shape.reset(new BoxShape);
			*m_shape = *pShape;
			break;

		case Shape::Type::e_circle:
			m_shape.reset(new CircleShape);
			*m_shape = *pShape;
			break;

		case Shape::Type::e_polygon:
			m_shape.reset(new PolygonShape);
			*m_shape = *pShape;
			break;
		}

	}
}