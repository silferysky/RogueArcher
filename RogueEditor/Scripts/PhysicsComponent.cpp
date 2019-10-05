#include "PhysicsComponent.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

const std::string config_path = "test.json";
void init()
{
	nlohmann::json j;
	std::ifstream is(config_path);
	j = nlohmann::json::parse(is);
}