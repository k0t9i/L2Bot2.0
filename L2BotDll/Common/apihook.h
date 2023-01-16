#pragma once
#include <Windows.h>

void* splice(void* splicedFunctionAddress, void* hookFunction);
BOOL restore(void*& oldProc);