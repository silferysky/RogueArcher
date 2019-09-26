#pragma once

enum ComponentType
{
	//Generic Components
	CMP_BASE = 0,
	CMP_TRANSFORM,

	//Physics Components
	CMP_RIGIDBODY,
	CMP_PHYMAT,
	CMP_COLLIDER,
	
	//Graphics Components
	CMP_SPRITE,
	CMP_ANIMATION,
	CMP_TEXT,
	CMP_CAMERA,

	//Particle Components
	CMP_PARTICLE,
	CMP_EMITTER,

	CMP_LAST //Signifies the last component of list
};