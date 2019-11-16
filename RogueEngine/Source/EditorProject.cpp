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

			for (auto& i : m_Directories)
			{
				ImGui::Selectable(i.c_str());
			}
		}
		ImGui::End();
		ImGui::Begin("File");
		if (ImGui::CollapsingHeader("File Display"))
		{
			//ImGui::ImageButton();
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
			auto lead = std::string(level * 3, ' ');
			for (const auto& entry : std::filesystem::directory_iterator(pathToShow))
			{
				auto filename = entry.path().filename();
				if (std::filesystem::is_directory(entry.status()))
				{
					std::cout << lead << "[+] " << filename << "\n";
					DisplayDirectoryTreeImp(entry, level + 1);
					std::cout << "\n";
					m_Directories.emplace_back(filename.string());
				}
				else if (std::filesystem::is_regular_file(entry.status()))
				{
					//DisplayFileInfo(entry, lead, filename);
				}
				else
					std::cout << lead << " [?]" << filename << "\n";
			}
		}
	}
	void ImGuiProject::DisplayDirectoryTree(const std::filesystem::path& pathToShow)
	{
		DisplayDirectoryTreeImp(pathToShow, 0);
	}
}

