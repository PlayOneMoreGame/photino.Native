#pragma once

#include "Photino.Windows.DpiHelp.h"


// =============================================
//
// Types
// 
// ===================
typedef UINT (WINAPI* _GetDpiForWindowFunc)(HWND hwnd);
typedef DPI_AWARENESS_CONTEXT(WINAPI* _SetThreadDpiAwarenessContextFunc)(DPI_AWARENESS_CONTEXT dpiContext);
typedef int (WINAPI* _GetSystemMetricsForDpiFunc)(int nIndex, UINT dpi);


// =============================================
//
// Statics
// 
// ===================
static HMODULE s_user32 = nullptr;
static bool s_initialized = false;

static _GetDpiForWindowFunc _getDpiForWindow;
static _GetSystemMetricsForDpiFunc _getSystemMetricsForDpi;
static _SetThreadDpiAwarenessContextFunc _setThreadDpiAwarenessContext;


// =============================================
//
// Exports
// 
// ===================
void CloseDpiHelper() {
  if (s_user32) {
    s_initialized = false;
    FreeLibrary(s_user32);
  }
}

int GetScreenHeight(int dpi) {
  if (s_initialized && _getSystemMetricsForDpi != nullptr)
    return _getSystemMetricsForDpi(SM_CYSCREEN, dpi);

  return GetSystemMetrics(SM_CYSCREEN);
}

int GetScreenWidth(int dpi) {
  if (s_initialized && _getSystemMetricsForDpi != nullptr)
    return _getSystemMetricsForDpi(SM_CXSCREEN, dpi);

  return GetSystemMetrics(SM_CXSCREEN);
}

unsigned int GetWindowDpi(HWND hwnd) {
  if (s_initialized && _getDpiForWindow != nullptr)
    return _getDpiForWindow(hwnd);

  return 96;
}

void InitDpiHelper() {
  s_user32 = LoadLibrary(L"User32.dll");

  if (s_user32 == nullptr) {
    return;
  }

  _getDpiForWindow = (_GetDpiForWindowFunc)GetProcAddress(s_user32, "GetDpiForWindow");
  _getSystemMetricsForDpi = (_GetSystemMetricsForDpiFunc)GetProcAddress(s_user32, "GetSystemMetricsForDpi");
  _setThreadDpiAwarenessContext = (_SetThreadDpiAwarenessContextFunc)GetProcAddress(s_user32, "SetThreadDpiAwarenessContext");

  if (_setThreadDpiAwarenessContext) {
    auto oldVal = _setThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
    if (oldVal != nullptr) {
      s_initialized = true;
    }
  }
}
