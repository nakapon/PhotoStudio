#include <Platform.h>

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID pReserved)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
