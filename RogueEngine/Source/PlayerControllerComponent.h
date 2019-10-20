#pragma once
#include "BaseComponent.h"

class PlayerControllerComponent
	: public BaseComponent
{
public:
	PlayerControllerComponent() = default;
	~PlayerControllerComponent() = default;

	//No need to serialize PlayerControllerComponent as of now
	std::string Serialize() { return std::string(); }
	void Deserialize(std::string) {}
};

