/*Start Header*************************************************************************/
/*!
\file           Circle.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Circle

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "Circle.h"

namespace Rogue
{
	Circle::Circle(float radius) : m_radius{ radius }
	{}

	Circle::~Circle()
	{}

	inline void Circle::setRadius(float rad)
	{
		m_radius = rad;
	}

	inline float Circle::getRadius() const
	{
		return m_radius;
	}
}