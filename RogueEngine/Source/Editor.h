/* Start Header ************************************************************************/
/*!
\file           Editor.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for editor

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Editor.h"
#include "EditorManager.h"
#include "REEditor.h"
#include "REEngine.h"
#include "EditorEvent.h"
#define DoingUndo true
#define DoingRedo false

namespace Rogue
{
	class Editor : public System, public EventListener
	{
	public:
		Editor() :
			System(SystemID::id_EDITOR)
		{}
		~Editor();
		static Editor& instance()
		{
			static Editor instance;
			return instance;
		}
		void Init() override;
		void Update() override;
		void Receive(Event& ev) override final;
		void Shutdown() override;

		//void ExecuteCommand(EditorEvent& command);
		void ExecuteCommand(bool isUndo = true);
		void UndoCommand();
		void RedoCommand();
		void DeleteCommand();
		void ClearUndoRedoStack();
		void CopyCommand();
		void PasteCommand();
		void HandleStack(bool exeUndo = DoingUndo);

		void AddToUndoStack(EditorEvent& ev);
		void AddToRedoStack(EditorEvent& ev);

	private:
		std::vector<std::reference_wrapper<EditorEvent>> m_undoStack;
		std::vector<std::reference_wrapper<EditorEvent>> m_redoStack;
		std::vector<Entity>& m_currentVector = g_engine.m_coordinator.GetActiveObjects();
		std::vector<Entity>& m_hierarchyVector = ImGuiEditorHierarchy::instance().GetCurrentHierarchyVector();
		bool& m_isHierarchyActive = ImGuiEditorHierarchy::instance().GetIsHierarchyFocused();
		Entity m_copiedEntity{};
		Entity m_pastedEntity{};
		std::vector<Entity> m_pastedEntitiesVector;
		std::vector<Entity> m_deletedEntitiesVector;
		size_t m_pasteCount = 0;
	};
}
