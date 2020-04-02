/* Start Header ************************************************************************/
/*!
\file           FadeComponent.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for FadeComponent

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
