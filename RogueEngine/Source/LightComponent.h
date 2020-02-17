#pragma once
#include "BaseComponent.h"
#include "GL/glew.h"
#include <string>

namespace Rogue
{
	class LightComponent
		: public BaseComponent
	{
		bool m_isActive = true;
		float m_ambientFactor = 1.0f;
		float m_diffuseFactor = 1.0f;
		float m_specularFactor = 1.0f;
		float m_attenuation = 1.0f;
	public:
		LightComponent() = default;
		~LightComponent() = default;

		void setIsActive(const bool& isActive);
		bool getIsActive() const;

		void setAmbientFactor(const float& ambientFactor);
		float getAmbientFactor() const;

		void setDiffuseFactor(const float& diffuseFactor);
		float getDiffuseFactor() const;

		void setSpecularFactor(const float& specularFactor);
		float getSpecularFactor() const;

		void setAttenuation(const float& attenuation);
		float getAttenuation() const;

		void DisplayOnInspector();
		//Serialize
		virtual std::string Serialize() override;
		virtual void Deserialize(std::string_view toDeserialize) override;
	};
}