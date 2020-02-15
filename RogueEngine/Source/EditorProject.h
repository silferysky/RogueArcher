/* Start Header ************************************************************************/
/*!
\file           EditorProject.h
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
		static ImGuiProject& instance()
		{
			static ImGuiProject instance;
			return instance;
		}
		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		
		void DisplayDirectoryTreeImp(const  std::filesystem::path& pathToShow, int level);
		void DisplayDirectoryTree(const std::filesystem::path& pathToShow);
		int m_currentLevel;
		std::string m_currentDirectory;
		std::string m_previousDirectory;
		std::string m_currentSelectedDirectory;
		std::map<std::string, std::pair<bool,std::vector<DirectoryInfo>>> m_data;
		std::map<std::string, std::pair<bool, std::vector<DirectoryInfo>>> m_updateData;
	};


}