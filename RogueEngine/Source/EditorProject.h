#pragma once
#include "IEditable.h"
#include <filesystem>

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
		std::vector<std::string> m_Directories;
	};
}