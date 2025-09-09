#pragma once
#include "IComponent.h"
#include "Material.h"

class Renderer : public IComponent
{
public:
	Renderer();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	Material* material;
};
