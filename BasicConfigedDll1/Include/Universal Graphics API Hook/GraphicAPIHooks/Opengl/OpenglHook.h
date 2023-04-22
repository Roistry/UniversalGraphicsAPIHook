#pragma once
#include <Windows.h>
#include <gl/GL.h>

#include "Universal Graphics API Hook/BaseGraphicAPIHook/BaseGraphicAPIHook.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_opengl2.h"

#pragma comment(lib, "opengl32.lib")

typedef BOOL(__stdcall* tOpenglWglSwapBuffers)(HDC hDC);

class OpenglHook : public BaseGraphicAPIHook
{
public:
	static tOpenglWglSwapBuffers m_oOpenglHookRoutine;

public:
	OpenglHook(tDrawRoutine _DrawRoutine);
	virtual DWORD Enable() override;
	virtual DWORD Disable() override;
};