#include "Precompiled.h"
#include "LightComponent.h"
#include "Main.h"

namespace Rogue
{
	void LightComponent::setIsActive(const bool& isActive)
	{
		m_isActive = isActive;
	}

	bool LightComponent::getIsActive() const
	{
		return m_isActive;
	}

	void LightComponent::DisplayOnInspector()
	{
		ImVec2 imageSize{ ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 8 };
		//ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PushItemWidth(75);

		ImGui::Checkbox("Active?", &m_isActive);
		setIsActive(m_isActive);
	}

	std::string LightComponent::Serialize()
	{
		std::ostringstream ss;
		return ss.str();
	}

	void LightComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;		//s2 is used if two are needed
		int counter = 0;

		while (std::getline(ss, s1, ';'))
		{
			switch (counter)
			{
			default:
				break;
			}

			++counter;
		}
	}
}