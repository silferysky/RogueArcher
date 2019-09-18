#pragma once
#include <vector>		//std::vector
#include <algorithm>	//std::find
#include <iterator>		//std::iterator
#include <memory>		//std::shared_ptr
#include "RogueEngine.h"

class MemoryManager :
	public BaseSystem
{
public:
	MemoryManager();
	~MemoryManager();

	void AddIntoMemory(BaseSystem &sys);
	void AddIntoMemory(BaseEntity &ent);
	void AddIntoMemory(BaseComponent &cmp);

	void RemoveFromMemory(BaseSystem &sys);
	void RemoveFromMemory(BaseEntity &ent);
	void RemoveFromMemory(BaseComponent &cmp);

	size_t SysListCount();
	size_t EntListCount();
	size_t CmpListCount();

	std::vector<std::shared_ptr<BaseSystem>> GetSysList();
	std::vector<std::shared_ptr<BaseEntity>> GetEntList();
	std::vector<std::shared_ptr<BaseComponent>> GetCmpList();

	//template <typename T>
	//void DeleteSharedPtrTo(std::shared_ptr<T> ptr) { delete ptr; }

private:
	std::vector<std::shared_ptr<BaseSystem>> SysList;
	std::vector<std::shared_ptr<BaseEntity>> EntList;
	std::vector<std::shared_ptr<BaseComponent>> CmpList;
};

