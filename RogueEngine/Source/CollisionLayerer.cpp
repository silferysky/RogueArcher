#include "Precompiled.h"
#include "CollisionLayerer.h"

namespace Rogue
{
	const std::pair<std::string, CollisionLayerer::Bits> CollisionLayerer::s_layerDefault = std::make_pair("Default", Bits(1));

	CollisionLayerer::CollisionLayerer() :
		m_layers{}
	{
		m_layers.insert(s_layerDefault);
	}

	void CollisionLayerer::AddLayer(std::string_view name, const Bits& layer)
	{
		m_layers.emplace(std::make_pair(name, layer));
	}

	void CollisionLayerer::RemoveLayer(std::string_view name)
	{
		m_layers.erase(std::string(name));
	}

	std::string_view CollisionLayerer::GetName(const Bits& layer) const
	{
		std::map<std::string, Bits>::const_iterator iName = std::find_if(std::begin(m_layers), std::end(m_layers),
			[&](const std::pair<std::string, Bits>& pair)
			{
				return pair.second == layer;
			});

		return (*iName).first;
	}

	void CollisionLayerer::RemoveLayer(const Bits& layer)
	{
		// TODO: Remove by finding layer
	}

	bool CollisionLayerer::FilterLayers(const Bits& maskA, const Bits& maskB) const
	{
		return (maskA & maskB) == 1;
	}
}