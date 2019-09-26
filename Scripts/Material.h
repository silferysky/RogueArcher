#pragma once
#include "MaterialList.h"

class Material
{
	MaterialList m_material;
	float m_density;
	float m_friction;

public:
	Material(MaterialList material = MAT_NONE);
	~Material();

};

