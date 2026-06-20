#pragma once
#include <string>
#include <stringapiset.h>

namespace StringUtils
{
    inline std::string wide_to_utf8(const wchar_t* w)
    {
        if (!w)
        {
            return {};
        }

        const int needed = ::WideCharToMultiByte(
            CP_UTF8, 0, w, -1, nullptr, 0, nullptr, nullptr
        );
        if (needed <= 1)
        {
            return {};
        }

        std::string result(needed - 1, '\0');
        ::WideCharToMultiByte(
            CP_UTF8, 0, w, -1, result.data(), needed, nullptr, nullptr
        );
        return result;
    }

    inline std::wstring utf8_to_wide(std::string_view utf8)
    {
        if (utf8.empty())
        {
            return {};
        }

        const int needed = ::MultiByteToWideChar(
            CP_UTF8, 0, utf8.data(), -1, nullptr, 0
        );
        if (needed <= 1)
        {
            return {};
        }

        std::wstring result(needed - 1, L'\0');
        ::MultiByteToWideChar(
            CP_UTF8, 0, utf8.data(), -1, result.data(), needed
        );
        return result;
    }
}
