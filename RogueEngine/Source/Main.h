#pragma once

#include "REEngine.h"
#include "ObjectFactory.h"
#include "GameStateList.h"
#include <Windows.h>
#include "Resource.h"
#include "ComponentList.h"

#if _DEBUG
#include <iostream>
#endif

class REEngine;
class ObjectFactory;

extern REEngine gEngine;
extern ObjectFactory gObjectFactory;
extern float gDeltaTime;
