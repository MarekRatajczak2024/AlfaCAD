#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <set>
#include <string>
#include <locale>

#ifndef LINUX
#include <Windows.h>
#include <conio.h>
#else
////#include <curses.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <stdint.h>
#endif

////#include "leak_detector_cpp.hpp"

#define DLLCALL STDCALL
#define DLLIMPORT _declspec(dllimport)
#define DLLEXPORT _declspec(dllexport)
#define DLLPRIVATE
#define NOMINMAX

typedef std::string String;
typedef std::wstring WString;

#define EMPTY_STRING u8""s
#define EMPTY_WSTRING L""s

#define Utf8Char unsigned char

using namespace std::literals::string_literals;

extern "C"
{
    void utf8Upper(unsigned char* text);
    void utf8Lower(unsigned char* text);  //MACOS

    extern Utf8Char* Utf8StrMakeUprUtf8Str(const Utf8Char* pUtf8);
    extern Utf8Char* Utf8StrMakeLwrUtf8Str(const Utf8Char* pUtf8);
}

//void utf8Lower(unsigned char* text);  //MACOS

class Strings
{
public:

    static String WideStringToString_1(const WString& wstr)
    {
        if (wstr.empty())
        {
            return String();
        }
        size_t pos;
        size_t begin = 0;
        String ret;

        int size;
        pos = wstr.find(static_cast<wchar_t>(0), begin);
        while (pos != WString::npos && begin < wstr.length())
        {
            WString segment = WString(&wstr[begin], pos - begin);
#ifndef LINUX
            size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), NULL, 0, NULL, NULL);
#else
            iconv_t cd1;
            cd1 = iconv_open("UTF-8", "ISO8859-2");

            int size_c = iconv(cd1, (char **)(segment.c_str()), (size_t*)(segment.length()), NULL, (size_t*)&size);
            iconv_close(cd1);
#endif

            String converted = String(size, 0);
#ifndef LINUX
            WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.size(), NULL, NULL);
#else
            iconv_t cd2;
            cd1 = iconv_open("UTF-8", "ISO8859-2");

            int size2 = iconv(cd2, (char**)(segment.c_str()), (size_t*)segment.length(), (char **)converted.c_str(), (size_t*)&size);
            iconv_close(cd2);
#endif
            ret.append(converted);
            ret.append({ 0 });
            begin = pos + 1;
            pos = wstr.find(static_cast<wchar_t>(0), begin);
        }
        if (begin <= wstr.length())
        {
            WString segment = WString(&wstr[begin], wstr.length() - begin);
#ifndef LINUX
            size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), NULL, 0, NULL, NULL);
#else
            iconv_t cd3;
            cd3 = iconv_open("UTF-8", "ISO8859-2");

            int size3_c = iconv(cd3, (char**)segment.c_str(), (size_t*)segment.length(), NULL, (size_t*)&size);
            iconv_close(cd3);
#endif
            String converted = String(size, 0);
#ifndef LINUX
            WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.size(), NULL, NULL);
#else
            iconv_t cd4;
            cd4 = iconv_open("UTF-8", "ISO8859-2");

            int size4 = iconv(cd4, (char**)segment[0], (size_t*)segment.length(), (char **)converted.c_str(), (size_t*)&size);
            iconv_close(cd4);
#endif
            ret.append(converted);
        }

        return ret;
    }

    static String WideStringToString_(const WString& wstr)
    {
        if (wstr.empty())
        {
            return String();
        }
        size_t pos;
        size_t begin = 0;
        String ret;

        int size;
        pos = wstr.find(static_cast<wchar_t>(0), begin);
        while (pos != WString::npos && begin < wstr.length())
        {
            WString segment = WString(&wstr[begin], pos - begin);
#ifndef LINUX
            size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), NULL, 0, NULL, NULL);

            String converted = String(size, 0);

            size_t size2 = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.size(), NULL, NULL);
#else
            char segment_[255];
            char *segment_ptr;
            char converted_[255];
            char *converted_ptr;
            size_t converted_size;
            size_t segment_size;

            const wchar_t *segment_ptr_ = segment.c_str();
            segment_ptr=(char*)segment_ptr_;

            converted_ptr=converted_;
            segment_size=segment.length();
            converted_size=2*(segment_size+1);

            String converted = {""};

            iconv_t cd2;
            cd2 = iconv_open("UTF-8", "ISO8859-2");

            size_t size2 = iconv(cd2, &segment_ptr, &segment_size,&converted_ptr, &converted_size);
            //int size2 = iconv(cd2, (char**)segment[0], (size_t*)segment.size(), (char **)&converted[0], (size_t*)converted.size());
            iconv_close(cd2);
            converted.append(converted_, converted_size);
