/* Start Header ************************************************************************/
/*!
\file           MaskingComponent.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for MaskingComponent

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
	class MaskingComponent
		: public BaseComponent
	{
		int m_frameSize = 0; // as a fraction of the image
		float m_timer = 0.0f;
		float m_maskVelocity = 0.0f; // how quickly the mask frame moves
		bool m_isActive = true;
		bool m_isLooping = false;
	public:
		MaskingComponent() = default;
		~MaskingComponent() = default;

		void setFrameSize(const int& frameSize);
		int getFrameSize() const;

		void setTimer(const float& timer);
		float getTimer() const;

		void setMaskVelocity(const float& maskVelocity);
		float getMaskVelocity() const;

		void setIsActive(const bool& isActive);
		bool getIsActive() const;

		void setIsLooping(const bool& isLooping);
		bool getIsLooping() const;

		void DisplayOnInspector();
		//Serialize
		virtual std::string Serialize() override;
		virtual void Deserialize(std::string_view toDeserialize) override;
	};
}
