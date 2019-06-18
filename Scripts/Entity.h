#pragma once

enum ENTITYID
{
	PLAYERCHAR = 0,
	ENEMYCHAR,
	OBSTACLECHAR,
	FRIENDLYCHAR,
	LASTENT
};

class Entity
{
public:
	Entity();
	Entity(ENTITYID id);
	~Entity();
	
private:
	ENTITYID ID;
	Entity* nextEnt;
};

