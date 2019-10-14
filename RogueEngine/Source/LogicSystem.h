#pragma once
#include "ILogic.h"
#include "EventListener.h"

class LogicSystem :
	public System, public EventListener
{
public:
	//Logic System constructors/destructors
	LogicSystem();
	~LogicSystem();

	//For entity logic
	void AddLogicInterface(Entity, ILogic*);
	void RemoveLogicInterface(Entity);

	//Basic System 
	void init();
	void update() override;
	void receive(Event* ev) override;
	
private:
	std::map<Entity, ILogic*> m_entityLogicMap;
};

