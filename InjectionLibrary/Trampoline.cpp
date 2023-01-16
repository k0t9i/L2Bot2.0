#include "pch.h"
#include "Trampoline.h"

namespace InjectLibrary
{
    Trampoline::Trampoline(void* hookedFunctionAddress, void* hookPayloadFunctionAddress, const BYTE oldCodeSize) :
        _hookedFunctionAddress(hookedFunctionAddress), _hookPayloadFunctionAddress(hookPayloadFunctionAddress)
    {
        auto size = oldCodeSize;
        if (size <= 0) {
            size = SIZE_OF_JUMP;
        }
        _trampolineLayout = new TrampolineLayout(size);
        // Код, который будет сгенерирован в будущем должен иметь разрешение на выполнение
        VirtualProtect(_trampolineLayout->code, _trampolineLayout->GetFullSize(), PAGE_EXECUTE_READWRITE, &_protect);
    }

    Trampoline::~Trampoline()
    {
        VirtualProtect(_trampolineLayout->code, _trampolineLayout->GetFullSize(), _protect, &_protect);
        delete _trampolineLayout;
    }

    const FARPROC Trampoline::Install()
    {
        FillLayout();
        InstallHook();
        return GetAddress();
    }

    void Trampoline::Uninstall()
    {
        DWORD oldProtect;
        VirtualProtect(_hookedFunctionAddress, SIZE_OF_JUMP, PAGE_EXECUTE_READWRITE, &oldProtect);
        // При удалении хука вернем на место затертые инструкции в перехватываемой фукнции
        CopyMemory(_hookedFunctionAddress, _trampolineLayout->code, SIZE_OF_JUMP);
        VirtualProtect(_hookedFunctionAddress, SIZE_OF_JUMP, oldProtect, &oldProtect);
    }

    const FARPROC Trampoline::GetAddress() const
    {
        return (FARPROC)(void*)_trampolineLayout->code;
    }

    void Trampoline::FillLayout()
    {
        const auto oldCodeSize = _trampolineLayout->GetOldCodeSize();
        // Скопируем первые oldCodeSize байт кода из перехватываемой функции в наш трамплин
        CopyMemory(_trampolineLayout->code, _hookedFunctionAddress, oldCodeSize);
        // Подсчитаем 32битное смещение адреса и запишем в наш трамплин после кода фукнции, скопировнного выше
        _trampolineLayout->jumpInstruction->rel32 = (DWORD)_hookedFunctionAddress - ((DWORD)_trampolineLayout->code + oldCodeSize);
    }

    void Trampoline::InstallHook() const
    {
        DWORD oldProtect;
        // Что бы изменить код перехватываемой функции, область памяти должна иметь разрешение на запись
        VirtualProtect(_hookedFunctionAddress, SIZE_OF_JUMP, PAGE_EXECUTE_READWRITE, &oldProtect);
        RelativeJumpLayout* instr = (RelativeJumpLayout*)((BYTE*)_hookedFunctionAddress);
        // Подсчитаем 32битное смещение адреса и запишем его вместе с опкодом инструкции джампа в начало перехватываемой функции
        // Джамп будет выполнен в нашу функцию, где выполняется реальная работа после перехвата и осуществляется переход на инструкцию трамплина
        instr->opcode = 0xe9;
        instr->rel32 = (DWORD)_hookPayloadFunctionAddress - ((DWORD)_hookedFunctionAddress + SIZE_OF_JUMP);
        VirtualProtect(_hookedFunctionAddress, SIZE_OF_JUMP, oldProtect, &oldProtect);
    }
}
