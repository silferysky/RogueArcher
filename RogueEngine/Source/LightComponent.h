/* Start Header ************************************************************************/
/*!
\file           LightComponent.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for LightComponent

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "BaseComponent.h"
#include "GL/glew.h"
#include <string>
#include <glm.hpp>

namespace Rogue
{
	class LightComponent
		: public BaseComponent
	{
		bool m_isActive = true;
		float m_ambientFactor = 1.0f;
		float m_specularFactor = 1.0f;
		float m_radius = 1.0f;

		glm::vec4 m_tint = { 1.0f, 1.0f, 1.0f, 1.0f };
	public:
		LightComponent() = default;
		~LightComponent() = default;

		void setIsActive(const bool& isActive);
		bool getIsActive() const;

		void setAmbientFactor(const float& ambientFactor);
		float getAmbientFactor() const;

		void setSpecularFactor(const float& specularFactor);
		float getSpecularFactor() const;

		void setRadius(const float& radius);
		float getRadius() const;

		glm::vec4& getTint();
		void setTint(const glm::vec4& tint);

		void DisplayOnInspector();

		//Serialize
		virtual std::string Serialize() override;
		virtual void Deserialize(std::string_view toDeserialize) override;
	};
}