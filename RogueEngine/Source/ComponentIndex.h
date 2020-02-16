#pragma once

namespace Rogue
{
	struct TypeCounter
	{
		static ComponentType value;
	};

	template <typename TComponent>
	struct TypeID
	{
		static ComponentType value()
		{
			// Value only inits once, the moment this template is instantiated.
			// So, for each component type, it has its own value.
			static ComponentType value = TypeCounter::value++;

			return value;
		}
	};
}