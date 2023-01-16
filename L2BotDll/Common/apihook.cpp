#include "pch.h"
#include "apihook.h"
#include "Trampoline.h"

#pragma pack(push, 1)
struct CallJmpInstr
{
	BYTE opcode;
	DWORD rel32;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct SavedFunction
{
	DWORD originalAddress;
	BYTE size;
	BYTE oldCode[5];
	CallJmpInstr* jumpInstruction;
};
#pragma pack(pop)

/*
* Если в начале функции для сплайсинг стоит инструкция jump (0xe9), то обычный сплайсинг не будет работать
* Необходимо пересчитать смещение джампа из оригинальной функции как при сохранении, так и при восстановлении оригинального кода
*/
void recalculateRel32IfIsJump(void* dest, void* source)
{
	CallJmpInstr* mayBeJump = (CallJmpInstr*)dest;
	if (mayBeJump->opcode == 0xe9)
	{
		mayBeJump->rel32 = (DWORD)source - (DWORD)dest + mayBeJump->rel32;
	}
}

BYTE saveOldFunction(void* proc, void* old)
{
	CopyMemory(old, proc, 5);
	recalculateRel32IfIsJump(old, proc);
	CallJmpInstr* instr = (CallJmpInstr*)((BYTE*)old + 5);
	instr->opcode = 0xe9;
	instr->rel32 = (DWORD)((BYTE*)proc - (BYTE*)old - 5);
	return 5;
}

void* splice(void* splicedFunctionAddress, void* hookFunction)
{
	DWORD oldProtect;
	VirtualProtect((DWORD*)splicedFunctionAddress, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	void* oldFunction = malloc(255);
	*(DWORD*)oldFunction = (DWORD)splicedFunctionAddress;
	*((BYTE*)oldFunction + 4) = saveOldFunction((DWORD*)((BYTE*)splicedFunctionAddress), (DWORD*)((BYTE*)oldFunction + 5));
	CallJmpInstr* instr = (CallJmpInstr*)((BYTE*)splicedFunctionAddress);
	instr->opcode = 0xe9;
	instr->rel32 = (DWORD)hookFunction - (DWORD)splicedFunctionAddress - 5;
	VirtualProtect((DWORD*)splicedFunctionAddress, 5, oldProtect, &oldProtect);

	return (DWORD*)((BYTE*)oldFunction + 5);
}

BOOL restore(void*& oldProc)
{
	if (oldProc != 0 && *((BYTE*)(*(DWORD*)((BYTE*)oldProc - 5))) == 0xe9) {
		void* proc = (DWORD*)(*(DWORD*)((BYTE*)oldProc - 5));
		DWORD size = (BYTE)(*(DWORD*)((BYTE*)oldProc - 1));
		DWORD oldProtect;
		VirtualProtect(proc, size, PAGE_EXECUTE_READWRITE, &oldProtect);
		CopyMemory(proc, oldProc, size);
		recalculateRel32IfIsJump(proc, oldProc);
		VirtualProtect(proc, size, oldProtect, &oldProtect);
		free((DWORD*)((BYTE*)oldProc - 5));
		oldProc = 0;
		return TRUE;
	}
	return FALSE;
}
