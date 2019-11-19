#include "Precompiled.h"
#include "EditorProject.h"

namespace Rogue
{
	namespace fs = std::filesystem;
	ImGuiProject::ImGuiProject()
	{
	}

	ImGuiProject::~ImGuiProject()
	{
	}

	void ImGuiProject::Init()
	{
		
		const fs::path pathToShow{ "Resources" };
		DisplayDirectoryTree(pathToShow);
	}

	void ImGuiProject::Update()
	{
		ImGui::Begin("Project");
		if (ImGui::CollapsingHeader("Folder Hierachy"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			for (auto& i : m_data)
			{
				if(ImGui::Selectable(i.first.c_str(), i.second.first, ImGuiSelectableFlags_AllowDoubleClick))
				{
					std::string temp = i.first;
					if (ImGui::IsItemClicked() || ImGui::IsMouseClicked(0))
					{
						i.second.first = !i.second.first;
						for (auto& i : m_data)
						{
							if (temp == i.first)
								continue;
							i.second.first = false;
						}
					}
				}

			}
		}

		ImGui::End();
		ImGui::Begin("File");
		if (ImGui::CollapsingHeader("File Display"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			for (auto& i : m_data)
			{	
				if (i.second.first == false)
						continue;
				for (auto& iter : i.second.second)
				{
					ImGui::Selectable(iter.m_objectName.c_str());
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("Sprite", &iter.m_objectPath, sizeof(iter.m_objectPath));
						ImGui::BeginTooltip();
						ImGui::Text("Dragging %s",iter.m_objectName.c_str());
						ImGui::EndTooltip();
						ImGui::EndDragDropSource();
					}
				}
			}
			
		}
		ImGui::End();
	}

	void ImGuiProject::Shutdown()
	{
	}

	void ImGuiProject::DisplayDirectoryTreeImp(const std::filesystem::path& pathToShow, int level)
	{
		if (std::filesystem::exists(pathToShow) && std::filesystem::is_directory(pathToShow))
		{
			m_currentLevel = level;
			for (const auto& entry : std::filesystem::directory_iterator(pathToShow))
			{
				auto filename = entry.path().filename();
				m_previousDirectory = filename.string();
				if (std::filesystem::is_directory(entry.status()))
				{
						m_currentDirectory = filename.string();
						m_data[filename.string()].second.push_back(DirectoryInfo("","", m_currentLevel));
						DisplayDirectoryTreeImp(entry, level + 1);
				}
				else if (std::filesystem::is_regular_file(entry.status()))
				{
					DirectoryInfo temp;
					temp.m_objectName = filename.string();
					temp.m_objectPath = pathToShow.string() + "\\" + filename.string();
					for (auto& i : m_data)
					{
						if (i.first == m_currentDirectory)
						{
							i.second.second.emplace_back(temp);
						}
					}
				}
				else
					throw;
			}
		}
	}
	void ImGuiProject::DisplayDirectoryTree(const std::filesystem::path& pathToShow)
	{
		DisplayDirectoryTreeImp(pathToShow, 0);
	}
}

