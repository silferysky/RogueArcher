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

	void Editor::ExecuteCommand(bool isUndo)
	{
		if (isUndo)
		{
			EditorEvent* editorEv = m_undoStack.back();
			editorEv->SetIsUndo(true);
			
			switch (editorEv->GetEventType())
			{
			case EventType::EvEditorCreateObject:
				EventDispatcher::instance().AddEvent(editorEv);
				AddToRedoStack(editorEv);
				m_hierarchyVector.pop_back();
				break;
			case EventType::EvEditorCopyObject:
				break;
			case EventType::EvEditorCutObject:
				break;
			case EventType::EvEditorPasteObject:
				EventDispatcher::instance().AddEvent(editorEv);
				AddToRedoStack(editorEv);
				m_hierarchyVector.pop_back();
				break;
			default:
				break;
			}

		}
		else
		{
			EditorEvent* editorEv = m_redoStack.front();
			editorEv->SetIsUndo(false);
			EventDispatcher::instance().AddEvent(editorEv);
			AddToUndoStack(editorEv);
		}
	}

	void Editor::UndoCommand()
	{
		if (!m_undoStack.size())
			return;
		if (m_undoStack.back())
		{
			ExecuteCommand(DoingUndo);
			HandleStack(DoingUndo);
		}
	}

	void Editor::RedoCommand()
	{
		if (!m_redoStack.size())
			return;
		if (m_redoStack.back())
		{
			ExecuteCommand(DoingRedo);
			HandleStack(DoingRedo);
		}
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

	void Editor::CopyCommand()
	{
		EditorCopyObjectEvent* event = new EditorCopyObjectEvent();
		event->SetSystemReceivers((int)SystemID::id_EDITOR);
		EventDispatcher::instance().AddEvent(event);
		AddToUndoStack(event);
	}

	void Editor::PasteCommand()
	{
		EditorPasteObjectEvent* event = new EditorPasteObjectEvent();
		event->SetSystemReceivers((int)SystemID::id_EDITOR);
		EventDispatcher::instance().AddEvent(event);
		AddToUndoStack(event);
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
				m_undoStack.pop_back();
			}
		}
		else
		{
			if (!m_redoStack.size())
				return;
			if (m_redoStack.back())
			{
				m_undoStack.push_back(m_redoStack.back());
				m_redoStack.pop_back();
			}
			
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
				
				if (keycode == KeyPress::KeyC && keycodeSpecial == KeyPressSub::KeyCtrl)
				{
					CopyCommand();
				}

				if (keycode == KeyPress::KeyV && keycodeSpecial == KeyPressSub::KeyCtrl)
				{
					PasteCommand();
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
			if (createObjEvent->GetIsUndo())
			{
				m_redoStack.push_back(createObjEvent);
				g_engine.m_coordinator.AddToDeleteQueue(createObjEvent->GetEntityID());
			}
			else
			{
				m_undoStack.push_back(createObjEvent);
				createObjEvent->SetEntityID(SceneManager::instance().Create2DSprite());
			}
			
			
			return;
		}
		case EventType::EvEditorCopyObject:
		{
			EditorCopyObjectEvent* copyObjEvent = dynamic_cast<EditorCopyObjectEvent*>(ev);
			if (copyObjEvent->GetIsUndo())
			{
				m_redoStack.push_back(copyObjEvent);
				//m_checkSprite = false;
				//m_checkCollision = false;
				g_engine.m_coordinator.AddToDeleteQueue(m_copiedEntity);
			}
			else
			{
				m_undoStack.push_back(copyObjEvent);
				for (auto& i : m_hierarchyVector)
				{
					if (g_engine.m_coordinator.GetHierarchyInfo(i).m_selected)
					{
						m_copiedEntity = i;
					}
				}
			}
			return;
		}
		case EvEditorPasteObject:
		{
			EditorPasteObjectEvent* pasteObjEvent = dynamic_cast<EditorPasteObjectEvent*>(ev);
			if (pasteObjEvent->GetIsUndo())
			{
				g_engine.m_coordinator.AddToDeleteQueue(m_pastedEntitesVector.back());
				m_pastedEntitesVector.pop_back();
				AddToRedoStack(m_undoStack.back());
				m_undoStack.pop_back();

			}
			else
			{
				m_undoStack.push_back(pasteObjEvent);
				m_pastedEntity = g_engine.m_coordinator.clone(m_copiedEntity);
				m_pastedEntitesVector.push_back(m_pastedEntity);
			}
			return;
		}
		}
	}

	void Editor::Shutdown()
	{
		EditorManager::instance().Shutdown();
	}
}

