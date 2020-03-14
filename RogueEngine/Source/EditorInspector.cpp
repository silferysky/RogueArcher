/* Start Header ************************************************************************/
/*!
\file           EditorInspector.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (70%)
				Lim Jia Hao, l.jiahao, 390006518 (10%)
				Chan Wai Kit Terence, c.terence, 440005918 (10%)
				Javier Foo, javier.foo, 440002318 (10%)
\par            kaiyi.loh\@digipen.edu
				l.jiahao\@digipen.edu
				c.terence\@digipen.edu
				javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorInspector

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "EditorInspector.h"
#include "REEngine.h"
#include "CameraManager.h"
#include "ParentEvent.h"
#include "EventDispatcher.h"
#include "CollisionManager.h"
#include "ChildComponent.h"
#include "EditorTileSet.h"
namespace Rogue
{
	ImGuiInspector::ImGuiInspector() :
		activeObjects{ g_engine.m_coordinator.GetActiveObjects() }, m_color{ 0.0f,0.0f,0.0f,0.0f }
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
				HierarchyInfo& infoObj = g_engine.m_coordinator.GetHierarchyInfo(i);
				if (infoObj.m_selected == true)
				{
					ImGui::TextWrapped("%s", infoObj.m_objectName.c_str());
					static char buffer[64] = "";
					static char tagging[64] = "";

					ImGui::TextDisabled("New Name");
					ImGui::SameLine();
					ImGui::PushItemWidth(75);
					if(ImGui::InputText("##HierachyInfo", buffer, 32, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						if (!buffer)
						{
							return;
						}
						infoObj.m_objectName = buffer;
						memset(buffer, 0, 64);
					}
					if (ImGui::Button("Apply") )
					{
						if (!buffer)
						{
							return;
						}	
						infoObj.m_objectName = buffer;
						memset(buffer, 0, 64);
					}
					
					ImGui::TextDisabled("Current Tag");
					ImGui::SameLine();
					ImGui::TextDisabled("%s",infoObj.m_tag.c_str());
					ImGui::TextDisabled("New Tag Name");
					ImGui::SameLine();
					if (ImGui::InputText("##Tagging", tagging, 64, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						infoObj.m_tag = tagging;
						memset(tagging, 0, 64);
					}
					if (ImGui::Button("Add New Tag"))
					{
						infoObj.m_tag = tagging;
						memset(tagging, 0, 64);
					}

					if (infoObj.m_archetypeName == "")
					{
						if (ImGui::Button("Save As Prefab"))
						{
							SceneManager::instance().SaveArchetype(infoObj.m_objectName.c_str(), infoObj.m_Entity);
							//auto archetypeMap = SceneManager::instance().GetArchetypeMap();
							//auto it = archetypeMap.find(infoObj.m_objectName);
							//if (it != archetypeMap.end())
							//	SceneManager::instance().SaveArchetype(infoObj.m_objectName.c_str(), infoObj.m_Entity);
							//else
							//{
							//	SceneManager::instance().AddToArchetypes(i);
							//	//SceneManager::instance().SaveArchetype(infoObj.m_objectName.c_str());
							//}
						}
					}
					else
					{
						if (ImGui::Button("Update Prefab"))
						{
							auto archetypeMap = SceneManager::instance().GetArchetypeMap();
							auto it = archetypeMap.find(infoObj.m_objectName);
							if (it != archetypeMap.end())
							{
								SceneManager::instance().UpdateArchetype(infoObj.m_archetypeName.c_str(), i);
							}
							//Only updates, not add new prefab
							//else
							//{
							//	SceneManager::instance().AddToArchetypes(i);
							//	//SceneManager::instance().SaveArchetype(infoObj.m_objectName.c_str());
							//}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<TransformComponent>(i))
					{
						if (ImGui::CollapsingHeader("Transform"))
						{
							auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(i);
							
							if (infoObj.m_parent == -1 || infoObj.m_parent == MAX_ENTITIES)
							{
								trans.DisplayOnInspector();
								if (trans.GetIsModified())
								{
									ParentTransformEvent setParentEv(i, MAX_ENTITIES);
									setParentEv.SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
									EventDispatcher::instance().AddEvent(setParentEv);
									trans.setModified(false);
								}
							}
							else
							{
								if (auto childTrans = g_engine.m_coordinator.TryGetComponent<ChildComponent>(i))
									childTrans->get().DisplayOnInspector();
							}
							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<TransformComponent>(i);
							}
						}
					}
					
					if (g_engine.m_coordinator.ComponentExists<SpriteComponent>(i))
					{
						if (ImGui::CollapsingHeader("Sprite"))
						{
							auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(i);
							sprite.DisplayOnInspector();			
							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<SpriteComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<AnimationComponent>(i))
					{
						if (ImGui::CollapsingHeader("Animation"))
						{
							auto& animation = g_engine.m_coordinator.GetComponent<AnimationComponent>(i);
							animation.DisplayOnInspector();
							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<AnimationComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<FadeComponent>(i))
					{
						if (ImGui::CollapsingHeader("Fade"))
						{
							auto& fade = g_engine.m_coordinator.GetComponent<FadeComponent>(i);
							fade.DisplayOnInspector();
							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<FadeComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(i))
					{
						if (ImGui::CollapsingHeader("Circle 2D Collider"))
						{
							auto& circle2D = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(i);
							auto& collider = g_engine.m_coordinator.GetComponent<ColliderComponent>(i);
							circle2D.DisplayOnInspector();
							collider.DisplayOnInspector();

							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<CircleCollider2DComponent>(i);

								if (!g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(i))
									g_engine.m_coordinator.RemoveComponent<ColliderComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(i))
					{
						if (ImGui::CollapsingHeader("Box2D Collider"))
						{
							auto& box2D = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(i);
							auto& collider = g_engine.m_coordinator.GetComponent<ColliderComponent>(i);
							box2D.DisplayOnInspector();
							collider.DisplayOnInspector();
						
							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<BoxCollider2DComponent>(i);

								if (!g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(i))
									g_engine.m_coordinator.RemoveComponent<ColliderComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(i))
					{
						if (ImGui::CollapsingHeader("RigidBody"))
						{
							auto& RigidBody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(i);
							RigidBody.DisplayOnInspector();
							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<RigidbodyComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<LogicComponent>(i))
					{
						if (ImGui::CollapsingHeader("Logic"))
						{
							auto& Logic = g_engine.m_coordinator.GetComponent<LogicComponent>(i);
							Logic.DisplayOnInspector();
							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<LogicComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<StatsComponent>(i))
					{
						if (ImGui::CollapsingHeader("Stats"))
						{
							auto& Stats = g_engine.m_coordinator.GetComponent<StatsComponent>(i);
							Stats.DisplayOnInspector();
							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<StatsComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<CameraComponent>(i))
					{
						if (ImGui::CollapsingHeader("Camera"))
						{
							auto& Camera = g_engine.m_coordinator.GetComponent<CameraComponent>(i);
							Camera.DisplayOnInspector();
							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<CameraComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<UIComponent>(i))
					{
						if (ImGui::CollapsingHeader("UI"))
						{
							auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(i);
							UI.DisplayOnInspector();

							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<UIComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(i))
					{
						if (ImGui::CollapsingHeader("Player Controller"))
						{
							auto& PCC = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(i);
							PCC.DisplayOnInspector();

							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<PlayerControllerComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<AudioEmitterComponent>(i))
					{
						if (ImGui::CollapsingHeader("Sound"))
						{
							auto& audio = g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(i);
							audio.DisplayOnInspector();

							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<AudioEmitterComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<ParticleEmitterComponent>(i))
					{
						if (ImGui::CollapsingHeader("Particle Emitter"))
						{
							auto& particleEmitter = g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(i);
							particleEmitter.DisplayOnInspector();

							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<ParticleEmitterComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<ForegroundComponent>(i))
					{
						if (ImGui::CollapsingHeader("Foreground"))
						{
							auto& foreground = g_engine.m_coordinator.GetComponent<ForegroundComponent>(i);
							foreground.DisplayOnInspector();

							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<ForegroundComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<BackgroundComponent>(i))
					{
						if (ImGui::CollapsingHeader("Background"))
						{
							auto& particleEmitter = g_engine.m_coordinator.GetComponent<BackgroundComponent>(i);
							particleEmitter.DisplayOnInspector();

							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<BackgroundComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<CursorComponent>(i))
					{
						// If we need to edit anything about the cursor e.g offset from the actual cursor
					}

					if (g_engine.m_coordinator.ComponentExists<TextComponent>(i))
					{
						if (ImGui::CollapsingHeader("Text"))
						{
							auto& text = g_engine.m_coordinator.GetComponent<TextComponent>(i);
							text.DisplayOnInspector();

							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<TextComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<MaskingComponent>(i))
					{
						if (ImGui::CollapsingHeader("Masking"))
						{
							auto& masking = g_engine.m_coordinator.GetComponent<MaskingComponent>(i);
							masking.DisplayOnInspector();

							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<MaskingComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<LightComponent>(i))
					{
						if (ImGui::CollapsingHeader("Light"))
						{
							auto& Light = g_engine.m_coordinator.GetComponent<LightComponent>(i);
							Light.DisplayOnInspector();
							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<LightComponent>(i);
							}
						}
					}

					if (g_engine.m_coordinator.ComponentExists<SoulComponent>(i))
					{
						if (ImGui::CollapsingHeader("Soul"))
						{
							auto& soul = g_engine.m_coordinator.GetComponent<SoulComponent>(i);
							soul.DisplayOnInspector();
							if (ImGui::Button("Remove Component"))
							{
								g_engine.m_coordinator.RemoveComponent<SoulComponent>(i);
							}
						}
					}

					ImGui::Separator();

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
							if (ImGui::MenuItem("Sprite Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<SpriteComponent>(i)))
							{
								auto& Sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(i);
								Sprite.Deserialize("Resources/Assets/DefaultSprite.png;1;1;1;1;1");
							}

							if (ImGui::MenuItem("Animation Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<AnimationComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, AnimationComponent());
							}

							if (ImGui::MenuItem("Fade Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<FadeComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, FadeComponent());
							}

							if (ImGui::MenuItem("Transform Component",nullptr,false, !g_engine.m_coordinator.ComponentExists<TransformComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent<TransformComponent>(i,TransformComponent(Vec2{ 0.0f, 0.0f }, Vec2{ 100.0f, 100.0f }, 0.0f));
							}

							if (ImGui::MenuItem("Player Controller Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, PlayerControllerComponent());
							}

							if (ImGui::MenuItem("Logic Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<LogicComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, LogicComponent());
								g_engine.m_coordinator.AddComponent(i, StatsComponent());
							}
							
							if (ImGui::MenuItem("Cursor Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<CursorComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, CursorComponent());
							}

							if (ImGui::MenuItem("Particle Emitter Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<ParticleEmitterComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, ParticleEmitterComponent());
							}

							if (ImGui::MenuItem("Foreground Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<ForegroundComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, ForegroundComponent());
							}

							if (ImGui::MenuItem("Background Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<BackgroundComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, BackgroundComponent());
							}

							if (ImGui::MenuItem("Masking Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<MaskingComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, MaskingComponent());
							}

							if (ImGui::MenuItem("Light Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<LightComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, LightComponent());
							}

							if (ImGui::MenuItem("Soul Component", nullptr, false, !g_engine.m_coordinator.ComponentExists<SoulComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, SoulComponent());
							}

							ImGui::EndMenu();
						}
						if (ImGui::BeginMenu("Colliders"))
						{
							if (ImGui::MenuItem("Circle Collider", nullptr, false, !g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, CircleCollider2DComponent());

								if(!g_engine.m_coordinator.ComponentExists<ColliderComponent>(i))
									g_engine.m_coordinator.AddComponent(i, ColliderComponent(std::make_shared<CircleShape>()));
							}

							if (ImGui::MenuItem("Box Collider 2D", nullptr, false, !g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(i)))
							{
								g_engine.m_coordinator.AddComponent(i, BoxCollider2DComponent());

								if (!g_engine.m_coordinator.ComponentExists<ColliderComponent>(i))
									g_engine.m_coordinator.AddComponent(i, ColliderComponent(std::make_shared<BoxShape>()));
							}

							ImGui::EndMenu();
						}

						if (ImGui::MenuItem("RigidBody", nullptr, false, !g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(i)))
						{
							g_engine.m_coordinator.AddComponent(i, RigidbodyComponent());
						}

						if (ImGui::MenuItem("Camera", nullptr, false, !g_engine.m_coordinator.ComponentExists<CameraComponent>(i)))
						{
							g_engine.m_coordinator.AddComponent(i, CameraComponent());
						}

						if (ImGui::MenuItem("Sound", nullptr, false, !g_engine.m_coordinator.ComponentExists<AudioEmitterComponent>(i)))
						{
							g_engine.m_coordinator.AddComponent(i, AudioEmitterComponent());
						}

						if (ImGui::MenuItem("UI", nullptr, false, !g_engine.m_coordinator.ComponentExists<UIComponent>(i)))
						{
							g_engine.m_coordinator.AddComponent(i, UIComponent());
						}

						if (ImGui::MenuItem("Text", nullptr, false, !g_engine.m_coordinator.ComponentExists<TextComponent>(i)))
						{
							g_engine.m_coordinator.AddComponent(i, TextComponent());
						}
						ImGui::EndPopup(); 
						
					}
					if (ImGui::BeginPopup("Delete Component"))
					{
						if (ImGui::BeginMenu("Component"))
						{
							if (ImGui::MenuItem("Sprite Component", nullptr, false, g_engine.m_coordinator.ComponentExists<SpriteComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<SpriteComponent>(i);
							}

							if (ImGui::MenuItem("Animation Component", nullptr, false, g_engine.m_coordinator.ComponentExists<AnimationComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<AnimationComponent>(i);
							}

							if (ImGui::MenuItem("Fade Component", nullptr, false, g_engine.m_coordinator.ComponentExists<FadeComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<FadeComponent>(i);
							}

							if (ImGui::MenuItem("Transform Component", nullptr, false, g_engine.m_coordinator.ComponentExists<TransformComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<TransformComponent>(i);
							}

							if (ImGui::MenuItem("Player Controller Component", nullptr, false, g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<PlayerControllerComponent>(i);
							}

							if (ImGui::MenuItem("Logic Component", nullptr, false, g_engine.m_coordinator.ComponentExists<LogicComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<LogicComponent>(i);
								g_engine.m_coordinator.RemoveComponent<StatsComponent>(i);
							}

							if (ImGui::MenuItem("Cursor Component", nullptr, false, g_engine.m_coordinator.ComponentExists<CursorComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<LogicComponent>(i);
							}
							if (ImGui::MenuItem("Particle Emitter Component", nullptr, false, g_engine.m_coordinator.ComponentExists<ParticleEmitterComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<ParticleEmitterComponent>(i);
							}
							if (ImGui::MenuItem("Masking Component", nullptr, false, g_engine.m_coordinator.ComponentExists<MaskingComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<MaskingComponent>(i);
							}
							if (ImGui::MenuItem("Light Component", nullptr, false, g_engine.m_coordinator.ComponentExists<LightComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<LightComponent>(i);
							}

							ImGui::EndMenu();
						}
						if (ImGui::BeginMenu("Colliders"))
						{
							if (ImGui::MenuItem("Circle Collider", nullptr, false, g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<CircleCollider2DComponent>(i);
								
								// If no more collider components in entity, remove the general collider component.
								if (!g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(i) &&
									g_engine.m_coordinator.ComponentExists<ColliderComponent>(i))
								{
									g_engine.m_coordinator.RemoveComponent<ColliderComponent>(i);
								}
							}

							if (ImGui::MenuItem("Box Collider 2D", nullptr, false, g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(i)))
							{
								g_engine.m_coordinator.RemoveComponent<BoxCollider2DComponent>(i);

								if (!g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(i) &&
									g_engine.m_coordinator.ComponentExists<ColliderComponent>(i))
								{
									g_engine.m_coordinator.RemoveComponent<ColliderComponent>(i);
								}
							}

							ImGui::EndMenu();
						}

						if (ImGui::MenuItem("RigidBody", nullptr, false, g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(i)))
						{
							g_engine.m_coordinator.RemoveComponent<RigidbodyComponent>(i);
						}

						if (ImGui::MenuItem("Camera", nullptr, false, g_engine.m_coordinator.ComponentExists<CameraComponent>(i)))
						{
							g_engine.m_coordinator.RemoveComponent<CameraComponent>(i);
						}

						if (ImGui::MenuItem("Sound", nullptr, false, g_engine.m_coordinator.ComponentExists<AudioEmitterComponent>(i)))
						{
							g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(i).DestroySound();
							g_engine.m_coordinator.RemoveComponent<AudioEmitterComponent>(i);
						}

						if (ImGui::MenuItem("UI", nullptr, false, g_engine.m_coordinator.ComponentExists<UIComponent>(i)))
						{
							g_engine.m_coordinator.RemoveComponent<UIComponent>(i);
						}

						if (ImGui::MenuItem("Text", nullptr, false, g_engine.m_coordinator.ComponentExists<TextComponent>(i)))
						{
							g_engine.m_coordinator.RemoveComponent<TextComponent>(i);
						}

						if (ImGui::MenuItem("Soul", nullptr, false, g_engine.m_coordinator.ComponentExists<SoulComponent>(i)))
						{
							g_engine.m_coordinator.RemoveComponent<SoulComponent>(i);
						}

						ImGui::EndPopup();
					}

					if (ImGui::Button("Clone Object"))
					{
						g_engine.m_coordinator.Clone(i);
					}
					ImGui::SameLine();
					if (ImGui::Button("Delete Object"))
					{
						if (g_engine.m_coordinator.ComponentExists<AudioEmitterComponent>(i))
							g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(i).DestroySound();

						g_engine.m_coordinator.AddToDeleteQueue(i);
						//SceneManager::instance().DeleteActiveEntity(i);
					}
					std::stringstream ss;
					ss << "Number of active entities: " << g_engine.m_coordinator.GetActiveObjects().size();
					ss << "\nNumber of Hierarchy Infos: " << g_engine.m_coordinator.GetHierarchyInfoArray().size();
					RE_INFO(ss.str());

				}		
		}

		bool m_worldCamera = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetWorldCamera();
		ImGui::Checkbox("Toggle Editor Camera?", &m_worldCamera);
		g_engine.m_coordinator.GetSystem<CameraSystem>()->SetWorldCamera(m_worldCamera);

		float m_cameraZoom = CameraManager::instance().GetCameraZoom();
		glm::vec3 m_cameraPos = CameraManager::instance().GetCameraPos();

		Vec2 m_cameraMin = CameraManager::instance().GetCameraMin();
		Vec2 m_cameraMax = CameraManager::instance().GetCameraMax();
		float m_levelCameraZoom = CameraManager::instance().GetLevelCameraZoom();

		Vec2 m_startCameraPos = PLAYER_STATUS.GetStartingPos();

		/* if (m_worldCamera)
		{
			m_cameraPos.x = 0.0f;
			m_cameraPos.y = 0.0f;
			m_cameraZoom = 1.630f;
		} */

		ImGui::DragFloat("Camera X", &m_cameraPos.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Y", &m_cameraPos.y, 1.0f, -10000.0f, 10000.0f);

		ImGui::DragFloat("Camera Min X", &m_cameraMin.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Min Y", &m_cameraMin.y, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Max X", &m_cameraMax.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Max Y", &m_cameraMax.y, 1.0f, -10000.0f, 10000.0f);

		ImGui::DragFloat("Camera Start X", &m_startCameraPos.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::DragFloat("Camera Start Y", &m_startCameraPos.y, 1.0f, -10000.0f, 10000.0f);

		ImGui::DragFloat("Level Camera Zoom", &m_levelCameraZoom, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("Camera Zoom", &m_cameraZoom, 0.01f, 0.0f, 10.0f);

		if (ImGui::Button("Reset Camera"))
		{
			m_cameraPos.x = 0.0f;
			m_cameraPos.y = 0.0f;
			m_cameraZoom = 1.630f;
		}

		CameraManager::instance().SetCameraPos(m_cameraPos);
		CameraManager::instance().SetCameraMin(m_cameraMin);
		CameraManager::instance().SetCameraMax(m_cameraMax);
		PLAYER_STATUS.SetStartingPos(m_startCameraPos);
		CameraManager::instance().SetLevelCameraZoom(m_levelCameraZoom);
		CameraManager::instance().SetCameraZoom(m_cameraZoom);

		bool m_toggleGravity = g_engine.m_coordinator.GetSystem<PhysicsSystem>()->getToggleGravity();
		Vec2 m_gravity = g_engine.m_coordinator.GetSystem<PhysicsSystem>()->getGravity();

		ImGui::Checkbox("Gravity?", &m_toggleGravity);
		g_engine.m_coordinator.GetSystem<PhysicsSystem>()->setToggleGravity(m_toggleGravity);

		ImGui::DragFloat("Set Gravity", &m_gravity.y, 1.0f, -10000.0f, 10000.0f);
		g_engine.m_coordinator.GetSystem<PhysicsSystem>()->setGravity(m_gravity);
		if (ImGui::Button("TileSet"))
		{
			if (!EditorManager::instance().GetTileInstanceCount())
			{
				ImGuiTileSet::instance().Init();
				EditorManager::instance().setTileSetOpen(true);
				EditorManager::instance().IncreaseTileInstanceCount();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Layer"))
		{	
			m_show = true;

		}
		if (m_show)
		{
			static char LayerBuffer[64];
			ImGui::Text("Layer Name");
			if (ImGui::InputText("##Layer", LayerBuffer, 64, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				if (!LayerBuffer)
				{
					return;
				}
				LayerManager::instance().AddLayer(LayerBuffer);
				memset(LayerBuffer, 0, 64);
			}
			if (ImGui::Button("Add Layer"))
			{
				if (!LayerBuffer)
				{
					return;
				}
				LayerManager::instance().AddLayer(LayerBuffer);
				memset(LayerBuffer, 0, 64);
			}
			if (ImGui::Button("Remove Layer"))
			{
				ImGui::OpenPopup("Layers");

			}
			if (ImGui::BeginPopup("Layers"))
			{
				for (int i = 0; i < LayerManager::instance().GetLayerSize(); ++i)
				{
					if (ImGui::MenuItem(LayerManager::instance().GetName(i).data()))
					{
						LayerManager::instance().RemoveLayer(i);
					}
				}
				ImGui::EndPopup();
			}
			if (ImGui::Button("Close"))
			{
				m_show = false;
			}

		}

		ImGui::End();
	}
	void ImGuiInspector::Shutdown()
	{
	}
};

