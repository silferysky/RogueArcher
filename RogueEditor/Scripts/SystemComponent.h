#pragma once

class System;

class SystemComponent
{
public:
	System* RogueEngine;
	SystemComponent() {}
	SystemComponent(System* _systemPointer) :RogueEngine{ _systemPointer } {}
	virtual ~SystemComponent() {};
	virtual void Initialize() {}
	virtual void Load() {}
	virtual void Update() {}
	virtual void Unload() {}
};