#include "EditorInspector.h"

namespace Rogue
{
	ImGuiInspector::ImGuiInspector()
	{
	}

	ImGuiInspector::~ImGuiInspector()
	{
	}

	void ImGuiInspector::Init()
	{
		
	}
	void ImGuiInspector::Update()
	{
		ImGui::Begin("Inspector");
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::TextDisabled("Name");
			for (auto& i : activeObjects)
			{
				if (i.m_selected == true)
				{
					if (g_engine.m_coordinator.CheckIfComponentExists<TransformComponent>(i.m_Entity))
					{
						Vec2 Scale = g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).getScale();
						Vec2 Position = g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).getPosition();
						float Rotation = g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).getRotation();

						ImGui::Text("%s", i.m_objectName.c_str());
						ImGui::Text("Scale    ");
						ImGui::SameLine();
						ImGui::PushItemWidth(75);
						ImGui::DragFloat(" ", &Scale.x, 1.0f, 0.0f, 100000.0f);
						ImGui::SameLine(0.0f, 36.0f);
						ImGui::DragFloat("  ", &Scale.y, 1.0f, 0.0f, 100000.0f);
						g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).setScale(Scale);

						ImGui::Text("Rotation ");
						ImGui::SameLine();
						ImGui::DragFloat("   ", &Rotation, 0.1f, 0.0f, 6.28f);
						g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).setRotation(Rotation);

						ImGui::Text("Translate");
						ImGui::SameLine();
						ImGui::PushItemWidth(75);
						ImGui::DragFloat("     ", &Position.x);
						ImGui::SameLine();
						ImGui::PushItemWidth(75);
						ImGui::DragFloat("      ", &Position.y);
						g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).setPosition(Position);
						ImGui::PushItemWidth(50);
					}
					

					if (ImGui::Button("Add Component"))
					{
						ImGui::OpenPopup("Add Component");
					}

					if (ImGui::Button("Delete Component"))
					{
						ImGui::OpenPopup("Delete Component");
					}

					if (ImGui::BeginPopup("Add Component"))
					{
						if (ImGui::BeginMenu("Component"))
						{
							if (ImGui::MenuItem("Sprite Component", nullptr, false, !g_engine.m_coordinator.CheckIfComponentExists<SpriteComponent>(i.m_Entity)))
							{
								auto& Sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(i.m_Entity);
								Sprite.Deserialize("Resources/Assets/DefaultSprite.png");
							}
							
							if (ImGui::MenuItem("Transform Component",nullptr,false, !g_engine.m_coordinator.CheckIfComponentExists<TransformComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent<TransformComponent>(
									i.m_Entity,
									TransformComponent(Vec2{ 0.0f, 0.0f }, Vec2{ 100.0f, 100.0f }, 0.0f));
							}

							if (ImGui::MenuItem("Player Controller Component", nullptr, false, !g_engine.m_coordinator.CheckIfComponentExists<PlayerControllerComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent(i.m_Entity, PlayerControllerComponent());
							}

							if (ImGui::MenuItem("Logic Component", nullptr, false, !g_engine.m_coordinator.CheckIfComponentExists<LogicComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent(i.m_Entity, LogicComponent());
							}
							ImGui::EndMenu();
						}
						if (ImGui::BeginMenu("Colliders"))
						{
							if (ImGui::MenuItem("Circle Collider", nullptr, false, !g_engine.m_coordinator.CheckIfComponentExists<CircleCollider2DComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent(i.m_Entity, CircleCollider2DComponent());
							}

							if (ImGui::MenuItem("Box Collider 2D", nullptr, false, !g_engine.m_coordinator.CheckIfComponentExists<BoxCollider2DComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent(i.m_Entity, BoxCollider2DComponent());
							}

							ImGui::EndMenu();
						}

						if (ImGui::MenuItem("RigidBody", nullptr, false, !g_engine.m_coordinator.CheckIfComponentExists<RigidbodyComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.AddComponent(i.m_Entity, RigidbodyComponent());
						}

						if (ImGui::MenuItem("Camera", nullptr, false, !g_engine.m_coordinator.CheckIfComponentExists<CameraComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.AddComponent(i.m_Entity, CameraComponent());
						}
						ImGui::EndPopup();
					}
					if (ImGui::BeginPopup("Delete Component"))
					{
						if (ImGui::BeginMenu("Component"))
						{
							if (ImGui::MenuItem("Sprite Component", nullptr, false, g_engine.m_coordinator.CheckIfComponentExists<SpriteComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<SpriteComponent>(i.m_Entity);
							}

							if (ImGui::MenuItem("Transform Component", nullptr, false, g_engine.m_coordinator.CheckIfComponentExists<TransformComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<TransformComponent>(i.m_Entity);
							}

							if (ImGui::MenuItem("Player Controller Component", nullptr, false, g_engine.m_coordinator.CheckIfComponentExists<PlayerControllerComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<PlayerControllerComponent>(i.m_Entity);
							}

							if (ImGui::MenuItem("Logic Component", nullptr, false, g_engine.m_coordinator.CheckIfComponentExists<LogicComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<LogicComponent>(i.m_Entity);
							}
							ImGui::EndMenu();
						}
						if (ImGui::BeginMenu("Colliders"))
						{
							if (ImGui::MenuItem("Circle Collider", nullptr, false, g_engine.m_coordinator.CheckIfComponentExists<CircleCollider2DComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<CircleCollider2DComponent>(i.m_Entity);
							}

							if (ImGui::MenuItem("Box Collider 2D", nullptr, false, g_engine.m_coordinator.CheckIfComponentExists<BoxCollider2DComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<BoxCollider2DComponent>(i.m_Entity);
							}

							ImGui::EndMenu();
						}

						if (ImGui::MenuItem("RigidBody", nullptr, false, g_engine.m_coordinator.CheckIfComponentExists<RigidbodyComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.RemoveComponent<RigidbodyComponent>(i.m_Entity);
						}

						if (ImGui::MenuItem("Camera"))
						{
							g_engine.m_coordinator.RemoveComponent<CameraComponent>(i.m_Entity);
						}
						ImGui::EndPopup();
					}
				}
			}			

			
		}

		ImGui::End();
	}
	void ImGuiInspector::Shutdown()
	{
	}
};

