#include <Windows.h>

#include "Universal Graphics API Hook/BaseGraphicAPIHook/BaseGraphicAPIHook.h"

#if _WIN64
void __fastcall Draw()
#else
void __stdcall Draw()
#endif
{
    static ImGuiIO& io = ImGui::GetIO();
    //io.MousePos = ImVec2(x, y);
    io.MouseDrawCursor = true;

    ImGui::ShowDemoWindow((bool*)true);
    ImGui::Begin("IDK", (bool*)true);
    ImGui::End();
}

DWORD thread(HMODULE hModule)
{
    AllocConsole();
    FILE* f = new FILE;
    freopen_s(&f, "CONOUT$", "w", stdout);

    auto graphicAPIHook = GraphicsAPIHook::Create(D3d11, "D", (tDrawRoutine)&Draw, true);
    graphicAPIHook->Enable();

    while (!GetAsyncKeyState(VK_DELETE) & 1)
    {

    }

    graphicAPIHook->Disable();

    FreeConsole();
    fclose(f);
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)thread, hModule, 0, nullptr));
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