#endif
            ret.append(converted);
            ret.append({ 0 });
            begin = pos + 1;
            pos = wstr.find(static_cast<wchar_t>(0), begin);
        }
        if (begin <= wstr.length())
        {
            WString segment = WString(&wstr[begin], wstr.length() - begin);
#ifndef LINUX
            size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), NULL, 0, NULL, NULL);

            String converted = String(size, 0);

            WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.size(), NULL, NULL);
            ret.append(converted);
#else

            char segment_[255];
            char *segment_ptr;
            char converted_[255];
            char *converted_ptr;
            size_t converted_size;
            size_t segment_size;
            strcpy(segment_,(char*)segment.c_str());
            segment_ptr=segment_;
            converted_ptr=converted_;
            segment_size=segment.length();
            converted_size=2*(segment_size+1);

            String converted = {""};

            iconv_t cd4;
            cd4 = iconv_open("UTF-8", "ISO8859-2");
            //cd4 = iconv_open("UTF-8", "WCHAR_T");

            size_t size4 = iconv(cd4, &segment_ptr, &segment_size,&converted_ptr, &converted_size);
            //int size4 = iconv(cd4, (char**)segment[0], (size_t*)segment.size(), (char **)&converted[0], (size_t*)converted.size());
            iconv_close(cd4);
            ret.append(converted_, converted_size);
#endif
        }

        return ret;
    }

    static String WideStringToString(const WString& wstr)
    {
        if (wstr.empty())
        {
            return String();
        }
        size_t pos;
        size_t begin = 0;
        String ret;

#ifndef LINUX
        int size;
        pos = wstr.find(static_cast<wchar_t>(0), begin);
        while (pos != WString::npos && begin < wstr.length())
        {
            WString segment = WString(&wstr[begin], pos - begin);
            size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), NULL, 0, NULL, NULL);
            String converted = String(size, 0);
            WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.size(), NULL, NULL);
            ret.append(converted);
            ret.append({ 0 });
            begin = pos + 1;
            pos = wstr.find(static_cast<wchar_t>(0), begin);
        }
        if (begin <= wstr.length())
        {
            WString segment = WString(&wstr[begin], wstr.length() - begin);
            size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), NULL, 0, NULL, NULL);
            String converted = String(size, 0);
            WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.size(), NULL, NULL);
            ret.append(converted);
        }
#else
        size_t size;
        pos = wstr.find(static_cast<wchar_t>(0), begin);
        while (pos != WString::npos && begin < wstr.length())
        {
            WString segment = WString(&wstr[begin], pos - begin);
            size = wcstombs(nullptr, segment.c_str(), 0);
            String converted = String(size, 0);
            wcstombs(&converted[0], segment.c_str(), converted.size());
            ret.append(converted);
            ret.append({ 0 });
            begin = pos + 1;
            pos = wstr.find(static_cast<wchar_t>(0), begin);
        }
        if (begin <= wstr.length())
        {
            WString segment = WString(&wstr[begin], wstr.length() - begin);
            size = wcstombs(nullptr, segment.c_str(), 0);
            if (size>INT16_MAX)
                return String();
            String converted = String(size, 0);
            wcstombs(&converted[0], segment.c_str(), converted.size());
            ret.append(converted);
        }

