#include "Coordinator.h"
#include <vector>
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "FileIO.h"
Coordinator RECoordinator;

int main()
{
	std::ifstream ifs{ R"(../Resources/test.json)" };
	if (!ifs.is_open())
	{
		std::cerr << "Could not open file for reading!\n";
		return EXIT_FAILURE;
	}

	rapidjson::IStreamWrapper Wrapper{ ifs };

	rapidjson::Document doc{};
	doc.ParseStream(Wrapper);

	rapidjson::StringBuffer buffer{};
	rapidjson::Writer<rapidjson::StringBuffer> writer{ buffer };
	doc.Accept(writer);

	if (doc.HasParseError())
	{
		std::cout << "Error  : " << doc.GetParseError() << '\n'
			<< "Offset : " << doc.GetErrorOffset() << '\n';
		return EXIT_FAILURE;
	}

	const std::string jsonStr{ buffer.GetString() };

	std::cout << jsonStr << '\n';

	doc["ip"] = "127.0.0.1";
	doc["angle x"] = "1232132132";
	std::ofstream ofs{ R"(../Resources/test.json)" };
	if (!ofs.is_open())
	{
		std::cerr << "Could not open file for writing!\n";
		return EXIT_FAILURE;
	}

	rapidjson::OStreamWrapper osw{ ofs };
	rapidjson::Writer<rapidjson::OStreamWrapper> writer2{ osw };
	doc.Accept(writer2);

	//return EXIT_SUCCESS;

	RECoordinator.Init();

	RECoordinator.RegisterComponent<Gravity>();
	RECoordinator.RegisterComponent<RigidBody>();
	RECoordinator.RegisterComponent<Transform>();

	auto physicsSystem = RECoordinator.RegisterSystem<PhysicsSystem>();

	Signature signature;
	signature.set(RECoordinator.GetComponentType<Gravity>());
	signature.set(RECoordinator.GetComponentType<RigidBody>());
	signature.set(RECoordinator.GetComponentType<Transform>());
	RECoordinator.SetSystemSignature<PhysicsSystem>(signature);
	//change this when adding entities
	std::vector<Entity> entities(1);


	for (auto& entity : entities)
	{
		entity = RECoordinator.CreateEntity();

		RECoordinator.AddComponent(entity,Gravity{0.0f});
		RECoordinator.AddComponent(entity, RigidBody{ 0.0f,0.0f });
		RECoordinator.RemoveComponent<Gravity>(entity);
	}


	float dt = 0.0f;
	for (auto& entity : entities)
	{
		RECoordinator.DestroyEntity(entity);
	}
}