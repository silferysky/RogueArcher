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
		
		const fs::path pathToShow{ fs::current_path() };
		////fs::current_path("Resources");
		DisplayDirectoryTree(pathToShow);
	}

	void ImGuiProject::Update()
	{
		ImGui::Begin("Project");
		if (ImGui::CollapsingHeader("Folder Hierachy"))
		{
			//ImGui::
			for (auto& i : m_data)
			{
				ImGui::Selectable(i.first.c_str());
			}
		}
		ImGui::End();
		ImGui::Begin("File");
		if (ImGui::CollapsingHeader("File Display"))
		{
			for (auto& i : m_data)
			{	
				for (auto& iterator : i.second)
				{
					ImGui::Text(iterator.c_str());
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
			for (const auto& entry : std::filesystem::directory_iterator(pathToShow))
			{
				auto filename = entry.path().filename();
				//std::string current = m_Data.begin();
				if (std::filesystem::is_directory(entry.status()))
				{
					DisplayDirectoryTreeImp(entry, level + 1);
					//m_Directories.emplace_back(filename.string());
					m_currentDirectory = filename.string();
					m_data[filename.string()].push_back("");
				}
				else if (std::filesystem::is_regular_file(entry.status()))
				{
					std::map<std::string, std::vector<std::string>>::iterator it = m_data.find(m_currentDirectory);
					if (it != m_data.end())
					{
						it->second.push_back(filename.string());
					}
					//DisplayFileInfo(entry, lead, filename);
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

