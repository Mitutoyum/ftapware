#include <Windows.h>

#include "ftapware/application.h"

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int) {
	FTAPWare::Application application;

	application.Run();

	ExitProcess(0);
	return 0;
}