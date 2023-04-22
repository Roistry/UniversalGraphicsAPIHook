#pragma once
#include <iostream>

#include <Windows.h>

class Hook
{
private:
	static bool ms_bDebug;

	BYTE* m_byteSource;
	BYTE* m_byteDestination;
	size_t m_szLen;
	BYTE stolenBytes[100];
	void* m_pGateway;

public:
	static void EnableDebug();
	static void DisableDebug();

	Hook(BYTE* source, BYTE* destination, size_t len);
	void* Init();
	void UnInit();
};