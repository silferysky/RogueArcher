#pragma once
#include "BaseComponent.h"
#include "GL/glew.h"
#include <string>

namespace Rogue
{
	class AnimationComponent
		: public BaseComponent
	{
		int m_frames;
		float m_secondsPerFrame = 0.1f;
		bool m_isAnimating = false;
		bool m_isLooping = false;
	public:
		AnimationComponent() = default;
		~AnimationComponent() = default;

		void setFrames(const int& frames);
		int getFrames() const;

		void setSecondsPerFrame(const float& secondsPerFrame);
		float getSecondsPerFrame() const;

		void setIsAnimating(const bool& isAnimating);
		bool getIsAnimating() const;

		void setIsLooping(const bool& isLooping);
		bool getIsLooping() const;

		//Serialize
		virtual std::string Serialize();
		virtual void Deserialize(std::string toDeserialize);
	};
}