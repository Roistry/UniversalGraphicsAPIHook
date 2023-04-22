#include "D3d10Hook.h"

D3d10Hook::D3d10Hook(tDrawRoutine _DrawRoutine)
{
	BaseGraphicAPIHook::m_oDrawRoutine = _DrawRoutine;
}

DWORD D3d10Hook::Enable()
{
	return 0;
}

DWORD D3d10Hook::Disable()
{
	return 0;
}