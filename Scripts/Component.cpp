#include "Component.h"



Component::Component()
{
	ID = LASTCOMP;
	nextComp = nullptr;
}

Component::Component(COMPONENTID id)
{
	ID = id;
	nextComp = nullptr;
}

Component::~Component()
{
}
