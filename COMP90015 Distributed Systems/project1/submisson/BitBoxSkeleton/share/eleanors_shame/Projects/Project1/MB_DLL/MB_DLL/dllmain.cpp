#include <windows.h>

extern "C" __declspec(dllexport) double expMessageBox(const char* title, const char* message, double flags) {
	return MessageBoxA(0, message, title, flags);
}