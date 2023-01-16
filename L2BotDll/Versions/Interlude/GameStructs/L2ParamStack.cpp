#include "pch.h"
#include "L2ParamStack.h"

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
		(*__Ctor)(this, size);
	}

	L2ParamStack::~L2ParamStack()
	{
		Init();
		(*__Dtor)(this);
	}

	int L2ParamStack::PushBack(void* val)
	{
		Init();
		return (*__PushBack)(this, val);
	}

	void* L2ParamStack::Top()
	{
		Init();
		return (*__Top)(this);
	}

	void** L2ParamStack::GetBuffer()
	{
		Init();
		return (__GetBuffer)(this);
	}

	int L2ParamStack::GetBufferSize()
	{
		Init();
		return (__GetBufferSize)(this);
	}

	int L2ParamStack::GetTotalBufferSize()
	{
		Init();
		return (__GetTotalBufferSize)(this);
	}

	void L2ParamStack::Init()
	{
		// todo exceptions
		if (_hModule == 0) {
			_hModule = GetModuleHandleA("Core.dll");
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