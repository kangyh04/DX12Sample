#include "World.h"

World::~World()
{
}

void World::Initialize()
{

}

void World::Update()
{
	for (auto& obj : allObjects)
	{
		obj->Update();
	}
}

void World::Draw()
{

	for (auto& obj : allObjects)
	{
		obj->Draw();
	}
}

World* World::GetWorld(string name)
{
	if (allWorlds.find(name) == allWorlds.end())
	{
		// TODO : throw exception;
		return nullptr;
	}
	return allWorlds[name];
}

World* World::CreateWorld(string name)
{
	if (!worldPool.IsKeyRegisted(worldKey))
	{
		Action<World*, int> configurator([=](World* newWorld, int count)
			{
				newWorld->SetId(count);
			});
		Action<World*> activator([](World* world)
			{
				world->Initialize();
			});
		Action<World*> deactivator([](World* world)
			{

			});

		worldPool.Regist(
			worldKey,
			configurator,
			activator,
			deactivator);
	}

	auto newWorld = worldPool.Pop(worldKey);
	newWorld->SetName(name);
	allWorlds[name] = newWorld;
	return newWorld;
}
