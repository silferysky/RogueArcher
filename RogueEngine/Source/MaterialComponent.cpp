/* Start Header ************************************************************************/
/*!
\file           MaterialComponent.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for MaterialComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "MaterialComponent.h"

namespace Rogue
{
	MaterialComponent::MaterialComponent(MaterialList material) : m_material{ material }, m_density{ 0.0f }, m_friction{ 0.0f }
	{}

	MaterialComponent::~MaterialComponent()
	{}
}