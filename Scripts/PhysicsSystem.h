#pragma once


class PhysicsSystem
{
	void positionUpdate(const char* ID);
	void collisionUpdate(const char* ID);

public:
	PhysicsSystem()
	{}

	~PhysicsSystem()
	{}

	void init();
	void update();
};