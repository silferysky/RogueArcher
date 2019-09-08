#pragma once

enum COMPONENTID
{
	RIGIDBODY = 0,
	LASTCOMP
};

class Component
{
public:
	Component();
	Component(COMPONENTID id);
	~Component();

private:
	COMPONENTID ID;
};

