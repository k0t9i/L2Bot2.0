// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <mutex>
#include <shared_mutex>
#include <ctime>
#include <iomanip>

std::string ConvertFromWideChar(const wchar_t* str);

#endif //PCH_H