#endif
        return ret;
    }

    static WString StringToWideString(const String& str)
    {
        if (str.empty())
        {
            return WString();
        }

        size_t pos = 0;
        size_t begin = 0;
        WString ret = {L""};

        int size = 0;
        pos = str.find(static_cast<char>(0), begin);
        while (pos != std::string::npos)
        {
            std::string segment = std::string(&str[begin], pos - begin);
            //std::wstring converted = std::wstring(segment.size() + 1, 0);
            std::wstring converted {L""};
#ifndef LINUX
            size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.length());
#else
            iconv_t cd;

            cd = iconv_open("ISO8859-2", "UTF-8");
            //cd = iconv_open("WCHAR_T", "UTF-8");

            char segment_[255];
            char *segment_ptr;
            char converted_[255];
            char *converted_ptr;
            size_t converted_size;
            size_t segment_size;
            strcpy(segment_,segment.c_str());
            //segment_ptr=segment_;
            segment_ptr=(char*)segment.c_str();
            converted_ptr=converted_;
            segment_size=segment.length();
            converted_size=2*(segment_size+1);

            size_t size_c = iconv(cd, &segment_ptr, &segment_size,&converted_ptr, &converted_size);
            iconv_close(cd);
            size=converted_size;

            converted.append((wchar_t *)converted_, converted_size);
#endif
            converted.resize(size);
            ret.append(converted);
            ret.append({ 0 });
            begin = pos + 1;
            pos = str.find(static_cast<char>(0), begin);
        }
        if (begin < str.length())
        {
            std::string segment = std::string(&str[begin], str.length() - begin);
            //std::wstring converted = std::wstring(segment.size() + 1, 0);

#ifndef LINUX
            std::wstring converted = std::wstring(2*(segment.length() + 1), 0);
            size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, segment.c_str(), segment.size(), &converted[0], converted.length());
#else
            //std::wstring converted{L""};
            iconv_t cd1;
            cd1 = iconv_open("ISO8859-2", "UTF-8");
            //cd1 = iconv_open("WCHAR_T", "UTF-8");

            char segment_[255];
            char *segment_ptr;
            char converted_[255]="";
            char *converted_ptr;
            size_t converted_size;
            size_t segment_size;
            strcpy(segment_,segment.c_str());
            //segment_ptr=segment_;
            segment_ptr=(char*)segment.c_str();
            converted_ptr=converted_;
            segment_size=segment.length();
            converted_size=2*(segment_size+1);

            size_t size_c = iconv(cd1, &segment_ptr, &segment_size,&converted_ptr, &converted_size);

            size = converted_size;

            iconv_close(cd1);

            std::wstring converted (converted_, converted_ + converted_size);

#endif
            converted.resize(size);
            ret.append(converted);
        }

        return ret;
    }

    static WString ToUpper(const WString& data)
    {
        WString result = data;
        auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());

        f.toupper(&result[0], &result[0] + result.size());
        return result;
    }

    static String ToUpper(const String& data)
    {
        return WideStringToString(ToUpper(StringToWideString(data)));
    }

    static WString ToLower(const WString& data)
    {
        WString result = data;
        auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());
        f.tolower(&result[0], &result[0] + result.size());
        return result;
    }

    static String ToLower(const String& data)
    {
        return WideStringToString(ToLower(StringToWideString(data)));
    }
};

String convertToString(char* a)
{
    String s(a);

    // we cannot use this technique again
    // to store something in s
    // because we use constructors
    // which are only called
    // when the string is declared.

    // Remove commented portion
    // to see for yourself

    /*
    char demo[] = "gfg";
    s(demo); // compilation error
    */

    return s;
}


String WStringToString(const WString& widestr) {
    std::string sstr = Strings::WideStringToString(widestr);
    return sstr;
}

void utf8Upper_(char* text)
{

    String s_text = convertToString(text);
    String s_text_u = Strings::ToUpper(s_text);

    strcpy(text, s_text_u.c_str());
}

void utf8Lower_(char* text)
{

    String s_text = convertToString(text);
    String s_text_u = Strings::ToLower(s_text);

    strcpy(text, s_text_u.c_str());
}

void utf8Upper(unsigned char* text)
{
    if (unsigned char* text_ = Utf8StrMakeUprUtf8Str(text); text_ != nullptr)
    {
        strcpy((char*)text, (char*)text_);
        free(text_);
    }
}

void utf8Lower(unsigned char* text)
{
    if (unsigned char* text_ = Utf8StrMakeLwrUtf8Str(text); text_ != nullptr)
    {
        strcpy((char*)text, (char*)text_);
        free(text_);
    }
}

/*

int main()
{

    std::locale::global(std::locale(u8"en_US.UTF8")); //Required for Linux. Error when run without set unicode locale. This need to be investigated and fixed.

    String dataStr = u8"Zoë Saldaña played in La maldición del padre Cardona. ëèñ αω óóChloë";
    
    dataStr = Strings::ToLower(dataStr);
    
    //Another examples
    WString string0(L"hello there είναι απλά ένα κείμενο χωρίς");
    WString string1(L"hallo Привет");

    string0 = Strings::ToUpper(string0);
    string1 = Strings::ToUpper(string1);
    

    return 0;
}

*/