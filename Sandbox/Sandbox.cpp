#include <iostream>
#include <Windows.h>

int main()
{
    /*NamedPipe p;
    p.Connect("PipeL2Bot");
    p.Send("\\\\.\\pipe\\Test");

    NamedPipe p1;
    p1.Connect("Test");

    while (true)
    {
        p1.Send("123");
        std::cin.get();
    }*/

    HMODULE hDll = LoadLibraryA("L2BotDll.dll");
    std::cin.get();
    FreeLibrary(hDll);
}
