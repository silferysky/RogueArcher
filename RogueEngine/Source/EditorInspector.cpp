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
					ImGui::TextDisabled("New Name");
					ImGui::SameLine();
					ImGui::InputText("                  ", buffer, 64);
					if (ImGui::Button("Edit"))
					{
						i.m_objectName = buffer;
					}
					if (g_engine.m_coordinator.ComponentExists<TransformComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Transform"))
						{
							Vec2 Scale = g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).getScale();
							Vec2 Position = g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).getPosition();
							float Rotation = g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).getRotation();

	
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

							if (ImGui::Button("Reset Position"))
							{
								g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).setPosition(Vec2{ 0,0 });
							}

							if (ImGui::Button("Reset Rotation"))
							{
								g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).setRotation(0.0f);
							}

							if (ImGui::Button("Reset Scale"))
							{
								g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).setScale(Vec2{ 100.0f,100.0f });
							}
						}
					}
					
					if (g_engine.m_coordinator.ComponentExists<SpriteComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Sprite"))
						{
							std::string m_spritePath = g_engine.m_coordinator.GetComponent<SpriteComponent>(i.m_Entity).getTexturePath();
							glm::vec4 m_color = g_engine.m_coordinator.GetComponent<SpriteComponent>(i.m_Entity).getFilter();
							int m_priority = g_engine.m_coordinator.GetComponent<SpriteComponent>(i.m_Entity).getDrawPriority();
							const std::string m_constSpritePath = "Resources/Assets/";
							static char m_newSpritePath[128];
							static char m_priorityDraw[128];
							
							ImGui::PushItemWidth(75);
							ImGui::TextWrapped("Current File Path");
							ImGui::TextWrapped("%s", m_spritePath.c_str());
							ImGui::TextWrapped("New Texture Path");
							ImGui::SameLine();
							ImGui::PushItemWidth(200);
							ImGui::InputText("                      ", m_newSpritePath, 128);

							if (ImGui::Button("Set New Sprite"))
							{
								m_spritePath = m_constSpritePath + m_newSpritePath;
								g_engine.m_coordinator.GetComponent<SpriteComponent>(i.m_Entity).setTexturePath(m_spritePath);
								g_engine.m_coordinator.GetComponent<SpriteComponent>(i.m_Entity).setTexture(m_spritePath.c_str());
							}

							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::Text("Enter name of file and click on Set New Path to update sprite. Note that new sprite will only be loaded when scene is saved and restarted");
								ImGui::EndTooltip();
							}

							ImGui::PushItemWidth(250);
							ImGui::ColorEdit4("Color", (float*)& m_color);
							g_engine.m_coordinator.GetComponent<SpriteComponent>(i.m_Entity).setFilter(m_color);


							ImGui::TextWrapped("Current Draw Priority : %d", m_priority);
							ImGui::TextWrapped("Set Draw Priority");
							ImGui::InputText("                       ", m_priorityDraw, 128);
							if (ImGui::Button("Set Priority"))
							{
								m_priority = atoi(m_priorityDraw);
								g_engine.m_coordinator.GetComponent<SpriteComponent>(i.m_Entity).setDrawPriority(m_priority);
							}
							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::Text("Higher number means the object will be drawn infront");
								ImGui::EndTooltip();
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<AnimationComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Animation"))
						{
							int m_frames = g_engine.m_coordinator.GetComponent<AnimationComponent>(i.m_Entity).getFrames();
							float m_secondsPerFrame = g_engine.m_coordinator.GetComponent<AnimationComponent>(i.m_Entity).getSecondsPerFrame();
							bool m_isAnimating = g_engine.m_coordinator.GetComponent<AnimationComponent>(i.m_Entity).getIsAnimating();
							bool m_looping = g_engine.m_coordinator.GetComponent<AnimationComponent>(i.m_Entity).getIsLooping();
							
							ImVec2 imageSize{ ImGui::GetWindowWidth()/2, ImGui::GetWindowHeight() / 8 };
							ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0));
							ImGui::PushItemWidth(75);
							ImGui::DragInt("Frames", &m_frames, 1.0f, 0, 60);
							g_engine.m_coordinator.GetComponent<AnimationComponent>(i.m_Entity).setFrames(m_frames);
				
							ImGui::DragFloat("Seconds Per Frame", &m_secondsPerFrame, 0.1f, 0.0f, 1.0f);
							g_engine.m_coordinator.GetComponent<AnimationComponent>(i.m_Entity).setSecondsPerFrame(m_secondsPerFrame);
				
							ImGui::Checkbox("Animating?", &m_isAnimating);
							g_engine.m_coordinator.GetComponent<AnimationComponent>(i.m_Entity).setIsAnimating(m_isAnimating);
				
							ImGui::Checkbox("Looping?", &m_looping);
							g_engine.m_coordinator.GetComponent<AnimationComponent>(i.m_Entity).setIsLooping(m_looping);
						}
					}

					if (g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Box2D Collider"))
						{
							Vec2 m_scale = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(i.m_Entity).m_aabb.getScaleOffSet();
							Vec2 m_center = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(i.m_Entity).m_aabb.getCenterOffSet();

							ImGui::PushItemWidth(75);
							ImGui::DragFloat("Scale X", &m_scale.x, 1.0f, -10000.0f, 10000.0f);
							ImGui::PushItemWidth(75);
							ImGui::DragFloat("Scale Y", &m_scale.y, 1.0f, -10000.0f, 10000.0f);
							g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(i.m_Entity).m_aabb.setScaleOffSet(m_scale);

							ImGui::PushItemWidth(75);
							ImGui::DragFloat("Center X", &m_center.x, 1.0f, -10000.0f, 10000.0f);
							ImGui::PushItemWidth(75);
							ImGui::DragFloat("Center Y", &m_center.y, 1.0f, -10000.0f, 10000.0f);
							g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(i.m_Entity).m_aabb.setCenterOffSet(m_center);
	
						}
					}

					if (g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("RigidBody"))
						{
							bool m_isStatic = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).getIsStatic();
							Vec2 m_velocity = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).getVelocity();
							Vec2 m_acceleration = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).getAcceleration();
							float m_damping = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).getDamping();
							float m_friction = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).getFriction();
							float m_restitution = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).getBounciness();
							//float m_mass = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).getInvMass();
							Vec2 m_force = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).getAccForce();

							ImGui::PushItemWidth(75);
							ImGui::Checkbox("Static?", &m_isStatic);
							g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).setIsStatic(m_isStatic);

							ImGui::PushItemWidth(75);
							ImGui::DragFloat("Velocity X", &m_velocity.x, 1.0f, -2000.0f, 2000.0f);
							ImGui::PushItemWidth(75);
							ImGui::DragFloat("Velocity Y", &m_velocity.y, 1.0f, -2000.0f, 2000.0f);
							g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).setVelocity(m_velocity);


							ImGui::PushItemWidth(75);
							ImGui::DragFloat("Acceleration X", &m_acceleration.x, 1.0f, -10000.0f, 10000.0f);
							ImGui::PushItemWidth(75);
							ImGui::DragFloat("Acceleration Y", &m_acceleration.y, 1.0f, -10000.0f, 10000.0f);
							g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).setAcceleration(m_acceleration);

							ImGui::PushItemWidth(75);
							ImGui::SliderFloat("Friction", &m_friction, 0.0f, 1.0f);
							g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).setFriction(m_friction);

							ImGui::PushItemWidth(75);
							ImGui::SliderFloat("Damping", &m_damping, 0.0f, 1.0f);
							g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).setDamping(m_damping);
							
							ImGui::PushItemWidth(75);
							ImGui::SliderFloat("Restitution", &m_restitution, 0.0f, 1.0f);
							g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i.m_Entity).setBounciness(m_restitution);
						}
					}

					if (g_engine.m_coordinator.ComponentExists<LogicComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Logic"))
						{
							const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
							static int m_ai = 0;
							ImGui::Combo("AI Type", &m_ai, items, IM_ARRAYSIZE(items));
							ImGui::Combo("Current State", &m_ai, items, IM_ARRAYSIZE(items));
							ImGui::Combo("Active State", &m_ai, items, IM_ARRAYSIZE(items));
						}

					}

					if (g_engine.m_coordinator.ComponentExists<CameraComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Camera"))
						{
							bool m_isMain = g_engine.m_coordinator.GetComponent<CameraComponent>(i.m_Entity).getIsActive();

							ImGui::PushItemWidth(75);
							ImGui::Checkbox("Active?", &m_isMain);
							ImGui::TextWrapped("There can only be 1 active non-world camera at a time, set others to non-active if you want this to be the main camera.");
							g_engine.m_coordinator.GetComponent<CameraComponent>(i.m_Entity).setIsActive(m_isMain);
							ImGui::PushItemWidth(75);
						}
					}

					if (g_engine.m_coordinator.ComponentExists<UIComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("UI"))
						{
							bool m_isActive = g_engine.m_coordinator.GetComponent<UIComponent>(i.m_Entity).getIsActive();

							ImGui::PushItemWidth(75);
							ImGui::Checkbox("Active?", &m_isActive);
							ImGui::TextWrapped("Check this box to show the UI element.");
							g_engine.m_coordinator.GetComponent<CameraComponent>(i.m_Entity).setIsActive(m_isActive);
							ImGui::PushItemWidth(75);
						}
					}

					if (g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Circle 2D Collider"))
						{
							float m_radius = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(i.m_Entity).m_collider.getRadius();
							Vec2 m_centerOffset = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(i.m_Entity).m_collider.getCenterOffSet();
							float m_rotationOffset = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(i.m_Entity).m_collider.getRotationOffSet();
							Vec2 m_scaleOffset = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(i.m_Entity).m_collider.getScaleOffSet();

							ImGui::PushItemWidth(75);
							ImGui::DragFloat("Radius", &m_radius, 0.5f, -100000.0f, 100000.0f);
							g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(i.m_Entity).m_collider.setRadius(m_radius);

							ImGui::DragFloat("Center Offset X ", &m_centerOffset.x, 0.5f, -100000.0f, 100000.0f);
							ImGui::DragFloat("Center Offset Y ", &m_centerOffset.y, 0.5f, -100000.0f, 100000.0f);
							g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(i.m_Entity).m_collider.setCenterOffSet(m_centerOffset);

							ImGui::DragFloat("Rotation Offset ", &m_rotationOffset, 0.5f, -100000.0f, 100000.0f);
							g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(i.m_Entity).m_collider.setRotationOffSet(m_rotationOffset);

							ImGui::DragFloat("Scale Offset X ", &m_scaleOffset.x, 0.5f, -100000.0f, 100000.0f);
							ImGui::DragFloat("Scale Offset Y ", &m_scaleOffset.y, 0.5f, -100000.0f, 100000.0f);
							g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(i.m_Entity).m_collider.setScaleOffSet(m_scaleOffset);

						}
					}

					if (g_engine.m_coordinator.ComponentExists<AudioEmitterComponent>(i.m_Entity))
					{
						if (ImGui::CollapsingHeader("Sound"))
						{
							std::string m_audioPath = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(i.m_Entity).getSoundPath();
							static char m_newaudioPath[128];
							const std::string m_constAudioPath = "Resources/Sounds/";
							ImGui::PushItemWidth(75);
							ImGui::TextWrapped("Current Sound Path : ");
							ImGui::TextWrapped("%s", m_audioPath.c_str());
							ImGui::TextDisabled("New Sound Path");
							ImGui::SameLine();
							ImGui::PushItemWidth(250);
							ImGui::InputText("                    ", m_newaudioPath, 128);
							if (ImGui::Button("Set new path"))
							{
								m_audioPath = m_constAudioPath + m_newaudioPath;
								g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(i.m_Entity).setSoundPath(m_audioPath);
							}
							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::Text("Enter name of file and click on Set New Path to update sound. Note that new sound will only be loaded when scene is saved and restarted");
								ImGui::EndTooltip();
							}
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
								Sprite.Deserialize("Resources/Assets/DefaultSprite.png");
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
		glm::vec3 m_cameraPos = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetCameraPos();
		ImGui::DragFloat("Camera X", &m_cameraPos.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Y", &m_cameraPos.y, 1.0f, -10000.0f, 10000.0f);

		Vec2 m_cameraMin = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetCameraMin();
		Vec2 m_cameraMax = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetCameraMax();

		ImGui::DragFloat("Camera Min X", &m_cameraMin.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Min Y", &m_cameraMin.y, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Max X", &m_cameraMax.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Max Y", &m_cameraMax.y, 1.0f, -10000.0f, 10000.0f);

		if (ImGui::Button("Reset Camera"))
		{
			m_cameraPos.x = 0.0f;
			m_cameraPos.y = 0.0f;
		}

		g_engine.m_coordinator.GetSystem<CameraSystem>()->SetCameraPos(m_cameraPos);

		g_engine.m_coordinator.GetSystem<CameraSystem>()->SetCameraMin(m_cameraMin);
		g_engine.m_coordinator.GetSystem<CameraSystem>()->SetCameraMax(m_cameraMax);

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

