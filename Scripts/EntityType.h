#pragma once

enum ENTITY_TYPE
{
	//Generic Entities
	ENT_BASE = 0,

	//GameObject Entities

	//Player-related
	ENT_PLAYER,
	ENT_ARROW,

	//AI-related
	//To be renamed when full list of implementable AI is designed
	ENT_AI,
	ENT_PROP,
	ENT_OBSTACLE,

	//Other interactable GameObjects
	ENT_PROJECTILE,
	ENT_WALL,

	//Non-interactable GameObjects
	ENT_BACKGROUND,
	ENT_CAMERA,
	ENT_PARTICLE,

	//Menu Related
	ENT_MCURSOR,
	ENT_BUTTON
	
};