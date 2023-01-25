#pragma once

#include <cstdint>
#include <string>

struct ChatMessage
{
	const uint32_t objectId = 0;
	const uint8_t channel = 0;
	const std::wstring name = L"";
	const std::wstring text = L"";
};