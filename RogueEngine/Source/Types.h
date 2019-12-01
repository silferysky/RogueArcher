#pragma once

namespace Rogue
{
	using Entity = std::uint32_t;

	const Entity MAX_ENTITIES = 65536;

	using ComponentType = std::uint8_t;

	const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;
}