#pragma once
#include "GameObject.h"
#include "ObjectPool.h"
#include "Timer.h"

using namespace std;

class World
{
public:
	World() = delete;
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	~World();

public:
	void Initialize();
	void Update();
	void Draw();

public:
	GameObject* GetRootObject() { return rootGameObject.get(); }
	string GetName() { return name; }
	string SetName(string newName)
	{
		name = newName;
		return name;
	}
	int GetId() { return id; }
	int SetId(int newId) 
	{
		id = newId;
		return id;
	}

public:
	static World* GetWorld(string name);
	static World* CreateWorld(string name);

public:
	static string GetMainWorldName() { return "MainWorld"; }

private:
	unique_ptr<GameObject> rootGameObject;
	string name;
	vector<GameObject*> allObjects;
	int id = -1;
	Timer timer;

private:
	static unordered_map<string, World*> allWorlds;
	static ObjectPool<UINT, World> worldPool;
	static const UINT worldKey = 1;
};
