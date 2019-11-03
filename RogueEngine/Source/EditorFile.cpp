#include "EditorFile.h"
#include "Main.h"
#include <sstream>

namespace Rogue
{
	ImGuiEditorFile::ImGuiEditorFile()
	{
	}

	ImGuiEditorFile::~ImGuiEditorFile()
	{	
	}

	void ImGuiEditorFile::Init()
	{

		
	}
	void ImGuiEditorFile::Update()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene"))
				{
					SceneManager& sceneManager = g_engine.m_coordinator.GetSceneManager();

					//Cleaning up old scene
					sceneManager.ClearAllEntities();

					//Creating new scene
					sceneManager.IncrementSceneIterator();
					sceneManager.Create2DSprite();

					//Setting values
					g_engine.m_coordinator.GetActiveObjects().begin()->m_objectName = "Background";
					g_engine.m_coordinator.GetComponent<TransformComponent>(g_engine.m_coordinator.GetActiveObjects().begin()->m_Entity).
						setScale(Vec2(GetWindowWidth(g_engine.GetWindowHandler()), GetWindowHeight(g_engine.GetWindowHandler())));
					
					//Resetting camera
					g_engine.m_coordinator.GetSystem<CameraSystem>()->ResetCamera();

					//Saving new scene so it exists (Won't crash when load non-existent scene)
					std::ostringstream ostrstream;
					ostrstream << "Level " << sceneManager.GetSceneIterator() << ".json";
					sceneManager.setCurrentFileName(ostrstream.str().c_str());
					sceneManager.SaveLevel(ostrstream.str().c_str());
					sceneManager.AddToLoadedLevels(ostrstream.str());
					sceneManager.SaveLevelFiles("Levels.json");

					//Reset game running state and game save state
					g_engine.m_coordinator.SetGameState(false);
					g_engine.m_coordinator.SetPauseState(false);
				}
				if (ImGui::BeginMenu("Open Scene"))
				{
					for (auto& levelStrIterator : g_engine.m_coordinator.GetSceneManager().GetLoadedLevels())
					{
						if (ImGui::MenuItem(levelStrIterator.c_str()))
						{
							SceneManager& sceneManager = g_engine.m_coordinator.GetSceneManager();

							sceneManager.setCurrentFileName(levelStrIterator.c_str());
							sceneManager.ClearAllEntities();
							sceneManager.LoadLevel(sceneManager.getCurrentFileName().c_str());

							g_engine.m_coordinator.SetGameState(false);
							g_engine.m_coordinator.SetPauseState(false);
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Save Scene", nullptr, false, !g_engine.m_coordinator.GetGameState()))
				{
					SceneManager& sceneManager = g_engine.m_coordinator.GetSceneManager();
					g_engine.m_coordinator.GetSceneManager().SaveLevel(sceneManager.getCurrentFileName().c_str());
				}
				if (ImGui::BeginMenu("Save Scene As", !g_engine.m_coordinator.GetGameState()))
				{
					for (auto& levelStrIterator : g_engine.m_coordinator.GetSceneManager().GetLoadedLevels())
					{
						if (ImGui::MenuItem(levelStrIterator.c_str()))
						{
							SceneManager& sceneManager = g_engine.m_coordinator.GetSceneManager();
							sceneManager.setCurrentFileName(levelStrIterator.c_str());
							g_engine.m_coordinator.GetSceneManager().SaveLevel(sceneManager.getCurrentFileName().c_str());
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("New Project",nullptr,false,false))
				{
					
				}
				if (ImGui::MenuItem("Save Project",nullptr,false,false))
				{
		
				}
				if (ImGui::MenuItem("Exit"))
				{
					g_engine.SetGameIsRunning(false);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		
	}
	void ImGuiEditorFile::Shutdown()
	{
	}
}

