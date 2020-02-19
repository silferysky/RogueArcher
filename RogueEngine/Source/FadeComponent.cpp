#include "Precompiled.h"
#include "FadeComponent.h"
#include "Main.h"

namespace Rogue
{
	void FadeComponent::setFadeVelocity(const float& fadeVelocity)
	{
		m_fadeVelocity = fadeVelocity;
	}

	float FadeComponent::getFadeVelocity() const
	{
		return m_fadeVelocity;
	}

	void FadeComponent::setIsActive(const bool& isActive)
	{
		m_isActive = isActive;
	}

	bool FadeComponent::getIsActive() const
	{
		return m_isActive;
	}

	void FadeComponent::setIsFadingIn(const bool& isFadingIn)
	{
		m_isFadingIn = isFadingIn;
	}

	bool FadeComponent::getIsFadingIn() const
	{
		return m_isFadingIn;
	}

	void FadeComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		ImVec2 imageSize{ ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 8 };
		//ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PushItemWidth(75);

		ImGui::Checkbox("Is Fading?", &m_isActive);
		setIsActive(m_isActive);

		ImGui::Checkbox("Fading In?", &m_isFadingIn);
		setIsFadingIn(m_isFadingIn);

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Checked means Fading In, Unchecked means Fading Out");
			ImGui::EndTooltip();
		}

		ImGui::DragFloat("Fade Velocity", &m_fadeVelocity, 0.1f, 0.0f, 20.0f);
		setFadeVelocity(m_fadeVelocity);
	}

	std::string FadeComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_fadeVelocity << ";";
		ss << m_isActive << ";";
		return ss.str();
	}

	void FadeComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;		//s2 is used if two are needed
		int counter = 0;

		while (std::getline(ss, s1, ';'))
		{
			switch (counter)
			{
			case 0:
				setFadeVelocity(std::stof(s1));
				break;
			case 1:
				setIsActive(std::stof(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}
}