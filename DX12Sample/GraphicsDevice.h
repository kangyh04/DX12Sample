#pragma once
#include <wtypes.h>

class IGraphicsDevice
{
public:
	virtual bool Initialize(HWND mainWhd, int width, int height) = 0;
	virtual void OnResize(int width, int height) = 0;
	virtual void Draw() = 0;
	virtual void RegisterMesh(class Mesh* mesh) = 0;
};
