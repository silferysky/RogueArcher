/* Start Header ************************************************************************/
/*!
\file           BackgroundComponent.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for BackgroundComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class BackgroundComponent
		: public BaseComponent
	{
		float m_parallax = 1.2f;
	public:
		BackgroundComponent() = default;
		~BackgroundComponent() = default;

		float GetParallax() const;
		void SetParallax(const float& parallax);

		//Serialize
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void DisplayOnInspector();
	};
}