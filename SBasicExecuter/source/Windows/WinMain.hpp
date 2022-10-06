#ifndef WINMAIN_HPP
#define WINMAIN_HPP

#include <Windows.h>

static const int DISPLAY_COLOR = 0;
static const int DISPLAY_GREEN = 1;

unsigned int* GetFrameBuffer(void);
HWND GetWindowHandle(void);

#endif
