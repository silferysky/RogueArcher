#pragma once
#include "BaseComponent.h"
#include "GL/glew.h"
#include <string>

namespace Rogue
{
	class CheckpointComponent
		: public BaseComponent
	{
		bool m_isActive = true;
	public:
		CheckpointComponent() = default;
		~CheckpointComponent() = default;

		void setIsActive(const bool& isActive);
		bool getIsActive() const;

		void DisplayOnInspector();
		//Serialize
		virtual std::string Serialize() override;
		virtual void Deserialize(std::string_view toDeserialize) override;
	};
}
