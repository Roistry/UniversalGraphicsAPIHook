#include "VulkanHook.h"

VulkanHook::VulkanHook(tDrawRoutine _DrawRoutine)
{
	BaseGraphicAPIHook::m_oDrawRoutine = _DrawRoutine;
}

DWORD VulkanHook::Enable()
{
	return 0;
}

DWORD VulkanHook::Disable()
{
	return 0;
}