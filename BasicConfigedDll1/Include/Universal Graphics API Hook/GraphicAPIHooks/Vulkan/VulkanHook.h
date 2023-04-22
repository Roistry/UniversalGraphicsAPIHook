#pragma once
#include <Windows.h>

#include "Universal Graphics API Hook/BaseGraphicAPIHook/BaseGraphicAPIHook.h"

class VulkanHook : public BaseGraphicAPIHook
{
public:
	VulkanHook(tDrawRoutine _DrawRoutine);
	virtual DWORD Enable() override;
	virtual DWORD Disable() override;
};