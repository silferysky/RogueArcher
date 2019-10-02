#pragma once

enum MaterialList
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
	float m_damping;

public:
	MaterialComponent(MaterialList material = MAT_NONE);
	~MaterialComponent();
};

