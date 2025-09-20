#include "GameObject.h"
#include <algorithm>

GameObject::GameObject()
	:GameObject("GameObject")
{
}

GameObject::GameObject(string name, GameObject* parent = nullptr)
{
	auto newParent = parent;
	if (parent == nullptr)
	{
		auto mainWorld = World::GetWorld(World::GetMainWorldName());
		newParent = mainWorld->GetRootObject();
	}
	SetParent(newParent);
	SetName(name);
}

void GameObject::Initialize()
{

}

void GameObject::Update()
{
	for (auto& component : allComponents)
	{
		component->Update();
	}
	for (auto& child : children)
	{
		child->Update();
	}
}

void GameObject::Draw()
{
	for (auto& component : allComponents)
	{
		component->Draw();
	}
	for (auto& child : children)
	{
		child->Draw();
	}
}

template<typename T>
void GameObject::AddComponent()
{
	auto newComponent = make_unique<T>();
	allComponents.push_back(move(newComponent));
}

template<typename T>
GameObject* GameObject::CreateObject(string name, World* world, GameObject* parent)
{
	if typeId = typeid(T).hash_code();
	if (!gameObjectPool.IsKeyRegisted(typeId))
	{
		Action<GameObject*, int> configurator([](GameObject* newObj, int count)
			{
				newObj->SetId(count);
			});
		Action<GameObject*> activator([](GameObject* newObj)
			{
				newObj->Initialize();
			});
		Action<GameObject*> deactivator([](GameObject* newObj)
			{

			});

		gameObjectPool.Regist(
			typeId,
			configurator,
			activator,
			deactivator);
	}

	return gameObjectPool.Pop(typeId);
}

void GameObject::SetChildren(GameObject* child)
{
	children.push_back(child);
}

void GameObject::RemoveFromChildren(GameObject* child)
{
	remove(children.begin(), children.end(), child);
}
