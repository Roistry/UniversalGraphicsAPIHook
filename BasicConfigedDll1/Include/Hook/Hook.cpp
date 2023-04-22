#include "Hook.h"

bool Hook::ms_bDebug;

Hook::Hook(BYTE* source, BYTE* destination, size_t len)
{
	this->m_byteSource = source;
	this->m_byteDestination = destination;
	this->m_szLen = len;

	DWORD oldprot;
	VirtualProtect((void*)this->m_byteSource, this->m_szLen, PAGE_EXECUTE_READWRITE, &oldprot);
	memcpy(this->stolenBytes, (void*)this->m_byteSource, this->m_szLen);
	VirtualProtect((void*)this->m_byteSource, this->m_szLen, oldprot, &oldprot);

	this->m_pGateway = nullptr;
}

void* Hook::Init()
{
	BYTE* jumpbackAddress = nullptr;
	BYTE* functionDestinationAddress = nullptr;

	this->m_pGateway = VirtualAlloc(nullptr, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (Hook::ms_bDebug)
	{
		if (this->m_pGateway)
		{
			printf("[+] Allocated memory for gateway at: %p\n", this->m_pGateway);
		}
		else
		{
			printf("[!] Failed allocated memory for gateway at: %p\n", this->m_pGateway);
			return 0;
}
	}
#ifdef _WIN64
	BYTE jumpShellcode[] =
	{
		0x49, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		//mov r10, jumpAddress
		0x41, 0xFF, 0xE2												//jmp r10
	};
	static const size_t jumpShellCodeSize = 13;
	static const size_t jumpAddressOffset = 2;

	jumpbackAddress = this->m_byteSource + this->m_szLen;
	functionDestinationAddress = this->m_byteDestination;
#else
	BYTE jumpShellcode[] =
	{
		0xE9, 0x00, 0x00, 0x00, 0x00									//jmp jumpAddress (jump address must be the relative address)
	};
	static const size_t jumpShellCodeSize = 5;
	static const size_t jumpAddressOffset = 1;

	jumpbackAddress = (BYTE*)((this->m_byteSource + this->m_szLen) - ((uintptr_t)this->m_pGateway + this->m_szLen + jumpShellCodeSize));
	functionDestinationAddress = (BYTE*)(this->m_byteDestination - this->m_byteSource - jumpShellCodeSize);
#endif

	DWORD dwOldProtection;
	VirtualProtect(this->m_pGateway, this->m_szLen + jumpShellCodeSize, PAGE_EXECUTE_READWRITE, &dwOldProtection);	//setup gateway
	{
		memcpy(this->m_pGateway, this->stolenBytes, this->m_szLen);

		*(BYTE**)(jumpShellcode + jumpAddressOffset) = jumpbackAddress;
		memcpy((void*)((uintptr_t)this->m_pGateway + this->m_szLen), jumpShellcode, jumpShellCodeSize);
		VirtualProtect(this->m_pGateway, this->m_szLen + jumpShellCodeSize, dwOldProtection, &dwOldProtection);
	}

	VirtualProtect((void*)this->m_byteSource, this->m_szLen, PAGE_EXECUTE_READWRITE, &dwOldProtection);				//setup jump to our function
	{
		memset((void*)this->m_byteSource, 0x90, this->m_szLen);

		*(BYTE**)(jumpShellcode + jumpAddressOffset) = functionDestinationAddress;
		memcpy(this->m_byteSource, jumpShellcode, jumpShellCodeSize);
		VirtualProtect((void*)this->m_byteSource, this->m_szLen, dwOldProtection, &dwOldProtection);
	}

	return this->m_pGateway;
}

void Hook::UnInit()
{
	DWORD oldprot;
	VirtualProtect((void*)this->m_byteSource, this->m_szLen, PAGE_EXECUTE_READWRITE, &oldprot);
	memcpy((void*)this->m_byteSource, this->stolenBytes, this->m_szLen);
	VirtualProtect((void*)this->m_byteSource, this->m_szLen, oldprot, &oldprot);

	VirtualFree(this->m_pGateway, 0, MEM_RELEASE);
}

void Hook::EnableDebug()
{
	Hook::ms_bDebug = true;
}

void Hook::DisableDebug()
{
	Hook::ms_bDebug = false;
}