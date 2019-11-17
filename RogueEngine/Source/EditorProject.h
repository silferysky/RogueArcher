#pragma once
#include "IEditable.h"
#include <filesystem>
#include "EditorHierarchyInfo.h"

namespace Rogue
{
	namespace fs = std::filesystem;
	class ImGuiProject : public IEditable
	{
	public:
		ImGuiProject();
		~ImGuiProject();
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		
		void DisplayDirectoryTreeImp(const  std::filesystem::path& pathToShow, int level);
		void DisplayDirectoryTree(const std::filesystem::path& pathToShow);
		std::string m_currentDirectory;
		std::string m_currentSelectedDirectory;
		std::map<std::string, std::pair<bool,std::vector<DirectoryInfo>>> m_data;
	};


}