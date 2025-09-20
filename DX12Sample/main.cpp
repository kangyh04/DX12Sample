#include "Window.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	auto t = Window(hInstance);
	if (!t.Initialize())
	{
		return 0;
	}
	t.Run();

}