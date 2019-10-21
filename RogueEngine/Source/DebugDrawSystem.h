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
	public:
		DebugDrawSystem();
		~DebugDrawSystem() = default;

		void init();
		void update() override;

		void drawAABB(Rogue::BoxCollider2DComponent* box, Rogue::TransformComponent* transform);
		void drawOBB(Rogue::BoxCollider2DComponent* box);
		void drawVelocity(Rogue::RigidbodyComponent* rBody, Rogue::TransformComponent* transform);

		void receive(Rogue::Event* ev);
	};
//}