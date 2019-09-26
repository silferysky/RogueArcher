#pragma once
#include "EntityType.h"
#include "BaseComponent.h"
#include <cstring>
#include <vector>

struct BaseEntity
{
	char _entName[32];
	std::vector<BaseComponent*> _ownedCmpList;
	ENTITY_TYPE _entType;
	bool _isActive;

//public:
	BaseEntity(const char* entName, ENTITY_TYPE type = ENT_BASE)
		: _entType{ type }
	{
		if (entName)
			strcpy_s(_entName, entName);

		_ownedCmpList = std::vector<BaseComponent*>();
	};

	template <typename T>
	auto GetComponent(ComponentType type)
	{
		for (auto &cmp : _ownedCmpList)
		{
			if (cmp._cmpType == type)
				return dynamic_cast<T*>(cmp);
		}
		return nullptr;
	}

	template <typename T>
	bool isType(T type)
	{		
		return _entType = type;
	}
	~BaseEntity() {};
	
};

