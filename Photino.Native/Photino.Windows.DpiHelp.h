#pragma once

#include <windows.h>

void CloseDpiHelper();
int GetScreenHeight(int dpi);
int GetScreenWidth(int dpi);
unsigned int GetWindowDpi(HWND hwnd);
void InitDpiHelper();

