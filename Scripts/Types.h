#pragma once
#include <iostream>
#include <bitset>

using Entity = std::uint32_t;

const Entity MAX_ENTITIES = 32; // Test with 2048

using ComponentType = std::uint8_t;

const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;