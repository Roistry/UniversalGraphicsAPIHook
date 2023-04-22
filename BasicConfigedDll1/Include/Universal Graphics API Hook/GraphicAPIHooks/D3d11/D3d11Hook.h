#pragma once
#include <Windows.h>
#include <d3d11.h>

#include "Universal Graphics API Hook/BaseGraphicAPIHook/BaseGraphicAPIHook.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#pragma comment(lib, "d3d11.lib")

typedef HRESULT(_fastcall* tD3d11Present)(IDXGISwapChain* _this, UINT SyncInterval, UINT Flags);

class D3d11Hook : public BaseGraphicAPIHook
{
public:
	static tD3d11Present m_oD3d11HookRoutine;
	static IDXGISwapChain* s_pD3d11SwapChain;
	static ID3D11Device* s_pD3d11Device;
	static ID3D11DeviceContext* s_pD3d11DeviceContext;
	static ID3D11RenderTargetView* s_pD3d11RenderTargetView;
	static ID3D11Texture2D* s_pD3d11BackBuffer;

public:
	D3d11Hook(tDrawRoutine _DrawRoutine);

	virtual DWORD Enable() override;
	virtual DWORD Disable() override;
};