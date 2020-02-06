#include "Precompiled.h"
#include "CheckpointComponent.h"
#include "Main.h"

namespace Rogue
{
	void CheckpointComponent::setIsActive(const bool& isActive)
	{
		m_isActive = isActive;
	}

	bool CheckpointComponent::getIsActive() const
	{
		return m_isActive;
	}

	void CheckpointComponent::DisplayOnInspector()
	{
		ImVec2 imageSize{ ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 8 };
		//ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PushItemWidth(75);

		ImGui::Checkbox("Active?", &m_isActive);
		setIsActive(m_isActive);
	}

	std::string CheckpointComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_isActive << ";";
		return ss.str();
	}

	void CheckpointComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;		//s2 is used if two are needed
		int counter = 0;

		while (std::getline(ss, s1, ';'))
		{
			switch (counter)
			{
			case 0:
				setIsActive(std::stof(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}
}