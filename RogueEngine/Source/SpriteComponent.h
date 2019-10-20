#pragma once
#include "BaseComponent.h"
#include "GL/glew.h"

namespace Rogue
{
	class SpriteComponent
		: public BaseComponent
	{
		GLuint m_texture;
	public:
		SpriteComponent();
		~SpriteComponent() = default;

		void setTexture(const char* texture);
		GLuint getTexture() const;

		//Serialize
		virtual std::string Serialize();
		virtual void Deserialize(std::string toDeserialize);

		//void operator=(SpriteComponent sprite);
	};
}