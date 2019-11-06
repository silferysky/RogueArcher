#include "EditorInspector.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "glew.h"
#include "REEngine.h"
#include "Main.h"

namespace Rogue
{
	ImGuiInspector::ImGuiInspector() :
		activeObjects{ g_engine.m_coordinator.GetEntityManager().m_getActiveObjects() }, m_color{ 0.0f,0.0f,0.0f,0.0f }
	{}

	void ImGuiInspector::Init()
	{
		
	}
	void ImGuiInspector::Update()
	{
		ImGui::Begin("Inspector");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Will not show anything until Game Object is double clicked");
			ImGui::EndTooltip();
		}
			ImGui::TextDisabled("Name");
			for (auto& i : activeObjects)
			{
				if (i.m_selected == true)
				{
					ImGui::TextWrapped("%s", i.m_objectName.c_str());
					static char buffer[64] = "";
					static char tagging[64] = "";

					ImGui::TextDisabled("New Name");
					ImGui::SameLine();
					ImGui::PushItemWidth(75);
					ImGui::InputText("                  ", buffer, 32);
					if (ImGui::Button("Edit"))
					{
						if (!buffer)
						{
							return;
						}
						i.m_objectName = buffer;
						memset(buffer, 0, 64);
					}
					
					ImGui::TextDisabled("Current Tag");
					ImGui::SameLine();
					ImGui::TextDisabled("%s",i.m_tag.c_str());
					ImGui::TextDisabled("New Tag Name");
					ImGui::SameLine();
					ImGui::InputText("               ", tagging, 64);
					if (ImGui::Button("Add New Tag"))
					{
						i.m_tag = tagging;
						memset(tagging, 0, 64);
					}
					if (ImGui::Button("Save As Prefab"))
					{
						auto archetypeMap = g_engine.m_coordinator.GetSceneManager().GetArchetypeMap();
						auto it = archetypeMap.find(i.m_objectName);
						if (it != archetypeMap.end())
							g_engine.m_coordinator.GetSceneManager().SaveArchetype(i.m_objectName.c_str());
						else
						{
							g_engine.m_coordinator.GetSceneManager().AddToArchetypes(i.m_Entity);
							g_engine.m_coordinator.GetSceneManager().SaveArchetype(i.m_objectName.c_str());
						}
					}

					if (g_engine.m_coordinator.ComponentExists<TransformComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Transform"))
						{
							auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity);
							trans.DisplayOnInspector();
						}
					}
					
					if (g_engine.m_coordinator.ComponentExists<SpriteComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Sprite"))
						{
							auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(i.m_Entity);
							sprite.DisplayOnInspector();							
						}
					}

					if (g_engine.m_coordinator.ComponentExists<AnimationComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Animation"))
						{
							auto& animation = g_engine.m_coordinator.GetComponent<AnimationComponent>(i.m_Entity);
							animation.DisplayOnInspector();
						}
					}

					if (g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Circle 2D Collider"))
						{
							auto& circle2D = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(i.m_Entity);
							circle2D.DisplayOnInspector();
						}
					}

					if (g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Box2D Collider"))
						{
							auto& box2D = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(i.m_Entity);
							box2D.DisplayOnInspector();	
						}
					}

					if (g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("RigidBody"))
						{
							auto& RigidBody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity);
							RigidBody.DisplayOnInspector();
						}
					}

					if (g_engine.m_coordinator.ComponentExists<LogicComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Logic"))
						{
							auto& Logic = g_engine.m_coordinator.GetComponent<LogicComponent>(i.m_Entity);
							Logic.DisplayOnInspector();
						}
					}

					if (g_engine.m_coordinator.ComponentExists<CameraComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Camera"))
						{
							auto& Camera = g_engine.m_coordinator.GetComponent<CameraComponent>(i.m_Entity);
							Camera.DisplayOnInspector();
						}
					}

					if (g_engine.m_coordinator.ComponentExists<UIComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("UI"))
						{
							auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(i.m_Entity);
							bool isActive = UI.getIsActive();
							ImGui::TextWrapped("Check this box to show the UI element.");
							if (g_engine.m_coordinator.ComponentExists<CameraComponent>(i.m_Entity))
								g_engine.m_coordinator.GetComponent<CameraComponent>(i.m_Entity).setIsActive(isActive);
						}
					}

					if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Player Controllable"))
						{
							auto& PCC = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(i.m_Entity);
							PCC.DisplayOnInspector();
						}					
					}

					if (g_engine.m_coordinator.ComponentExists<AudioEmitterComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Sound"))
						{
							auto& audio = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(i.m_Entity);
							audio.DisplayOnInspector();
						}
					}

					if (g_engine.m_coordinator.ComponentExists<CursorComponent>(i.m_Entity))
					{
						// If we need to edit anything about the cursor e.g offset from the actual cursor
					}

					if (g_engine.m_coordinator.ComponentExists<TextComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Text"))
						{
							auto& text = g_engine.m_coordinator.GetComponent<TextComponent>(i.m_Entity);
	
						}
					}

					if (ImGui::Button("Add Component"))
					{
						ImGui::OpenPopup("Add Component");
					}
					ImGui::SameLine();
					if (ImGui::Button("Delete Component"))
					{
						ImGui::OpenPopup("Delete Component");
					}

					if (ImGui::BeginPopup("Add Component"))
					{
						if (ImGui::BeginMenu("Component"))
						{
							if (ImGui::MenuItem("Sprite Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<SpriteComponent>(i.m_Entity)))
							{
								auto& Sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(i.m_Entity);
								Sprite.Deserialize("Resources/Assets/DefaultSprite.png;1");
							}

							if (ImGui::MenuItem("Animation Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<AnimationComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent(i.m_Entity,AnimationComponent());
							}

							if (ImGui::MenuItem("Transform Component",nullptr,false, !g_engine.m_coordinator.ComponentExists<TransformComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent<TransformComponent>(i.m_Entity,TransformComponent(Vec2{ 0.0f, 0.0f }, Vec2{ 100.0f, 100.0f }, 0.0f));
							}

							if (ImGui::MenuItem("Player Controller Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent(i.m_Entity, PlayerControllerComponent());
							}

							if (ImGui::MenuItem("Logic Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<LogicComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent(i.m_Entity, LogicComponent());
							}
							
							if (ImGui::MenuItem("Cursor Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<CursorComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent(i.m_Entity, CursorComponent());
							}

							ImGui::EndMenu();
						}
						if (ImGui::BeginMenu("Colliders"))
						{
							if (ImGui::MenuItem("Circle Collider", nullptr, false, !g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent(i.m_Entity, CircleCollider2DComponent());
							}

							if (ImGui::MenuItem("Box Collider 2D", nullptr, false, !g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.AddComponent(i.m_Entity, BoxCollider2DComponent());
							}

							ImGui::EndMenu();
						}

						if (ImGui::MenuItem("RigidBody", nullptr, false, !g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.AddComponent(i.m_Entity, RigidbodyComponent());
						}

						if (ImGui::MenuItem("Camera", nullptr, false, !g_engine.m_coordinator.ComponentExists<CameraComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.AddComponent(i.m_Entity, CameraComponent());
						}

						if (ImGui::MenuItem("Sound", nullptr, false, !g_engine.m_coordinator.ComponentExists<AudioEmitterComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.AddComponent(i.m_Entity, AudioEmitterComponent());
						}

						if (ImGui::MenuItem("UI", nullptr, false, !g_engine.m_coordinator.ComponentExists<UIComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.AddComponent(i.m_Entity, UIComponent());
						}

						if (ImGui::MenuItem("Text", nullptr, false, !g_engine.m_coordinator.ComponentExists<TextComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.AddComponent(i.m_Entity, TextComponent());
						}
						ImGui::EndPopup();
					}
					if (ImGui::BeginPopup("Delete Component"))
					{
						if (ImGui::BeginMenu("Component"))
						{
							if (ImGui::MenuItem("Sprite Component", nullptr, false, g_engine.m_coordinator.ComponentExists<SpriteComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<SpriteComponent>(i.m_Entity);
							}

							if (ImGui::MenuItem("Animation Component", nullptr, false, g_engine.m_coordinator.ComponentExists<AnimationComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<AnimationComponent>(i.m_Entity);
							}

							if (ImGui::MenuItem("Transform Component", nullptr, false, g_engine.m_coordinator.ComponentExists<TransformComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<TransformComponent>(i.m_Entity);
							}

							if (ImGui::MenuItem("Player Controller Component", nullptr, false, g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<PlayerControllerComponent>(i.m_Entity);
							}

							if (ImGui::MenuItem("Logic Component", nullptr, false, g_engine.m_coordinator.ComponentExists<LogicComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<LogicComponent>(i.m_Entity);
							}

							if (ImGui::MenuItem("Cursor Component", nullptr, false, g_engine.m_coordinator.ComponentExists<CursorComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<LogicComponent>(i.m_Entity);
							}

							ImGui::EndMenu();
						}
						if (ImGui::BeginMenu("Colliders"))
						{
							if (ImGui::MenuItem("Circle Collider", nullptr, false, g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<CircleCollider2DComponent>(i.m_Entity);
							}

							if (ImGui::MenuItem("Box Collider 2D", nullptr, false, g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(i.m_Entity)))
							{
								g_engine.m_coordinator.RemoveComponent<BoxCollider2DComponent>(i.m_Entity);
							}

							ImGui::EndMenu();
						}

						if (ImGui::MenuItem("RigidBody", nullptr, false, g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.RemoveComponent<RigidbodyComponent>(i.m_Entity);
						}

						if (ImGui::MenuItem("Camera", nullptr, false, g_engine.m_coordinator.ComponentExists<CameraComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.RemoveComponent<CameraComponent>(i.m_Entity);
						}

						if (ImGui::MenuItem("Sound", nullptr, false, g_engine.m_coordinator.ComponentExists<AudioEmitterComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(i.m_Entity).Destroy();
							g_engine.m_coordinator.RemoveComponent<AudioEmitterComponent>(i.m_Entity);
						}

						if (ImGui::MenuItem("UI", nullptr, false, g_engine.m_coordinator.ComponentExists<UIComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.RemoveComponent<UIComponent>(i.m_Entity);
						}

						if (ImGui::MenuItem("Text", nullptr, false, g_engine.m_coordinator.ComponentExists<TextComponent>(i.m_Entity)))
						{
							g_engine.m_coordinator.RemoveComponent<TextComponent>(i.m_Entity);
						}

						ImGui::EndPopup();
					}

					if (ImGui::Button("Clone Object"))
					{
						g_engine.m_coordinator.clone(i.m_Entity);
					}
					ImGui::SameLine();
					if (ImGui::Button("Delete Object"))
					{
						if (g_engine.m_coordinator.ComponentExists<AudioEmitterComponent>(i.m_Entity))
							g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(i.m_Entity).Destroy();

						g_engine.m_coordinator.GetSceneManager().DeleteActiveEntity(i.m_Entity);
					}

				}		
		}

		bool m_worldCamera = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetWorldCamera();
		ImGui::Checkbox("Toggle World Camera?", &m_worldCamera);
		g_engine.m_coordinator.GetSystem<CameraSystem>()->SetWorldCamera(m_worldCamera);

		float m_cameraZoom = g_engine.GetCameraZoom();
		glm::vec3 m_cameraPos = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetCameraPos();

		if (m_worldCamera)
		{
			m_cameraPos.x = 0.0f;
			m_cameraPos.y = 0.0f;
			m_cameraZoom = 1.630f;
		}

		ImGui::DragFloat("Camera X", &m_cameraPos.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Y", &m_cameraPos.y, 1.0f, -10000.0f, 10000.0f);

		Vec2 m_cameraMin = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetCameraMin();
		Vec2 m_cameraMax = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetCameraMax();

		ImGui::DragFloat("Camera Min X", &m_cameraMin.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Min Y", &m_cameraMin.y, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Max X", &m_cameraMax.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Max Y", &m_cameraMax.y, 1.0f, -10000.0f, 10000.0f);

		ImGui::DragFloat("Camera Zoom", &m_cameraZoom, 0.01f, 0.0f, 10.0f);

		if (ImGui::Button("Reset Camera"))
		{
			m_cameraPos.x = 0.0f;
			m_cameraPos.y = 0.0f;
			m_cameraZoom = 1.630f;
		}

		g_engine.m_coordinator.GetSystem<CameraSystem>()->SetCameraPos(m_cameraPos);

		g_engine.m_coordinator.GetSystem<CameraSystem>()->SetCameraMin(m_cameraMin);
		g_engine.m_coordinator.GetSystem<CameraSystem>()->SetCameraMax(m_cameraMax);

		g_engine.SetCameraZoom(m_cameraZoom);

		bool m_toggleGravity = g_engine.m_coordinator.GetSystem<PhysicsSystem>()->getToggleGravity();
		Vec2 m_gravity = g_engine.m_coordinator.GetSystem<PhysicsSystem>()->getGravity();

		ImGui::Checkbox("Gravity?", &m_toggleGravity);
		g_engine.m_coordinator.GetSystem<PhysicsSystem>()->setToggleGravity(m_toggleGravity);

		ImGui::DragFloat("Set Gravity", &m_gravity.y, 1.0f, -10000.0f, 10000.0f);
		g_engine.m_coordinator.GetSystem<PhysicsSystem>()->setGravity(m_gravity);


		ImGui::End();


	}
	void ImGuiInspector::Shutdown()
	{
	}
};

