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
#include "CollisionManager.h"

#define MAX_32BIT_NUMBER 0xFFFFFFFF
#define MAX_16BIT_NUMBER 0x7FFF

namespace Rogue
{
	ColliderComponent::ColliderComponent(const std::shared_ptr<Shape> ptr) :
		m_shape{ ptr },
		m_collisionCategory{ 0 },
		m_collisionMask{ MAX_32BIT_NUMBER }
	{
		size_t layer = LayerManager::s_layerDefault.first;
		m_collisionCategory.set(layer);
		m_collisionMask.set(layer);
	}

	ColliderComponent::ColliderComponent(const ColliderComponent& rhs) :
		m_shape{ nullptr },
		m_collisionCategory{ rhs.m_collisionCategory },
		m_collisionMask{ rhs.m_collisionMask }
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
		m_shape{ nullptr },
		m_collisionCategory{ 0 },
		m_collisionMask{ 0 }
	{
		std::swap(m_shape, rhs.m_shape);
		std::swap(m_collisionCategory, rhs.m_collisionCategory);
		std::swap(m_collisionMask, rhs.m_collisionMask);
	}

	ColliderComponent& ColliderComponent::operator=(const ColliderComponent& rhs)
	{
		if (!rhs.m_shape)
		{

			RE_CORE_ERROR("Copied collider shape is nullptr!");

			m_shape.reset(new BoxShape);
			*m_shape = *rhs.m_shape;
			return *this;
		}

		if (this != &rhs)
		{
			m_collisionCategory = rhs.m_collisionCategory;
			m_collisionMask = rhs.m_collisionMask;

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
			std::swap(m_collisionCategory, rhs.m_collisionCategory);
			std::swap(m_collisionMask, rhs.m_collisionMask);
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

		// Divider
		ss << ";";

		// Collision category
		ss << m_collisionCategory.to_ulong() << ";";

		// Collision mask
		ss << m_collisionMask.to_ulong() << ";";

		return ss.str();
	}

	void ColliderComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;

		if (std::getline(ss, s1, ';'))
		{
			if (s1 == "BOX")
				m_shape = std::make_shared<BoxShape>();
			else if (s1 == "CIRCLE")
				m_shape = std::make_shared<CircleShape>();
			else if (s1 == "POLYGON")
				m_shape = std::make_shared<PolygonShape>();
		}
		
		if (std::getline(ss, s1, ';'))
			m_collisionCategory = LayerManager::Bits(std::stoull(s1));
		else
			m_collisionCategory = LayerManager::s_layerDefault.first;

		if (std::getline(ss, s1, ';'))
			m_collisionMask = LayerManager::Bits(std::stoull(s1));
		else
			m_collisionMask = LayerManager::Bits(MAX_32BIT_NUMBER);
	}

	void ColliderComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);

		size_t cat = CollisionManager::instance().GetLayerCategory(m_collisionCategory);
		std::string name = std::string(CollisionManager::instance().GetLayerName(cat));
	
		ImGui::NewLine();
		ImGui::Text("Current Layer: %s", name.c_str());
		
		size_t numLayers = CollisionManager::instance().GetNumberOfLayers();
		std::stringstream ss;

		if (ImGui::Button("Change Layer"))
		{
			ImGui::OpenPopup("Change Layer");
		}
		
		if (ImGui::BeginPopup("Change Layer"))
		{
			if (ImGui::BeginMenu("Layers"))
			{
				for (unsigned pos = 0; pos < numLayers; pos++)
				{
					ss << CollisionManager::instance().GetLayerName(pos);
					bool ownLayer = m_collisionCategory.test(pos);
					
					if (ImGui::MenuItem(ss.str().c_str(), nullptr, false, !ownLayer))
					{
						ChangeLayer(pos);
					}

					CLEARSTRING(ss);

				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		bool checked;
		ImGui::NewLine();
		ImGui::Text("Collides With:");

		for(unsigned pos = 0; pos < numLayers; pos++)
		{
			checked = m_collisionMask[pos];
			ss << CollisionManager::instance().GetLayerName(pos);
			ImGui::Checkbox(ss.str().c_str(), &checked);

			CLEARSTRING(ss);

			m_collisionMask.set(pos, checked);
		}
		ImGui::NewLine();
	}

	std::shared_ptr<Shape> ColliderComponent::GetShape() const
	{
		return m_shape;
	}

	const LayerManager::Bits& ColliderComponent::GetCollisionMask() const
	{
		return m_collisionMask;
	}

	const LayerManager::Bits& ColliderComponent::GetCollisionCat() const
	{
		return m_collisionCategory;
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

	void ColliderComponent::SetMask(size_t layerPos, bool set)
	{
		m_collisionMask.set(layerPos, set);
	}

	void ColliderComponent::SetCollisionMask(const LayerManager::Bits& bits)
	{
		m_collisionMask = bits;
	}
	
	void ColliderComponent::SetCollisionCat(const LayerManager::Bits& layer)
	{
		m_collisionCategory = layer;
	}


	void ColliderComponent::ChangeLayer(size_t pos)
	{
		if (pos)
			m_collisionCategory = LayerManager::Bits(1ULL << pos);
		else
			m_collisionCategory = 0;
	}

	void ColliderComponent::ChangeLayer(std::string_view name)
	{
		int pos = LayerManager::instance().GetLayerCategory(name);
		
		if (pos != -1)
		{
			if (pos)
				m_collisionCategory = LayerManager::Bits(1ULL << pos);
			else
				m_collisionCategory = 0;;
		}
	}
}