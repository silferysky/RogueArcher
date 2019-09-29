#include "REEngine.h"

void REEngine::init()
{
	m_coordinator.Init();
}

void REEngine::update()
{
//	RE_CORE_INFO("Engine update");
	m_coordinator.update();
}
