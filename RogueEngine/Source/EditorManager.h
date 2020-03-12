/* Start Header ************************************************************************/
/*!
\file           EditorManager.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "REEditor.h"
#include "IEditable.h"


namespace Rogue
{
	class EditorManager
	{
	public:
		static EditorManager& instance()
		{
			static EditorManager instance;
			return instance;
		}

		EditorManager();
		~EditorManager() = default;
		void Init();
		void Update();
		void Shutdown();

		int GetTileInstanceCount() const;
		void ReduceTileInstanceCount();
		void IncreaseTileInstanceCount();

		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		template<typename T>
		T* GetEditorWindow(std::string_view name)
		{
			auto Window = m_WindowsMap.find(name);
			return (T*)Window->second;
		}
		
		template<typename T>
		const T* GetEditorWindow(std::string_view name)
		{
			auto Window = m_WindowsMap.find(name);
			return (T*)Window->second;
		}

		template <typename T>
		void AddEditorWindow(std::string_view name)
		{
			m_WindowsVector.emplace_back(std::make_shared<T>());
			m_WindowsMap.emplace(name, std::make_shared<T>());
		}

		template <typename T>
		void RemoveEditorWindow(std::string name)
		{
			m_WindowsVector.pop_back();
			m_WindowsMap.erase(name);
		}
		int GetPickedEntity() const { return m_pickedEntity; }
		void SetPickedEntity(int entity) { m_pickedEntity = entity; }

		void setTileSetOpen(bool open)
		{
			m_isTileSetOpen = open;
		}
	private:
		std::vector<std::shared_ptr<IEditable>> m_WindowsVector;
		std::unordered_map<std::string, std::shared_ptr<IEditable>> m_WindowsMap;
		float RETime = 0.0f;
		bool m_isTileSetOpen;
		int m_pickedEntity;
		int m_tileInstanceCount;
	};

}

	
