#include "BaseComponent.h"
#include "json.hpp"


BaseComponent::BaseComponent()
{
	ID = LASTCOMP;
}

BaseComponent::BaseComponent(COMPONENTID id)
{
	ID = id;
}

BaseComponent::~BaseComponent()
{
}
