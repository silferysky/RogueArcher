#pragma once
#include "IEditable.h"
#include <filesystem>

namespace Rogue
{
	class ImGuiProject : public IEditable
	{
	public:
		ImGuiProject();
		~ImGuiProject();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		
		//void DisplayDirectoryTreeImp(const  std::experimental::filesystem::path& pathToShow, int level);
		//void DisplayDirectoryTree(const std::experimental::filesystem::path& pathToShow);
		struct Data
		{
			std::string m_filePath;
			std::string m_name;
			std::vector<std::string> m_data;
		};
		std::vector<Data> m_Directories;
	};
}