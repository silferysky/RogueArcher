#include "IEditable.h"


IEditable::IEditable(const std::string& Name, bool defOpen, ImGuiWindowFlags ImGuiFlags, EditorWindowFlags EditorFlags)
	:m_Name(Name),m_ImGuiFlags(ImGuiFlags),m_EditorFlags(EditorFlags)
{

}
