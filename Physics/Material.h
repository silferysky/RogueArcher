#pragma once
#include "../Scripts/BaseComponent.h"

enum MaterialList
{
	MAT_NONE = 0,
	MAT_A,
	MAT_B,
	MAT_C,
	MAT_ETC,

	MAT_MAX
};

class Material : public BaseComponent
{
	MaterialList m_material;
	float m_density;
	float m_friction;

public:
	Material(MaterialList material = MAT_NONE);
	~Material();

};

