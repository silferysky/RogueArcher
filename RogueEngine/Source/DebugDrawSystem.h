/*Start Header*************************************************************************/
/*!
\file           DebugDrawSystem.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for DebugDrawSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

//namespace Rogue
//{
	class Rogue::Timer;
	class DebugDrawSystem : public Rogue::System, public Rogue::EventListener
	{
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;

		Rogue::Shader m_shader;

		// Uniform Buffer Object
		GLuint m_uboMatrices;

		GLint m_filterLocation;

		std::shared_ptr<Rogue::CameraSystem> m_pCamera;

		bool m_isActive = false;
	public:
		DebugDrawSystem();
		~DebugDrawSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void TrueUpdate();

		void drawAABB(Rogue::BoxCollider2DComponent* box, Rogue::TransformComponent* transform);
		void drawOBB(Rogue::BoxCollider2DComponent* box, Rogue::RigidbodyComponent* body);
		void drawCircle(Rogue::CircleCollider2DComponent* circle, Rogue::TransformComponent* transform);
		void drawVelocity(Rogue::RigidbodyComponent* rBody, Rogue::TransformComponent* transform);

		void Receive(Rogue::Event& ev) override;
	};
//}