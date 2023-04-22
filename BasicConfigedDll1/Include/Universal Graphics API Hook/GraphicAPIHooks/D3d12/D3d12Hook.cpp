#include "D3d12Hook.h"

D3d12Hook::D3d12Hook(tDrawRoutine _DrawRoutine)
{
	BaseGraphicAPIHook::m_oDrawRoutine = _DrawRoutine;
}

DWORD D3d12Hook::Enable()
{
	return 0;
}

DWORD D3d12Hook::Disable()
{
	return 0;
}