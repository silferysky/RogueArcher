/*Start Header*************************************************************************/
/*!
\file           ColliderComponent.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ColliderComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "ColliderComponent.h"
#include "Logger.h"

namespace Rogue
{
	ColliderComponent::ColliderComponent(const std::shared_ptr<Shape> ptr) :
		m_shape{ ptr }
	{}

	ColliderComponent::ColliderComponent(const ColliderComponent& rhs) :
		m_shape{ nullptr }
	{
		if (!rhs.m_shape)
		{
			RE_CORE_ERROR("Copied shape is nullptr!");
			m_shape.reset(new BoxShape);
			*m_shape = *rhs.m_shape;
			return;
		}

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
		if (!rhs.m_shape)
		{
			RE_CORE_ERROR("Copied shape is nullptr!");
			m_shape.reset(new BoxShape);
			*m_shape = *rhs.m_shape;
			return *this;
		}

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
			break;

		case Shape::Type::e_circle:
			ss << "CIRCLE";
			break;

		case Shape::Type::e_polygon:
			ss << "POLYGON";
			break;

		default:
			ss << "NONE";
			break;
		}

		return ss.str();
	}

	void ColliderComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
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