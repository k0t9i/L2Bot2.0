#pragma once

#include <string>
#include <cstdint>

std::string ConvertFromWideChar(const wchar_t* str);
std::string GenerateUUID();
std::uint32_t GetEnchantValue(const std::uint16_t baseValue, const std::uint16_t enchantLevel, const uint8_t lowEnchantDelta, const uint8_t highEnchantDelta);