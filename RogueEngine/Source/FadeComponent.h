#pragma once
#include "BaseComponent.h"
#include "GL/glew.h"
#include <string>

namespace Rogue
{
	class FadeComponent
		: public BaseComponent
	{
		float m_fadeVelocity = 1.0f; // how quickly the sprite fades
		bool m_isActive = true;
		bool m_isFadingIn = true;
	public:
		FadeComponent() = default;
		~FadeComponent() = default;

		void setFadeVelocity(const float& fadeVelocity);
		float getFadeVelocity() const;

		void setIsActive(const bool& isActive);
		bool getIsActive() const;

		void setIsFadingIn(const bool& isFadingIn);
		bool getIsFadingIn() const;

		void DisplayOnInspector();
		//Serialize
		virtual std::string Serialize() override;
		virtual void Deserialize(std::string_view toDeserialize) override;
	};
}
