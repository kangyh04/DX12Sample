#pragma once

class IComponent
{
public:
	virtual void Initialize() {}
	virtual void Update() {}
	virtual void Draw() {}
};
