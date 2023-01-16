#pragma once

#include <windows.h>
#include <stdexcept>

namespace InjectLibrary
{
    // Вынесем инструкцию джампа в отдельную структуру, и запретим компилятору вырванивание адресов 
#pragma pack(push, 1)
    struct RelativeJumpLayout
    {
        BYTE opcode = 0;
        DWORD rel32 = 0;
    };
#pragma pack(pop)

    const BYTE SIZE_OF_JUMP = sizeof(RelativeJumpLayout);

    struct TrampolineLayout
    {
        static const BYTE CODE_MAX_SIZE = 100;
        // В этой структуре запрет на выравнивание адресов важен только для генерируемого кода
#pragma pack(push, 1)
        BYTE code[CODE_MAX_SIZE] = { 0 };
#pragma pack(pop)
        RelativeJumpLayout* jumpInstruction = nullptr;

        TrampolineLayout(const BYTE oldCodeSize) : _oldCodeSize(oldCodeSize)
        {
            if (oldCodeSize < SIZE_OF_JUMP) {
                throw std::overflow_error("oldCodeSize lesser than SIZE_OF_JUMP");
            }
            if (oldCodeSize > CODE_MAX_SIZE - SIZE_OF_JUMP) {
                throw std::overflow_error("oldCodeSize greater than CODE_MAX_SIZE - SIZE_OF_JUMP");
            }
            // Настроим адрес инструкции джампа на адрес, находящийся сразу после реальных инструкций, которые будут скопированы из перехватываемой функции
            jumpInstruction = (RelativeJumpLayout*)((BYTE*)code + _oldCodeSize);
            jumpInstruction->opcode = 0xe9;
        }

        const BYTE GetFullSize() const
        {
            return GetOldCodeSize() + SIZE_OF_JUMP;
        }

        const BYTE GetOldCodeSize() const
        {
            return _oldCodeSize;
        }
    private:
        const BYTE _oldCodeSize = 0;
    };

    class Trampoline
    {
    public:
        Trampoline(void* hookedFunctionAddress, void* hookPayloadFunctionAddress, const BYTE oldCodeSize = 0);
        virtual ~Trampoline();
        const FARPROC Install();
        void Uninstall();
        const FARPROC GetAddress() const;
        
        Trampoline(const Trampoline&) = delete;
        Trampoline& operator=(const Trampoline&) = delete;
        Trampoline(const Trampoline&&) = delete;
        Trampoline& operator=(const Trampoline&&) = delete;
    private:
        void FillLayout();
        void InstallHook() const;

    private:
        TrampolineLayout* _trampolineLayout = nullptr;
        void* _hookedFunctionAddress = nullptr;
        void* _hookPayloadFunctionAddress = nullptr;
        DWORD _protect = 0;

    };
};