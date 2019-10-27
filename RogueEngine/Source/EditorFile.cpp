#include "EditorFile.h"
#include "Main.h"

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
					sceneManager.ClearActiveEntities();
					g_engine.m_coordinator.DestroyAllEntity();
				}
				if (ImGui::MenuItem("Open Scene"))
				{
					SceneManager& sceneManager = g_engine.m_coordinator.GetSceneManager();
					sceneManager.ClearActiveEntities();
					g_engine.m_coordinator.DestroyAllEntity();
					//sceneManager.LoadLevel(sceneManager.getCurrentFileName().c_str());
				}
				if (ImGui::MenuItem("Save Scene"))
				{
					SceneManager& sceneManager = g_engine.m_coordinator.GetSceneManager();
					g_engine.m_coordinator.GetSceneManager().SaveLevel(sceneManager.getCurrentFileName().c_str());
				}
				if (ImGui::MenuItem("Save Scene As"))
				{
					
				}
				if (ImGui::MenuItem("New Project"))
				{
					
				}
				if (ImGui::MenuItem("Save Project"))
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

