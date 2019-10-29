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
					sceneManager.ClearAllEntities();
					sceneManager.IncrementSceneIterator();
					sceneManager.Create2DSprite();
					std::ostringstream ostrstream;
					ostrstream << "Level " << sceneManager.GetSceneIterator() << ".json";
					sceneManager.setCurrentFileName(ostrstream.str().c_str());
					sceneManager.SaveLevel(ostrstream.str().c_str());
					sceneManager.AddToLoadedLevels(ostrstream.str());
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
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Save Scene"))
				{
					SceneManager& sceneManager = g_engine.m_coordinator.GetSceneManager();
					g_engine.m_coordinator.GetSceneManager().SaveLevel(sceneManager.getCurrentFileName().c_str());
				}
				if (ImGui::BeginMenu("Save Scene As"))
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

