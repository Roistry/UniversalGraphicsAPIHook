#include "WindowProcedureHook.h"

WNDPROC WindowProcedureHook::s_NewWindowProcedure;
WNDPROC WindowProcedureHook::s_PreviousWindowProcedure;

LRESULT NewWindowProcedure(HWND hwnd, UINT msg, WPARAM wideparam, LPARAM lowparam)
{
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wideparam, lowparam);
	return CallWindowProcA(WindowProcedureHook::s_NewWindowProcedure, hwnd, msg, wideparam, lowparam);
}

WindowProcedureHook::WindowProcedureHook(HWND _hWindow)
{
	this->m_hWindow = _hWindow;
	WindowProcedureHook::s_PreviousWindowProcedure = (WNDPROC)GetWindowLongA(this->m_hWindow, GWLP_WNDPROC);
}

void WindowProcedureHook::SetNewWindowProcedure()
{
	WindowProcedureHook::s_NewWindowProcedure = (WNDPROC)SetWindowLongPtrW(this->m_hWindow, GWLP_WNDPROC, (LONG_PTR)NewWindowProcedure);
}

void WindowProcedureHook::SetPreviousWindowProcedure()
{
	SetWindowLongPtrW(this->m_hWindow, GWLP_WNDPROC, (LONG_PTR)WindowProcedureHook::s_PreviousWindowProcedure);
}

WindowProcedureHook::WindowProcedureHook()
{
	this->m_hWindow = nullptr;
	WindowProcedureHook::s_PreviousWindowProcedure = (WNDPROC)GetWindowLongA(this->m_hWindow, GWLP_WNDPROC);
}