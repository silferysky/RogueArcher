#include "Precompiled.h"
#include "LayerManager.h"

namespace Rogue
{
	const std::pair<size_t, std::string> LayerManager::s_layerDefault = std::make_pair(0, "Default");

	LayerManager::LayerManager() :
		m_layers{}
	{
		m_layers.emplace(s_layerDefault);
		AddLayer("Light");
		AddLayer("Dark");
	}

	void LayerManager::AddLayer(std::string_view name)
	{
		m_layers.emplace(std::make_pair(m_layers.size(), name));
	}

	void LayerManager::RemoveLayer(size_t pos)
	{
		m_layers.erase(pos);
	}

	void LayerManager::RemoveLayer(std::string_view name)
	{
		// TODO: Remove by finding layer

	}

	std::string_view LayerManager::GetName(size_t pos) const
	{
		auto it = m_layers.find(pos);

		if (it != m_layers.end())
			return it->second;
		else
			return "";
	}

	bool LayerManager::FilterLayers(const Bits& maskA, const Bits& maskB) const
	{
		return (maskA & maskB) != 0;
	}

	void LayerManager::PrintNames() const
	{
		ImGui::Text("Collision Layers");

		for (auto& pair : m_layers)
		{
			ImGui::Text("%s", pair.second.c_str());
			ImGui::SameLine();
			ImGui::PushItemWidth(75);
		}
		ImGui::NewLine();
	}

	void LayerManager::PrintMask(const Bits& mask) const
	{
		ImGui::Text("Collides with:");

		for (auto& pair : m_layers)
		{
			if (mask.test(pair.first))
			{
				ImGui::Text("%s", pair.second.c_str());
			}
		}

		ImGui::Text("Doesn't collide with:");

		for (auto& pair : m_layers)
		{
			if (!mask.test(pair.first))
			{
				ImGui::Text("%s", pair.second.c_str());
			}
		}
	}

	size_t LayerManager::GetLayerSize() const
	{
		return m_layers.size();
	}
	
	size_t LayerManager::GetLayerCategory(const Bits& category) const
	{
		size_t ulong = category.to_ulong();
		size_t pos = 0;

		while (ulong % 2 == 0)
		{
			pos++;
			ulong /= 2;
		}
		return pos;
	}

	int LayerManager::GetLayerCategory(std::string_view name) const
	{
		auto it = std::find_if(std::begin(m_layers), std::end(m_layers), [&](const std::pair<size_t, std::string>& pair)
			{
				return pair.second == name;
			});

		if (it != m_layers.end())
			return static_cast<int>(it->first);
		else
			return -1;
	}
}