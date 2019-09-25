#pragma once

enum COMPONENTID
{
	RIGIDBODY = 0,
	SPRITE,
	LASTCOMP
};

class BaseComponent
{
public:
	BaseComponent();
	BaseComponent(COMPONENTID id);
	~BaseComponent();

private:
	COMPONENTID ID;
};

