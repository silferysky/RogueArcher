/* Start Header ************************************************************************/
/*!
\file           AnimationComponent.h
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
		int m_startFrame = 0;
		int m_endFrame = 0;
		int m_offSet; // each frame in pixels
		float m_secondsPerFrame = 0.1f;
		float m_timer = 0.0f;
		bool m_isAnimating = true;
		bool m_isLooping = false;
		bool m_isNotReversed = true;
	public:
		AnimationComponent() = default;
		~AnimationComponent() = default;

		void setFrames(const int& frames);
		int getFrames() const;

		void setCurrentFrame(const int& frame);
		int getCurrentFrame() const;

		void setStartFrame(const int& frame);
		int getStartFrame() const;

		void setEndFrame(const int& frame);
		int getEndFrame() const;

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

		void setIsNotReversed(const bool& isNotReversed);
		bool getIsNotReversed() const;

		void DisplayOnInspector();
		//Serialize
		virtual std::string Serialize() override;
		virtual void Deserialize(std::string_view toDeserialize) override;
	};
}