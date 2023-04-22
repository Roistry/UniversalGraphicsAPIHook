#pragma once
#include <Windows.h>
#include <d3d12.h>

#include "Universal Graphics API Hook/BaseGraphicAPIHook/BaseGraphicAPIHook.h"

class D3d12Hook : public BaseGraphicAPIHook
{
public:
	D3d12Hook(tDrawRoutine _DrawRoutine);

	virtual DWORD Enable() override;
	virtual DWORD Disable() override;
};