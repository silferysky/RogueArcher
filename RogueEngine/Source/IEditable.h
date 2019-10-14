#pragma once
#include <string>
#include "REEditor.h"
typedef int EditorWindowFlags;
class IEditable
{
public:
	virtual ~IEditable() = default;

	virtual void Init(){}
	virtual void Update() = 0;
	virtual void Shutdown() {}

	ImGuiID	GetWindowID() const;
	std::string	GetName() const 
	{
		return m_Name;
	}
	ImGuiWindowFlags GetImGuiFlags() const 
	{ 
		return m_ImGuiFlags;
	}
	EditorWindowFlags GetEditorFlags() const 
	{
		return m_EditorFlags; 
	}
	void SetName(const std::string& name) 
	{ 
		m_Name = name; 
	}
	void SetImGuiFlags(ImGuiWindowFlags flags) 
	{ 
		m_ImGuiFlags = flags;
	}
	void SetEditorFlags(EditorWindowFlags flags) 
	{ 
		m_EditorFlags = flags;
	}
protected:
	std::string m_Name;
	ImGuiWindowFlags m_ImGuiFlags;
	EditorWindowFlags m_EditorFlags;
};