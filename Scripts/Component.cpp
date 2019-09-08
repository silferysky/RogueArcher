#include "Component.h"



Component::Component()
{
	ID = LASTCOMP;
}

Component::Component(COMPONENTID id)
{
	ID = id;
}

Component::~Component()
{
}
