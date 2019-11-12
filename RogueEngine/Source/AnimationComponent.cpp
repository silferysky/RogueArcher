#include "pch.h"
#include "AnimationComponent.h"
#include "Main.h"

namespace Rogue
{
	void AnimationComponent::setFrames(const int& frames)
	{
		m_frames = frames;
	}

	int AnimationComponent::getFrames() const
	{
		return m_frames;
	}

	void AnimationComponent::setCurrentFrame(const int& frame)
	{
		m_currentFrame = frame;
	}

	int AnimationComponent::getCurrentFrame() const
	{
		return m_currentFrame;
	}

	void AnimationComponent::setOffSet(const int& offSet)
	{
		m_offSet = offSet;
	}

	int AnimationComponent::getOffSet() const
	{
		return m_offSet;
	}

	void AnimationComponent::setSecondsPerFrame(const float& secondsPerFrame)
	{
		m_secondsPerFrame = secondsPerFrame;
	}

	float AnimationComponent::getSecondsPerFrame() const
	{
		return m_secondsPerFrame;
	}

	void AnimationComponent::updateTimer()
	{
		m_timer += g_deltaTime;
	}

	void AnimationComponent::setTimer(const float& timer)
	{
		m_timer = timer;
	}

	float AnimationComponent::getTimer() const
	{
		return m_timer;
	}

	void AnimationComponent::setIsAnimating(const bool& isAnimating)
	{
		m_isAnimating = isAnimating;
	}

	bool AnimationComponent::getIsAnimating() const
	{
		return m_isAnimating;
	}

	void AnimationComponent::setIsLooping(const bool& isLooping)
	{
		m_isLooping = isLooping;
	}

	bool AnimationComponent::getIsLooping() const
	{
		return m_isLooping;
	}

	void AnimationComponent::DisplayOnInspector()
	{
		ImVec2 imageSize{ ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 8 };
		//ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PushItemWidth(75);
		ImGui::DragInt("Frames", &m_frames, 1.0f, 0, 60);
		setFrames(m_frames);

		ImGui::DragFloat("Seconds Per Frame", &m_secondsPerFrame, 0.1f, 0.0f, 1.0f);
		setSecondsPerFrame(m_secondsPerFrame);

		ImGui::Checkbox("Looping?", &m_isLooping);
		setIsLooping(m_isLooping);
	}

	std::string AnimationComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_frames << ";";
		ss << m_secondsPerFrame << ";";
		ss << m_isLooping << ";";
		return ss.str();
	}

	void AnimationComponent::Deserialize(const std::string& toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;		//s2 is used if two are needed
		int counter = 0;

		while (std::getline(ss, s1, ';'))
		{
			switch (counter)
			{
			case 0:
				setFrames(std::stoi(s1));
				break;
			case 1:
				setSecondsPerFrame(std::stof(s1));
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