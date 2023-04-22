#pragma once
#include <Windows.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wideparam, LPARAM lowparam);

class WindowProcedureHook
{
public:
	HWND m_hWindow;
public:
	static WNDPROC s_NewWindowProcedure;
	static WNDPROC s_PreviousWindowProcedure;

	WindowProcedureHook(HWND hwnd);
	void SetNewWindowProcedure();
	void SetPreviousWindowProcedure();

protected:
	WindowProcedureHook();
};