/* Start Header ************************************************************************/
/*!
\file           EditorProject.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorProject

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
		

	}

	void ImGuiProject::Update()
	{
		const fs::path pathToShow{ "Resources" };
		m_data.clear();
		DisplayDirectoryTree(pathToShow);

		if (m_updateData.size() != m_data.size())
		{
			m_updateData.clear();
			m_updateData = m_data;
		}

		ImGui::Begin("Project");
		if (ImGui::CollapsingHeader("Folder Hierachy"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			for (auto& i : m_updateData)
			{
				if(ImGui::Selectable(i.first.c_str(), i.second.first, ImGuiSelectableFlags_AllowDoubleClick))
				{
					
				}
				std::string temp = i.first;
				if (ImGui::IsItemClicked(0))
				{
					i.second.first = !i.second.first;
					for (auto& i : m_updateData)
					{
						if (temp == i.first)
							continue;
						i.second.first = false;
					}
				}
			}
		}

		ImGui::End();
		ImGui::Begin("File");
		if (ImGui::CollapsingHeader("File Display"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			for (auto& i : m_updateData)
			{	
				if (i.second.first == false)
						continue;
				for (auto& iter : i.second.second)
				{
					ImGui::Selectable(iter.m_fileName.c_str());
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("Data", &iter, sizeof(iter));
						ImGui::BeginTooltip();
						ImGui::Text("Dragging %s", iter.m_fileName.c_str());
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
						m_data[filename.string()].second.push_back(DirectoryInfo("","", m_currentLevel,""));
						DisplayDirectoryTreeImp(entry, level + 1);
				}
				else if (std::filesystem::is_regular_file(entry.status()))
				{
					DirectoryInfo temp;
					temp.m_fileName = filename.string();
					temp.m_filePath = pathToShow.string() + "\\" + filename.string();
					size_t pos = filename.string().find_last_of(".");
					temp.m_fileType = filename.string().substr(pos + 1);
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

