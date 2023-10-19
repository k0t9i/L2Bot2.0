#include "pch.h"
#include "L2ParamStack.h"
#include "Domain/Exceptions.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	void(__thiscall* L2ParamStack::__Ctor)(L2ParamStack* This, int) = 0;
	void(__thiscall* L2ParamStack::__Dtor)(L2ParamStack* This) = 0;
	int(__thiscall* L2ParamStack::__PushBack)(L2ParamStack* This, void*) = 0;
	void* (__thiscall* L2ParamStack::__Top)(L2ParamStack* This) = 0;
	void** (__thiscall* L2ParamStack::__GetBuffer)(L2ParamStack* This) = 0;
	int (__thiscall* L2ParamStack::__GetBufferSize)(L2ParamStack* This) = 0;
	int (__thiscall* L2ParamStack::__GetTotalBufferSize)(L2ParamStack* This) = 0;
	HMODULE L2ParamStack::_hModule = 0;

	L2ParamStack::L2ParamStack(int size)
	{
		Init();
		__try {
			(*__Ctor)(this, size);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw CriticalRuntimeException(L"L2ParamStack::constructor failed");
		}
	}

	L2ParamStack::~L2ParamStack()
	{
		Init();
		__try {
			(*__Dtor)(this);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw CriticalRuntimeException(L"L2ParamStack::desctructor failed");
		}
	}

	int L2ParamStack::PushBack(void* val)
	{
		Init();
		__try {
			return (*__PushBack)(this, val);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw CriticalRuntimeException(L"L2ParamStack::PushBack failed");
		}
	}

	void* L2ParamStack::Top()
	{
		Init();
		__try {
			return (*__Top)(this);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw CriticalRuntimeException(L"L2ParamStack::Top failed");
		}
	}

	void** L2ParamStack::GetBuffer()
	{
		Init();
		__try {
			return (__GetBuffer)(this);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw CriticalRuntimeException(L"L2ParamStack::GetBuffer failed");
		}
	}

	int L2ParamStack::GetBufferSize()
	{
		Init();
		__try {
			return (__GetBufferSize)(this);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw CriticalRuntimeException(L"L2ParamStack::GetBufferSize failed");
		}
	}

	int L2ParamStack::GetTotalBufferSize()
	{
		Init();
		__try {
			return (__GetTotalBufferSize)(this);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			throw CriticalRuntimeException(L"L2ParamStack::GetTotalBufferSize failed");
		}
	}

	void L2ParamStack::Init()
	{
		if (_hModule == 0) {
			_hModule = GetModuleHandleA("Core.dll");
			if (!_hModule) {
				throw CriticalRuntimeException(L"cannot load Core.dll");
			}
			(FARPROC&)__Ctor = GetProcAddress(_hModule, "??0L2ParamStack@@QAE@H@Z");
			(FARPROC&)__Dtor = GetProcAddress(_hModule, "??1L2ParamStack@@QAE@XZ");
			(FARPROC&)__PushBack = GetProcAddress(_hModule, "?PushBack@L2ParamStack@@QAEHPAX@Z");
			(FARPROC&)__Top = GetProcAddress(_hModule, "?Top@L2ParamStack@@QAEPAXXZ");
			(FARPROC&)__GetBuffer = GetProcAddress(_hModule, "?GetBuffer@L2ParamStack@@QAEPAPAXXZ");
			(FARPROC&)__GetBufferSize = GetProcAddress(_hModule, "?GetBufferSize@L2ParamStack@@QAEHXZ");
			(FARPROC&)__GetTotalBufferSize = GetProcAddress(_hModule, "?GetTotalBufferSize@L2ParamStack@@QAEHXZ");
		}
	}
}