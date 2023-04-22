#include "D3d11Hook.h"

tD3d11Present D3d11Hook::m_oD3d11HookRoutine;

IDXGISwapChain* D3d11Hook::s_pD3d11SwapChain;
ID3D11Device* D3d11Hook::s_pD3d11Device;
ID3D11DeviceContext* D3d11Hook::s_pD3d11DeviceContext;
ID3D11RenderTargetView* D3d11Hook::s_pD3d11RenderTargetView;
ID3D11Texture2D* D3d11Hook::s_pD3d11BackBuffer;

HRESULT __fastcall D3d11Draw(IDXGISwapChain* _This, UINT _SyncInterval, UINT _Flags)
{
	if (!BaseGraphicAPIHook::m_bReady)
	{
		D3d11Hook::s_pD3d11SwapChain = _This;
		if (!D3d11Hook::s_pD3d11SwapChain)
			return D3d11Hook::m_oD3d11HookRoutine(_This, _SyncInterval, _Flags);

		D3d11Hook::s_pD3d11SwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&D3d11Hook::s_pD3d11Device);
		if (!D3d11Hook::s_pD3d11Device)
			return D3d11Hook::m_oD3d11HookRoutine(_This, _SyncInterval, _Flags);

		D3d11Hook::s_pD3d11Device->GetImmediateContext(&D3d11Hook::s_pD3d11DeviceContext);
		if (!D3d11Hook::s_pD3d11DeviceContext)
			return D3d11Hook::m_oD3d11HookRoutine(_This, _SyncInterval, _Flags);

		D3d11Hook::s_pD3d11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&D3d11Hook::s_pD3d11BackBuffer);
		if (!D3d11Hook::s_pD3d11BackBuffer)
			return D3d11Hook::m_oD3d11HookRoutine(_This, _SyncInterval, _Flags);

		D3d11Hook::s_pD3d11Device->CreateRenderTargetView(D3d11Hook::s_pD3d11BackBuffer, NULL, &D3d11Hook::s_pD3d11RenderTargetView);

		D3d11Hook::s_pD3d11BackBuffer->Release();
	}
	else
	{
		if (BaseGraphicAPIHook::m_bUseImGui)
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}

		BaseGraphicAPIHook::m_oDrawRoutine();

		if (BaseGraphicAPIHook::m_bUseImGui)
		{
			ImGui::EndFrame();
			ImGui::Render();
			D3d11Hook::s_pD3d11DeviceContext->OMSetRenderTargets(1, &D3d11Hook::s_pD3d11RenderTargetView, NULL);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
	}
	return D3d11Hook::m_oD3d11HookRoutine(_This, _SyncInterval, _Flags);
}

D3d11Hook::D3d11Hook(tDrawRoutine _DrawRoutine)
{
	BaseGraphicAPIHook::m_oDrawRoutine = _DrawRoutine;
	size_t c_HookLength = 0;
	#ifdef _WIN64
	this->m_oD3d11HookRoutine = (tD3d11Present)((uintptr_t)(GetModuleHandleA("gameoverlayrenderer64.dll")) + 0x88EB0);
	c_HookLength = 16;
	#else
	this->m_oD3d11HookRoutine = (tD3d11Present)((uintptr_t)(GetModuleHandleA("gameoverlayrenderer64.dll")) + 0);
	c_HookLength = 0;
	#endif
	this->m_pGraphicAPIHook = new Hook{ (BYTE*)this->m_oD3d11HookRoutine, (BYTE*)D3d11Draw, c_HookLength };
}

DWORD D3d11Hook::Enable()
{
	this->m_oD3d11HookRoutine = (tD3d11Present)this->m_pGraphicAPIHook->Init();
	while (!D3d11Hook::s_pD3d11RenderTargetView)
		printf("D3d11 components are not setup yet!\n");

	if (BaseGraphicAPIHook::m_bUseImGui)
	{
		ImGui::SetCurrentContext(ImGui::CreateContext());
		ImGui_ImplWin32_Init(this->m_hWindow);
		ImGui_ImplDX11_Init(D3d11Hook::s_pD3d11Device, D3d11Hook::s_pD3d11DeviceContext);
		this->SetNewWindowProcedure();
	}
	BaseGraphicAPIHook::m_bReady = true;
	return 0;
}

DWORD D3d11Hook::Disable()
{
	this->m_pGraphicAPIHook->UnInit();
	if (BaseGraphicAPIHook::m_bUseImGui)
	{
		Sleep(1000); //sometimes uninit takes a little while and crashes because we shutdown imgui but hook tries to render with imgui
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
		this->SetPreviousWindowProcedure();
	}
	return 0;
}