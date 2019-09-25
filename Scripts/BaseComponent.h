#pragma once


enum COMPONENTID
{
	SPRITE = 0,
	TRANSFORM,
	RIGIDBODY,
	BOXCOLLIDER2D,
	CIRCLECOLLIDER2D,
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

