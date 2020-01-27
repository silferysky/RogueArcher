#pragma once
#define MAX_COLLISION_LAYERS 32

namespace Rogue
{
	// Consists of collision layers mapped to their own names, and functions to deal with layers.
	class CollisionLayerer
	{
	public:
		using Bits = std::bitset<MAX_COLLISION_LAYERS>;

		static const std::pair<std::string, Bits> s_layerDefault;
	private:
		std::map<std::string, Bits> m_layers; // Collision masks mapped to its own name
	public:
		CollisionLayerer(); // Constructor

		void AddLayer(std::string_view name, const Bits& layer);
		void RemoveLayer(const Bits& layer);
		void RemoveLayer(std::string_view name);
		std::string_view GetName(const Bits& layer) const;
		bool FilterLayers(const Bits& maskA, const Bits& maskB) const;
	};

}// namespace Rogue