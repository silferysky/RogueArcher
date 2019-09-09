#pragma once

enum ENTITYID
{
	PLAYERCHAR = 0,
	ENEMYCHAR,
	OBSTACLECHAR,
	FRIENDLYCHAR,
	LASTENT
};

class BaseEntity
{
public:
	BaseEntity();
	BaseEntity(ENTITYID id);
	~BaseEntity();
	
private:
	ENTITYID ID;
};

