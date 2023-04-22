#include "BaseGraphicAPIHook.h"

#include "Universal Graphics API Hook/GraphicAPIHooks/Opengl/OpenglHook.h"
#include "Universal Graphics API Hook/GraphicAPIHooks/D3d9/D3d9Hook.h"
#include "Universal Graphics API Hook/GraphicAPIHooks/D3d10/D3d10Hook.h"
#include "Universal Graphics API Hook/GraphicAPIHooks/D3d11/D3d11Hook.h"
#include "Universal Graphics API Hook/GraphicAPIHooks/D3d12/D3d12Hook.h"
#include "Universal Graphics API Hook/GraphicAPIHooks/Vulkan/VulkanHook.h"

tDrawRoutine BaseGraphicAPIHook::m_oDrawRoutine;
bool BaseGraphicAPIHook::m_bUseImGui;
bool BaseGraphicAPIHook::m_bReady;

GraphicAPIs GraphicsAPIHook::GetGraphicAPI()
{
	GraphicAPIs c_GraphicAPI = None;
	if (GetModuleHandleA("opengl32.dll"))
		c_GraphicAPI = Opengl;

	if (GetModuleHandleA("d3d9.dll"))
		c_GraphicAPI = D3d9;

	if (GetModuleHandleA("d3d10.dll"))
		c_GraphicAPI = D3d10;

	if (GetModuleHandleA("d3d11.dll"))
		c_GraphicAPI = D3d11;

	if (GetModuleHandleA("d3d12.dll"))
		c_GraphicAPI = D3d12;

	if (GetModuleHandleA("vulkan.dll"))
		c_GraphicAPI = Vulkan;

	return c_GraphicAPI;
}

BaseGraphicAPIHook* GraphicsAPIHook::Create(GraphicAPIs _SelectedGraphicAPI, const char* _WindowName, tDrawRoutine _DrawRoutine, bool _UseImGui)
{
	BaseGraphicAPIHook* c_SelectedBaseGraphicAPIHook = nullptr;
	switch (_SelectedGraphicAPI)
	{
	case Opengl:
		c_SelectedBaseGraphicAPIHook = new OpenglHook{ _DrawRoutine };
		break;

	case D3d9:
		c_SelectedBaseGraphicAPIHook = new D3d9Hook{ _DrawRoutine };
		break;

	case D3d10:
		c_SelectedBaseGraphicAPIHook = new D3d10Hook{ _DrawRoutine };
		break;

	case D3d11:
		c_SelectedBaseGraphicAPIHook = new D3d11Hook{ _DrawRoutine };
		break;

	case D3d12:
		c_SelectedBaseGraphicAPIHook = new D3d12Hook{ _DrawRoutine };
		break;

	case Vulkan:
		c_SelectedBaseGraphicAPIHook = new VulkanHook{ _DrawRoutine };
		break;
	}

	c_SelectedBaseGraphicAPIHook->m_hWindow = FindWindowA(0, _WindowName);
	BaseGraphicAPIHook::m_bUseImGui = _UseImGui;

	return c_SelectedBaseGraphicAPIHook;
}