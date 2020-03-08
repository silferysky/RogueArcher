/* Start Header ************************************************************************/
/*!
\file           BaseComponent.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for BaseComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "ISerializable.h"

namespace Rogue
{
	class BaseComponent
		: public ISerializable
	{
	public:
		bool m_componentIsActive;

		BaseComponent() : m_componentIsActive(true) {};
		virtual ~BaseComponent() = default;

	};
}