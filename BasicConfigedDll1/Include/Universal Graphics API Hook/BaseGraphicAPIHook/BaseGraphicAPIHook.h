#pragma once
#include <Windows.h>

#include "Hook/Hook.h"
#include "Universal Graphics API Hook/WindowProcedure/WindowProcedureHook.h"

#ifdef _WIN64
typedef void(__fastcall* tDrawRoutine)();
#else
typedef void(__stdcall* tDrawRoutine)();
#endif

enum GraphicAPIs
{
	None,
	Opengl,
	D3d9,
	D3d10,
	D3d11,
	D3d12,
	Vulkan
};

class BaseGraphicAPIHook : public WindowProcedureHook
{
public:
	static tDrawRoutine m_oDrawRoutine;
	static bool m_bUseImGui;
	static bool m_bReady;

public:
	virtual DWORD Enable() = 0;
	virtual DWORD Disable() = 0;

protected:
	Hook* m_pGraphicAPIHook;
};

static bool g_FoundGraphicAPI = false;

namespace GraphicsAPIHook
{
	//*UNSTABLE*
	//Returns Opengl->D3d9->D3d10->D3d11->D3d12->Vulkan:
	//If opengl32.dll and d3d11.dll are loaded its gonna prioratize the most right in that list and in this case its gonna return D3d11.
	GraphicAPIs GetGraphicAPI();

	//_DrawRoutine has to be a void function with no parameters with the calling convention depending on architecture
	//x64: __fastcall void Name()
	//x86: __stdcall  void Name()
	BaseGraphicAPIHook* Create(GraphicAPIs _SelectedGraphicAPI, const char* _WindowName, tDrawRoutine _DrawRoutine, bool _UseImGui);
}