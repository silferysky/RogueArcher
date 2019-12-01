/*Start Header*************************************************************************/
/*!
\file           Circle.h
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
#pragma once


namespace Rogue
{
	// Circle class
	class Circle
	{
		float m_radius;

	public:
		Circle(float radius);
		~Circle();

		inline void setRadius(float rad);
		inline float getRadius() const;
	};
}