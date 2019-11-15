#pragma once
#include <string>

namespace Rogue
{
	class ISerializable
	{
	public:
		virtual std::string Serialize() = 0;
		virtual void Deserialize(std::string_view deserializeStr) = 0;
		virtual void DisplayOnInspector() {};
	};
}