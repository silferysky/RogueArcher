#pragma once
#include <iostream>
#include <bitset>

const std::uint32_t MAX_ENTITIES = 5000;

const std::uint8_t MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;