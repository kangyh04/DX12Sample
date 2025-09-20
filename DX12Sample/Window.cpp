#include "Window.h"
#include <iostream>

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Window::GetWindow()->MsgProc(hwnd, msg, wParam, lParam);
}

Window::Window(HINSTANCE hInstance, unique_ptr<IGraphicsDevice> graphicsDevice, unique_ptr<Input> inputSystem, vector<unique_ptr<World>> worlds)
	: mainWndInst(hInstance), device(move(graphicsDevice)), input(move(inputSystem))
{
	for (auto& world : worlds)
	{
		allWorlds.push_back(move(world));
	}
}

Window::~Window()
{

}

int Window::Run()
{
	MSG msg = { 0 };

	timer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Tick();
			if (!appPaused)
			{
				Update(timer);
				Draw(timer);
				device->Draw();
			}
			else
			{
				Sleep(1000);
			}
		}
	}

	return (int)msg.wParam;
}

bool Window::Initialize()
{
	if (!InitMainWindow())
	{
		return false;
	}

	if (!device->Initialize(mainWnd, clientWidth, clientHeight))
	{
		return false;
	}

	OnResize();

	return true;
}

void Window::OnResize()
{
	device->OnResize(clientWidth, clientHeight);
}

LRESULT Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			appPaused = true;
			timer.Stop();
		}
		else
		{
			appPaused = false;
			timer.Start();
		}
		return 0;
	case WM_SIZE:
		clientWidth = LOWORD(lParam);
		clientHeight = HIWORD(lParam);

		if (wParam == SIZE_MINIMIZED)
		{
			appPaused = true;
			minimized = true;
			maximized = false;
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			appPaused = false;
			minimized = false;
			maximized = true;
			OnResize();
		}
		else if (wParam == SIZE_RESTORED)
		{
			if (minimized)
			{
				appPaused = false;
				minimized = false;
				OnResize();
			}
			else if (maximized)
			{
				appPaused = false;
				maximized = false;
				OnResize();
			}
			else if (resizing)
			{

			}
			else
			{
				OnResize();
			}
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		appPaused = true;
		resizing = true;
		timer.Stop();
		return 0;

	case WM_EXITSIZEMOVE:
		appPaused = false;
		resizing = false;
		timer.Start();
		OnResize();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		return 0;

	case WM_INPUT:
		input->ProcessInput(lParam);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool Window::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mainWndInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	RECT R = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	mainWnd = CreateWindow(L"MainWnd", mainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mainWndInst, 0);
	if (!mainWnd)
	{
		MessageBox(0, L"CreateWindow Faileed.", 0, 0);
		return false;
	}

	ShowWindow(mainWnd, SW_SHOW);
	UpdateWindow(mainWnd);

	return true;
}

void Window::CreateDebugConsole()
{
	AllocConsole();

	FILE* pCout;
	FILE* pCerr;
	FILE* pCin;

	freopen_s(&pCout, "CONOUT$", "w", stdout);
	freopen_s(&pCerr, "CONOUT$", "w", stderr);
	freopen_s(&pCin, "CONIN$", "r", stdin);

	ios::sync_with_stdio(true);
	wcout.clear();
	cout.clear();
	wcerr.clear();
	cerr.clear();
	wcin.clear();
	cin.clear();
}

void Window::Update(const Timer& gt)
{
	for (auto& world : allWorlds)
	{
		world->Update();
	}
}

void Window::Draw(const Timer& gt)
{
	for (auto& world : allWorlds)
	{
		world->Draw();
	}
}
