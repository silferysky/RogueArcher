/*Start Header*************************************************************************/
/*!
\file           ColliderComponent.h
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
#pragma once
#include "PhysicsDataStructures.hpp"
#include "BaseComponent.h"
#include "LayerManager.h"

namespace Rogue
{
	class ColliderComponent : public BaseComponent
	{
		std::shared_ptr<Shape> m_shape;
		LayerManager::Bits m_collisionMask;
		LayerManager::Bits m_collisionCategory;
	public:
		ColliderComponent(const std::shared_ptr<Shape> ptr = nullptr);
		ColliderComponent(const ColliderComponent& rhs); // Copy constructor
		ColliderComponent(ColliderComponent&& rhs) noexcept; // Move constructor
		ColliderComponent& operator=(const ColliderComponent& rhs); // Copy assignment
		ColliderComponent& operator=(ColliderComponent&& rhs) noexcept; // Move assignment

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
		void DisplayOnInspector();

		std::shared_ptr<Shape> GetShape() const;
		const LayerManager::Bits& GetCollisionMask() const;
		const LayerManager::Bits& GetCollisionCat() const;

		void SetShape(const std::shared_ptr<Shape>& pShape);
		void SetMask(size_t layerPos, bool set = true);
		void SetCollisionMask(const LayerManager::Bits& bits);
		void SetCollisionCat(const LayerManager::Bits& layer);
		void ChangeLayer(size_t pos);
		void ChangeLayer(std::string_view name);
	};

}