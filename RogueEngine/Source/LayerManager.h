#pragma once
#define MAX_COLLISION_LAYERS 32

namespace Rogue
{
	// Consists of collision layers mapped to their own names, and functions to deal with layers.
	class LayerManager
	{
	public:
		using Bits = std::bitset<MAX_COLLISION_LAYERS>;

		static LayerManager& instance()
		{
			static LayerManager instance;
			return instance;
		}

		static const std::pair<size_t, std::string> s_layerDefault;
	private:
		std::map<size_t, std::string> m_layers; // Collision masks mapped to its own name
	public:
		LayerManager(); // Constructor

		void AddLayer(std::string_view name);
		void RemoveLayer(size_t layer);
		void RemoveLayer(std::string_view name);
		std::string_view GetName(size_t pos) const;
		std::string_view GetName(const Bits& layer) const;
		bool FilterLayers(const Bits& maskA, const Bits& maskB) const;

		void PrintNames() const;
		void PrintMask(const Bits& mask) const;
		size_t GetLayerSize() const;
		size_t GetLayerCategory(const Bits& category) const;
		int GetLayerCategory(std::string_view name) const;
	};

}// namespace Rogue