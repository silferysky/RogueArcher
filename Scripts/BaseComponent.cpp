#include "BaseComponent.h"



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
