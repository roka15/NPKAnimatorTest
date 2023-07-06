#pragma once
#include <locale> 
#include <codecvt>

std::wstring s2ws(const std::string& str);
std::string ws2s(const std::wstring& wstr);

template<typename ... Args>
std::wstring format_string(const std::wstring& format, Args ... args)
{
    size_t size = swprintf(nullptr, 0, format.c_str(), args ...) + 1;
    std::unique_ptr<wchar_t[]> buffer(new wchar_t[size]);
    swprintf(buffer.get(), size, format.c_str(), args ...);
    return std::wstring(buffer.get(), buffer.get() + size - 1);
}

class CommonFunc
{
};


int B2LEndianInt(void* ptr);


