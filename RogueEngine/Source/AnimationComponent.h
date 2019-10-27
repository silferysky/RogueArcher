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
		bool m_isAnimating = 0;
	public:
		AnimationComponent() = default;
		~AnimationComponent() = default;

		void setFrames(const int& frames);
		int getFrames() const;

		void setIsAnimating(const bool& isAnimating);
		bool getIsAnimating() const;

		//Serialize
		virtual std::string Serialize();
		virtual void Deserialize(std::string toDeserialize);
	};
}