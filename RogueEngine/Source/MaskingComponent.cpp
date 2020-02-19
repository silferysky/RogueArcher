#include "Precompiled.h"
#include "MaskingComponent.h"
#include "Main.h"

namespace Rogue
{
	void MaskingComponent::setFrameSize(const int& frameSize)
	{
		m_frameSize = frameSize;
	}

	int MaskingComponent::getFrameSize() const
	{
		return m_frameSize;
	}

	void MaskingComponent::setTimer(const float& timer)
	{
		m_timer = timer;
	}

	float MaskingComponent::getTimer() const
	{
		return m_timer;
	}

	void MaskingComponent::setMaskVelocity(const float& maskVelocity)
	{
		m_maskVelocity = maskVelocity;
	}

	float MaskingComponent::getMaskVelocity() const
	{
		return m_maskVelocity;
	}

	void MaskingComponent::setIsLooping(const bool& isLooping)
	{
		m_isLooping = isLooping;
	}

	bool MaskingComponent::getIsLooping() const
	{
		return m_isLooping;
	}

	void MaskingComponent::setIsActive(const bool& isActive)
	{
		m_isActive = isActive;
	}

	bool MaskingComponent::getIsActive() const
	{
		return m_isActive;
	}

	void MaskingComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		ImVec2 imageSize{ ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 8 };
		//ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PushItemWidth(75);
		
		ImGui::DragInt("Mask Size X", &m_frameSize, 1, 0, 3000);
		setFrameSize(m_frameSize);

		ImGui::DragFloat("Mask Velocity", &m_maskVelocity, 0.1f, 0.0f, 20.0f);
		setMaskVelocity(m_maskVelocity);

		ImGui::Checkbox("Active?", &m_isActive);
		setIsActive(m_isActive);

		ImGui::Checkbox("Looping?", &m_isLooping);
		setIsLooping(m_isLooping);
	}

	std::string MaskingComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_frameSize << ";";
		ss << m_maskVelocity << ";";
		ss << m_isLooping << ";";
		return ss.str();
	}

	void MaskingComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;		//s2 is used if two are needed
		int counter = 0;

		while (std::getline(ss, s1, ';'))
		{
			switch (counter)
			{
			case 0:
				setFrameSize(std::stoi(s1));
				break;
			case 1:
				setMaskVelocity(std::stof(s1));
				break;
			case 2:
				setIsLooping(std::stof(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}
}