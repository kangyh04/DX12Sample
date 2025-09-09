#pragma once
#include <unordered_map>
#include <queue>
#include <memory>
#include "Func.h"

using namespace std;


template<typename T>
class PooledObjectTemplate
{
public:
	PooledObjectTemplate() = default;
	PooledObjectTemplate(Action<T*, int> objConfigurator, Action<T*> objActivator, Action<T*> objDeactivator)
		:configurator(objConfigurator), activator(objActivator), deactivator(objDeactivator)
	{
	}

	unique_ptr<T> Create(int count)
	{
		unique_ptr<T> t = make_unique<T>();
		configurator(t.get(), count);
		return t;
	}

	T* Activate(T* obj)
	{
		activator(obj);
		return obj;
	}

	T* Deactivate(T* obj)
	{
		deactivator(obj);
		return obj;
	}

private:
	Action<T*, int> configurator;
	Action<T*> activator;
	Action<T*> deactivator;
};


template<typename TKey, typename TItem>
class ObjectPool
{
public:
	int CreatedItemAmount(TKey key)
	{
		return allItems[key].size();
	}

	int RemainedItemAmount(TKey key)
	{
		return container[key].size();
	}

	int PopedItemAmount(TKey key)
	{
		return popedItems[key].size();
	}

	bool IsKeyRegisted(TKey key)
	{
		return templates.find(key) != templates.end();
	}

	void Regist(TKey key, Action<TItem*, int> configurator, Action<TItem*> activator, Action<TItem*> deactivator)
	{
		templates[key] = make_unique<PooledObjectTemplate<TItem>>(configurator, activator, deactivator);
	}

	TItem* Pop(TKey key)
	{
		auto& itemContainer = container[key];
		auto& temp = *templates[key];
		auto& popedItem = popedItems[key];
		if (itemContainer.size() == 0)
		{
			auto& allItem = allItems[key];
			unique_ptr<TItem> item = temp.Create(allItem.size());
			TItem* ptr = item.get();
			allItem.push_back(move(item));
			itemContainer.push(ptr);
		}
		TItem* obj = itemContainer.front();
		itemContainer.pop();

		temp.Activate(obj);
		popedItem.push_back(obj);

		return obj;
	}

	void Push(TKey key, TItem* obj)
	{
		auto& temp = *templates[key];
		temp.Deactivate(obj);
		auto& itemContainer = container[key];
		itemContainer.push(obj);
		auto& popedItem = popedItems[key];
		auto it = remove(popedItem.begin(), popedItem.end(), obj);
		popedItem.erase(it, popedItem.end());
	}

private:
	unordered_map<TKey, unique_ptr<PooledObjectTemplate<TItem>>> templates;
	unordered_map<TKey, vector<unique_ptr<TItem>>> allItems;
	unordered_map<TKey, queue<TItem*>> container;
	unordered_map<TKey, vector<TItem*>> popedItems;
};

