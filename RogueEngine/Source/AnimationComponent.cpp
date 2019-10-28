#include <sstream>
#include "AnimationComponent.h"

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

	void AnimationComponent::setSecondsPerFrame(const float& secondsPerFrame)
	{
		m_secondsPerFrame = secondsPerFrame;
	}

	float AnimationComponent::getSecondsPerFrame() const
	{
		return m_secondsPerFrame;
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

	std::string AnimationComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_secondsPerFrame << ";";
		ss << m_frames << ";";
		ss << m_isAnimating << ";";
		ss << m_isLooping << ";";
		return ss.str();
	}

	void AnimationComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1;		//s2 is used if two are needed
		int counter = 0;

		while (std::getline(ss, s1, ';'))
		{
			switch (counter)
			{
			case 0:
				setFrames(std::stof(s1));
				break;
			case 1:
				setSecondsPerFrame(std::stof(s1));
				break;
			case 2:
				setIsAnimating(std::stof(s1));
				break;
			case 3:
				setIsLooping(std::stof(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}
}