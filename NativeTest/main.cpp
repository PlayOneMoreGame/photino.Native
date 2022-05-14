
#include <thread>

#include "../Photino.Native/Photino.h"
#include <shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

int main() {
  auto ui_thread = []() {
    auto _ = CoInitializeEx(nullptr, COINIT::COINIT_APARTMENTTHREADED);

    wchar_t startUrl[MAX_PATH];
    wchar_t title[256];
    wcscpy_s(title, _countof(title), L"mytest");

    HINSTANCE instance = GetModuleHandle(nullptr);
    int filenameLen = GetModuleFileName(instance, startUrl, _countof(startUrl));
    PathRemoveFileSpec(startUrl);

    wcscat_s(startUrl, _countof(startUrl), L"\\index.html");

    PhotinoInitParams params = { 0 };
    params.Size = sizeof(params);
    params.TitleWide = title;
    params.StartUrlWide = startUrl;
    params.Width = 1024;
    params.Height = 768;
    params.CenterOnInitialize = true;
    params.Minimized = false;
    params.Resizable = true;
    params.Top = 200;
    params.Left = 200;

    Photino::Register(instance);
    Photino window(&params);

    window.WaitForExit();
  };

  std::thread wt(ui_thread);
  wt.join();
}
