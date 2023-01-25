#include "pch.h"

#include "Common.h"
#include <cmath>
#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

std::string ConvertFromWideChar(const wchar_t* str)
{
    std::wstring ws(str);
    std::string result(ws.begin(), ws.end());
    return result;
}

std::string GenerateUUID()
{
    UUID uuid;
    ::ZeroMemory(&uuid, sizeof(UUID));

    ::UuidCreate(&uuid);

    WCHAR* wszUuid = NULL;
    ::UuidToStringW(&uuid, (RPC_WSTR*)&wszUuid);

    if (wszUuid == NULL)
    {
        return "";
    }

    std::wstring ws = wszUuid;

    ::RpcStringFree((RPC_WSTR*)&wszUuid);
    wszUuid = NULL;

    return std::string(ws.begin(), ws.end());
}