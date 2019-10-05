#pragma once

enum COMPONENTID
{
	RIGIDBODY = 0,
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

