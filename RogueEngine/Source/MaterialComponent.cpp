#include "MaterialComponent.h"

MaterialComponent::MaterialComponent(MaterialList material) : m_material{ material }, m_density { 0.0f }, m_friction{ 0.0f }
{}

MaterialComponent::~MaterialComponent()
{}