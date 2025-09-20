#pragma once
#include "Transform.h"
#include "IComponent.h"
#include "World.h"

class GameObject
{
public:
	GameObject();
	GameObject(string name, GameObject* parent = nullptr);
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

public:
	void Initialize();
	void Update();
	void Draw();

	template<typename T>
	void AddComponent();

public:
	Transform GetTransform() const { return transform; }
	string GetName() const { return name; }
	void SetName(string newName)
	{
		name = newName;
	}
	GameObject* GetParent() const { return parent; }
	void SetParent(GameObject* newParent)
	{
		parent->RemoveFromChildren(this);
		parent = newParent;
		parent->SetChildren(this);
	}
	vector<GameObject*> GetChildren() const { return children; }
	vector<IComponent*> GetComponents() const
	{
		vector<IComponent*> components(allComponents.size());
		std::transform(allComponents.begin(), allComponents.end(), components.begin(),
			[](const unique_ptr<IComponent>& component) {return component.get(); });
		return components;
	}

public:
	template<typename T>
	static GameObject* CreateObject(string name, World* world, GameObject* parent);

private:
	void SetChildren(GameObject* child);
	void RemoveFromChildren(GameObject* child);
	void SetId(UINT newId)
	{
		id = newId;
	}

private:
	UINT id;
	string name;
	Transform transform;
	GameObject* parent;
	vector<GameObject*> children;
	vector<unique_ptr<IComponent>> allComponents;

private:
	static ObjectPool<size_t, GameObject> gameObjectPool;
};
