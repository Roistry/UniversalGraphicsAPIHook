#include "D3d9Hook.h"

tD3d9EndScene D3d9Hook::m_oD3d9HookRoutine;
IDirect3DDevice9* D3d9Hook::s_pD3d9Device;

HRESULT __stdcall D3d9Draw(IDirect3DDevice9* _This)
{
    if (!BaseGraphicAPIHook::m_bReady)
    {
        D3d9Hook::s_pD3d9Device = _This;
    }
    else
    {
        if (BaseGraphicAPIHook::m_bReady)
        {
            ImGui_ImplWin32_NewFrame();
            ImGui_ImplDX9_NewFrame();
            ImGui::NewFrame();
        }

        BaseGraphicAPIHook::m_oDrawRoutine();

        if (BaseGraphicAPIHook::m_bReady)
        {
            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        }
    }
	return D3d9Hook::m_oD3d9HookRoutine(_This);
}

D3d9Hook::D3d9Hook(tDrawRoutine _DrawRoutine)
{
	BaseGraphicAPIHook::m_oDrawRoutine = _DrawRoutine;

    IDirect3DDevice9* c_D3d9DummyDevice = nullptr;
    {
        IDirect3D9* c_D3d9Dummy = nullptr;
        c_D3d9Dummy = Direct3DCreate9(D3D_SDK_VERSION);

        D3DPRESENT_PARAMETERS d3dpp;
        d3dpp.BackBufferWidth = 800;
        d3dpp.BackBufferHeight = 600;
        d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
        d3dpp.BackBufferCount = 1;
        d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
        d3dpp.MultiSampleQuality = 0;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.hDeviceWindow = this->m_hWindow;
        d3dpp.Windowed = true;
        d3dpp.EnableAutoDepthStencil = true;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
        d3dpp.Flags = 0;
        d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        c_D3d9Dummy->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->m_hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &c_D3d9DummyDevice);
    }

    this->m_oD3d9HookRoutine = (tD3d9EndScene)(*(uintptr_t**)(*(uintptr_t**)c_D3d9DummyDevice + iEndScene));
	size_t c_HookLength = 0;
#ifdef _WIN64
	c_HookLength = 0;
#else
	c_HookLength = 7;
#endif
    this->m_pGraphicAPIHook = new Hook{ (BYTE*)this->m_oD3d9HookRoutine, (BYTE*)D3d9Draw, c_HookLength };
}

DWORD D3d9Hook::Enable()
{
	this->m_oD3d9HookRoutine = (tD3d9EndScene)this->m_pGraphicAPIHook->Init();
	while (!D3d9Hook::s_pD3d9Device)
		printf("D3d9 components are not setup yet!\n");

	if (BaseGraphicAPIHook::m_bUseImGui)
	{
		ImGui::SetCurrentContext(ImGui::CreateContext());
		ImGui_ImplWin32_Init(this->m_hWindow);
		ImGui_ImplDX9_Init(D3d9Hook::s_pD3d9Device);
		this->SetNewWindowProcedure();
	}
	BaseGraphicAPIHook::m_bReady = true;
	return 0;
}

DWORD D3d9Hook::Disable()
{
    this->m_pGraphicAPIHook->UnInit();
    if (BaseGraphicAPIHook::m_bUseImGui)
    {
        Sleep(1000); //sometimes uninit takes a little while and crashes because we shutdown imgui but hook tries to render with imgui
        ImGui_ImplWin32_Shutdown();
        ImGui_ImplDX9_Shutdown();
        ImGui::DestroyContext();
        //this->SetPreviousWindowProcedure(); freezes window for some reason
    }
    return 0;
}