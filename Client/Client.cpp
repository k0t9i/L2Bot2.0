// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

HANDLE hPipe;
DWORD dwWritten;
BOOL   fSuccess = FALSE;
DWORD  cbRead, cbToWrite, cbWritten, dwMode;

int CreatePipe(std::wstring name)
{
    while (1)
    {
        hPipe = CreateFileW(
            name.c_str(),   // pipe name 
            GENERIC_READ |  // read and write access 
            GENERIC_WRITE,
            0,              // no sharing 
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe 
            0,              // default attributes 
            NULL);          // no template file 

      // Break if the pipe handle is valid. 

        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        // Exit if an error other than ERROR_PIPE_BUSY occurs. 

        if (GetLastError() != ERROR_PIPE_BUSY)
        {
            return -1;
        }
    }

    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(
        hPipe,    // pipe handle 
        &dwMode,  // new pipe mode 
        NULL,     // don't set maximum bytes 
        NULL);    // don't set maximum time 
    if (!fSuccess)
    {
        return -1;
    }

    return 0;
}

std::wstring ReadMessage()
{
    wchar_t  chBuf[10240];
    do
    {
        // Read from the pipe. 

        fSuccess = ReadFile(
            hPipe,    // pipe handle 
            chBuf,    // buffer to receive reply 
            10240 * sizeof(wchar_t),  // size of buffer 
            &cbRead,  // number of bytes read 
            NULL);    // not overlapped 

        if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
            break;
    } while (!fSuccess);

    return std::wstring(chBuf);
}

int main()
{
    CreatePipe(L"\\\\.\\pipe\\PipeL2Bot");
    std::cout << "Connected to the connection pipe" << std::endl;

    auto name = ReadMessage();
    CloseHandle(hPipe);
    std::wcout << L"Received main pipe name: " << name << std::endl;

    std::cin.get();
    CreatePipe(name);

    const std::wstring message = L"invalidate";
    DWORD written;
    WriteFile(hPipe, message.c_str(), message.size() + 1, &written, NULL);

    while (true) {
        const auto msg = ReadMessage();
        std::wcout << msg << std::endl;
    }
    std::cin.get();
}