#include "PhysicsDataStructures.hpp"
#include "BaseComponent.h"

namespace Rogue
{
	class ColliderComponent : public BaseComponent
	{
		std::shared_ptr<Shape> m_shape;
	public:
		ColliderComponent(const std::shared_ptr<Shape> ptr = nullptr);
		ColliderComponent(const ColliderComponent& rhs); // Copy constructor
		ColliderComponent(ColliderComponent&& rhs) noexcept; // Move constructor
		ColliderComponent& operator=(const ColliderComponent& rhs); // Copy assignment
		ColliderComponent& operator=(ColliderComponent&& rhs) noexcept; // Move assignment

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		std::shared_ptr<Shape> GetShape() const;
		void SetShape(const std::shared_ptr<Shape>& pShape);
	};

}