#pragma once
#include "BaseComponent.h"
#include "GL/glew.h"
#include <string>

namespace Rogue
{
	class AnimationComponent
		: public BaseComponent
	{
		int m_frames = 0;
		int m_currentFrame = 0;
		int m_offSet; // each frame in pixels
		float m_secondsPerFrame = 0.1f;
		float m_timer = 0.0f;
		bool m_isAnimating = true;
		bool m_isLooping = false;
	public:
		AnimationComponent() = default;
		~AnimationComponent() = default;

		void setFrames(const int& frames);
		int getFrames() const;

		void setCurrentFrame(const int& frame);
		int getCurrentFrame() const;

		void setOffSet(const int& offSet);
		int getOffSet() const;

		void setSecondsPerFrame(const float& secondsPerFrame);
		float getSecondsPerFrame() const;

		void updateTimer();
		void setTimer(const float& timer);
		float getTimer() const;

		void setIsAnimating(const bool& isAnimating);
		bool getIsAnimating() const;

		void setIsLooping(const bool& isLooping);
		bool getIsLooping() const;

		void DisplayOnInspector();
		//Serialize
		virtual std::string Serialize() override;
		virtual void Deserialize(const std::string& toDeserialize) override;
	};
}