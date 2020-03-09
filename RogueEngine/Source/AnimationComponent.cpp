/* Start Header ************************************************************************/
/*!
\file           AnimationComponent.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for AnimationComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
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

	void AnimationComponent::setStartFrame(const int& frame)
	{
		m_startFrame = frame;
	}

	int AnimationComponent::getStartFrame() const
	{
		return m_startFrame;
	}

	void AnimationComponent::setEndFrame(const int& frame)
	{
		m_endFrame = frame;
	}

	int AnimationComponent::getEndFrame() const
	{
		return m_endFrame;
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
		m_timer += g_fixedDeltaTime * g_engine.GetTimeScale();
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

	void AnimationComponent::setIsNotReversed(const bool& isNotReversed)
	{
		m_isNotReversed = isNotReversed;
	}

	bool AnimationComponent::getIsNotReversed() const
	{
		return m_isNotReversed;
	}

	void AnimationComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		ImVec2 imageSize{ ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 8 };
		//ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PushItemWidth(75);
		ImGui::DragInt("Frames", &m_frames, 1.0f, 0, 60);
		setFrames(m_frames);

		ImGui::DragInt("Starting Frame", &m_startFrame, 1.0f, 0, 60);
		setStartFrame(m_startFrame);

		ImGui::DragInt("Final Frame", &m_endFrame, 1.0f, 0, 60);
		setEndFrame(m_endFrame);

		ImGui::DragFloat("Seconds Per Frame", &m_secondsPerFrame, 0.1f, 0.0f, 1.0f);
		setSecondsPerFrame(m_secondsPerFrame);

		ImGui::Checkbox("Looping?", &m_isLooping);
		setIsLooping(m_isLooping);

		ImGui::Checkbox("Not Reversed?", &m_isNotReversed);
		setIsNotReversed(m_isNotReversed);
	}

	std::string AnimationComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_frames << ";";
		ss << m_endFrame << ";";
		ss << m_secondsPerFrame << ";";
		ss << m_isLooping << ";";
		ss << m_startFrame << ";";
		ss << m_isNotReversed << ";";
		return ss.str();
	}

	void AnimationComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
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
				setEndFrame(std::stoi(s1));
				break;
			case 2:
				setSecondsPerFrame(std::stof(s1));
				break;
			case 3:
				setIsLooping(std::stof(s1));
				break;
			case 4:
				setStartFrame(std::stoi(s1));
				break;
			case 5:
				setIsNotReversed(std::stoi(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}
}