#pragma once
#include <Windows.h>

#include "Universal Graphics API Hook/BaseGraphicAPIHook/BaseGraphicAPIHook.h"

class D3d10Hook : public BaseGraphicAPIHook
{
public:
	D3d10Hook(tDrawRoutine _DrawRoutine);

	virtual DWORD Enable() override;
	virtual DWORD Disable() override;
};