/* Start Header ************************************************************************/
/*!
\file           MaterialComponent.h
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
#pragma once

namespace Rogue
{
	enum class MaterialList
	{
		MAT_NONE = 0,
		MAT_A,
		MAT_B,
		MAT_C,
		MAT_ETC,

		MAT_MAX
	};

	class MaterialComponent
	{
		MaterialList m_material;
		float m_density;
		float m_friction;

	public:
		MaterialComponent(MaterialList material = MaterialList::MAT_NONE);
		~MaterialComponent();
	};

}