#pragma once

#if defined (DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <wtypes.h>
#include <string>
#include <memory>
#include "GraphicsDevice.h"
#include "Input.h"
#include "World.h"
#include "Timer.h"

using namespace std;

class Window
{
public:
	Window(HINSTANCE hInstance, unique_ptr<IGraphicsDevice> graphicsDevice, unique_ptr<Input> input, vector<unique_ptr<World>> worlds);
	Window(const Window& rhs) = delete;
	Window& operator=(const Window& rhs) = delete;
	~Window();

public:
	int Run();
	bool Initialize();
	void OnResize();
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	bool InitMainWindow();
	void CreateDebugConsole();
	void Update(const Timer& gt);
	void Draw(const Timer& gt);

public:
	HINSTANCE GetAppInst() const { return mainWndInst; }
	HWND GetMainWnd() const { return mainWnd; }
	float GetAspectRatio() const { return static_cast<float>(clientWidth) / clientHeight; }

public:
	static Window* GetWindow() { return window; }

private:
	static Window* window;

private:
	HINSTANCE mainWndInst = nullptr;
	HWND mainWnd = nullptr;

	bool appPaused = false;
	bool minimized = false;
	bool maximized = false;
	bool resizing = false;

	int clientWidth = 800;
	int clientHeight = 600;

	wstring mainWndCaption = L"Sample Application";
	unique_ptr<IGraphicsDevice> device;
	unique_ptr<Input> input;
	vector<unique_ptr<World>> allWorlds;

	Timer timer;
};
