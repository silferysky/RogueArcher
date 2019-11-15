#pragma once
#include "Precompiled.h"
#include "imgui.h"

namespace Rogue
{
	typedef int EditorWindowFlags;
	class IEditable
	{
	public:
		virtual ~IEditable() = default;

		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;


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
		void SetName(std::string_view name)
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
}
