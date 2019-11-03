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
		GLint m_viewLocation;
		GLint m_projLocation;

		std::shared_ptr<Rogue::CameraSystem> m_pCamera;

		bool m_isActive = true;
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

		void Receive(Rogue::Event* ev) override;
	};
//}