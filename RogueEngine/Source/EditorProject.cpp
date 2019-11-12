#include "Precompiled.h"
#include "EditorProject.h"

namespace Rogue
{
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
		ImGui::Begin("Project");
		if (ImGui::CollapsingHeader("Folder Hierachy"))
		{

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

	/*void ImGuiProject::DisplayDirectoryTreeImp(const std::experimental::filesystem::path& pathToShow, int level)
	{
		if (std::experimental::filesystem::exists(pathToShow) && std::experimental::filesystem::is_directory(pathToShow))
		{
			auto lead = std::string(level * 3, ' ');
			for (const auto& entry : std::experimental::filesystem::directory_iterator(pathToShow))
			{
				auto filename = entry.path().filename();
				if (std::experimental::filesystem::is_directory(entry.status()))
				{
					std::cout << lead << "[+] " << filename << "\n";
					DisplayDirectoryTreeImp(entry, level + 1);
					std::cout << "\n";
				}
				else if (std::experimental::filesystem::is_regular_file(entry.status()))
				{
					//DisplayFileInfo(entry, lead, filename);
				}
				else
					std::cout << lead << " [?]" << filename << "\n";
			}
		}
	}
	void ImGuiProject::DisplayDirectoryTree(const std::experimental::filesystem::path& pathToShow)
	{
		DisplayDirectoryTreeImp(pathToShow, 0);
	}*/
}

