#pragma once
#include <vector>		//std::vector
#include <algorithm>	//std::find
#include <iterator>		//std::iterator
#include <memory>		//std::shared_ptr
#include "RogueEngine.h"

static class MemoryManager :
	public System
{
public:
	MemoryManager();
	~MemoryManager();

	void AddIntoMemory(System &sys);
	void AddIntoMemory(Entity &ent);
	void AddIntoMemory(Component &cmp);

	void RemoveFromMemory(System &sys);
	void RemoveFromMemory(Entity &ent);
	void RemoveFromMemory(Component &cmp);

	size_t SysListCount();
	size_t EntListCount();
	size_t CmpListCount();

	std::vector<std::shared_ptr<System>> GetSysList();
	std::vector<std::shared_ptr<Entity>> GetEntList();
	std::vector<std::shared_ptr<Component>> GetCmpList();

	//template <typename T>
	//void DeleteSharedPtrTo(std::shared_ptr<T> ptr) { delete ptr; }

private:
	std::vector<std::shared_ptr<System>> SysList;
	std::vector<std::shared_ptr<Entity>> EntList;
	std::vector<std::shared_ptr<Component>> CmpList;
};

