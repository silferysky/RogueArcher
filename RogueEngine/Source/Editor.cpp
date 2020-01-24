/* Start Header ************************************************************************/
/*!
\file           Editor.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the functions for editor

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "Editor.h"
#include "EventDispatcher.h"

namespace Rogue
{
	//void Editor::ExecuteCommand(EditorEvent* command)
	//{

	//	if (command->Execute())
	//	{
	//		m_undoStack.pop_back();
	//	}

	//	m_undoStack.push_back(command);
	//	if (!m_redoStack.size())
	//	{
	//		m_redoStack.clear();
	//	}
	//}

	void Editor::UndoCommand()
	{
		if (!m_undoStack.size())
			return;

		if (m_undoStack.back())
		{
			ExecuteCommand(DoingUndo);
			m_redoStack.push_back(m_undoStack.back());
		}
		m_undoStack.pop_back();
	}

	void Editor::RedoCommand()
	{
		if (!m_redoStack.size())
			return;
		if (m_redoStack.front())
		{
			ExecuteCommand(DoingRedo);
			m_undoStack.push_back(m_redoStack.front());
		}
		m_redoStack.pop_back();
	}

	void Editor::ClearUndoRedoStack()
	{
		if (m_undoStack.size() != NULL)
		{
			/*for (ICommandable* i : m_undoStack)
			{
				delete i;
			}*/
			m_undoStack.clear();
		}

		if (m_redoStack.size() != NULL)
		{
			/*for (ICommandable* i : m_redoStack)
			{
				delete i;
			}*/
			m_redoStack.clear();
		}
	}

	void Editor::HandleStack(bool exeUndo)
	{
		if (exeUndo)
		{
			if (!m_undoStack.size())
				return;

			if (m_undoStack.back())
			{
				m_redoStack.push_back(m_undoStack.back());
			}
			m_undoStack.pop_back();
		}
		else
		{
			if (!m_redoStack.size())
				return;
			if (m_redoStack.front())
			{
				m_undoStack.push_back(m_redoStack.front());
			}
			m_redoStack.pop_back();
			
		}
	}

	void Editor::AddToUndoStack(EditorEvent* ev)
	{
		m_undoStack.push_back(ev);
	}

	void Editor::AddToRedoStack(EditorEvent* ev)
	{
		m_redoStack.push_back(ev);
	}

	void Editor::Init()
	{
		Signature signature;
		g_engine.m_coordinator.SetSystemSignature<Editor>(signature);
		REGISTER_LISTENER(SystemID::id_EDITOR, Editor::Receive);
		EditorManager::instance().AddEditorWindow<ImGuiEditorFile>("File");
		EditorManager::instance().AddEditorWindow<ImGuiEditorEdit>("Edit");
		EditorManager::instance().AddEditorWindow<ImGuiComponent>("Component");
		EditorManager::instance().AddEditorWindow<ImGuiGameObject>("Game Object");
		EditorManager::instance().AddEditorWindow<ImGuiEditorHierarchy>("Hierarchy");
		EditorManager::instance().AddEditorWindow<ImGuiInspector>("Inspector");
		EditorManager::instance().AddEditorWindow<ImGuiProject>("Project");
		EditorManager::instance().AddEditorWindow<ImGuiEditorViewport>("Viewport");
		EditorManager::instance().AddEditorWindow<ImGuiEditorSettings>("Settings");
		EditorManager::instance().AddEditorWindow<ImGuiConsole>("Console");
		EditorManager::instance().AddEditorWindow<ImGuiProfiler>("Profiler");
		EditorManager::instance().Init();
	}

	void Editor::Update()
	{
		EditorManager::instance().Update();
	}

	void Editor::Receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent* keytriggeredevent = dynamic_cast<KeyTriggeredEvent*>(ev);
			KeyPress keycode = keytriggeredevent->GetKeyCode(); 
			if (ev->GetEventCat() & EventCatCombinedInput)
			{
				KeyTriggeredCombinedEvent* keytriggeredcombinedev = dynamic_cast<KeyTriggeredCombinedEvent*>(ev);
				KeyPress keycode = keytriggeredcombinedev->GetKeyCode();
				KeyPressSub keycodeSpecial = keytriggeredcombinedev->GetSubKey();

				if (keycode == KeyPress::KeyS && keycodeSpecial == KeyPressSub::KeyCtrl)
				{
					SceneManager& sceneManager = SceneManager::instance();
					SceneManager::instance().SaveLevel(sceneManager.getCurrentFileName().c_str());
				}

				if (keycode == KeyPress::KeyZ && keycodeSpecial == KeyPressSub::KeyCtrl)
				{
					UndoCommand();
					//Controller.UndoCommand();
				}

				if (keycode == KeyPress::KeyY && keycodeSpecial == KeyPressSub::KeyCtrl)
				{
					RedoCommand();
					//Controller.RedoCommand();
				}
				return;
			}
			/*if (keytriggeredevent->GetKeyCode() == KeyPress::Numpad8)
			{
				Entity selected = 0;
				for (auto& i : m_currentVector)
				{
					if (g_engine.m_coordinator.GetHierarchyInfo(i).m_selected == true)
						selected = i;
				}

				if (!g_engine.m_coordinator.ComponentExists<TransformComponent>(selected))
					return;

				for (int i = 0; i < 1500; ++i)
				{
					Vec2 Position = g_engine.m_coordinator.GetComponent<TransformComponent>(selected).GetPosition();
					Position.x = (float)(rand() % 1500);
					Position.y = (float)(rand() % 1500);
					g_engine.m_coordinator.GetComponent<TransformComponent>(selected).setPosition(Position);
					g_engine.m_coordinator.clone(selected);
				}
			}*/
			return;
		}
		case EventType::EvEntityPicked:
		{
			EntPickedEvent* pickedEvent = dynamic_cast<EntPickedEvent*>(ev);
			EditorManager::instance().SetPickedEntity(pickedEvent->GetEntityID());

			return;
		}
		case EventType::EvEditorCreateObject:
		{
			EditorCreateObjectEvent* createObjEvent = dynamic_cast<EditorCreateObjectEvent*>(ev);
			SceneManager::instance().Create2DSprite();
			
			return;
		}
		}
	}

	void Editor::Shutdown()
	{
		EditorManager::instance().Shutdown();
	}
}

