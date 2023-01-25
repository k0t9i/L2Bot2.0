#include "pch.h"

#include "Common.h"
#include <cmath>
#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

std::wstring GenerateUUID()
{
    UUID uuid;
    ::ZeroMemory(&uuid, sizeof(UUID));

    ::UuidCreate(&uuid);

    WCHAR* wszUuid = NULL;
    ::UuidToStringW(&uuid, (RPC_WSTR*)&wszUuid);

    if (wszUuid == NULL)
    {
        return L"";
    }

    std::wstring ws = wszUuid;

    ::RpcStringFree((RPC_WSTR*)&wszUuid);
    wszUuid = NULL;

    return ws;
}