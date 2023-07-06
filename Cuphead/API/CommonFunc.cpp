#include "CommonFunc.h"

std::wstring s2ws(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

int B2LEndianInt(void* ptr)
{
	const char* point = reinterpret_cast<const char*>(ptr);
	char lendian[4] = {};
	int value = 0;
	int max = 4;
	for (int i = 0; i < max; i++)
	{
		lendian[(max-1)-i] = point[i];
	}
	value = static_cast<int>(*reinterpret_cast<int*>(lendian));
	return value;
}
