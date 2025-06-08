/*
Font Name Library 1.0.0

Copyright (c) 2009 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/

#ifdef MACOS
#define _LIBCPP_PREFERRED_NAME
#endif

#include "StdAfx.h"
#include "ttf.h"
#ifdef LINUX
#include <iconv.h>
#include <cstdio>
#include <cwctype>
#ifdef MACOS
#include <string>
#endif
#endif

typedef std::string String;
typedef std::wstring WString;
extern String WStringToString(const WString& widestr);

TTF::TTF()
{
	m_pFile = nullptr;
	m_nInfo = 0;
	m_bExternalFilePtr = false;
	m_pMemPtrReader = nullptr;
	m_bBold = false;
	m_bRegular = false;
	m_bItalic = false;
}

TTF::~TTF()
{
	Clear();
}

bool TTF::Parse(const std::wstring& szFilePath)
{
	Clear();
	m_szFilePath = szFilePath;  //TO CHECK
#ifndef LINUX
	m_pFile = _wfopen(szFilePath.c_str(), L"rb");
#else
    //std::string szFilePathS;
    //int ret = WStringToString(szFilePathS, szFilePath);
    std::string szFilePathS = WStringToString(szFilePath);
    m_pFile = fopen((char *)szFilePathS.c_str(), "r");
#endif

	if(NULL==m_pFile)
		return false;

	return ParseOffsetTable();
}

bool TTF::Parse(FILE* pFile, size_t nOffsetFromStart)
{
	Clear();

	if(nullptr==pFile)
		return false;

	m_pFile = pFile;


	m_bExternalFilePtr = true;

	return ParseOffsetTable();
}

bool TTF::ParseOffsetTable()
{
	stOffsetTable OffsetTable{};
#ifndef MACOS
	memset(&OffsetTable, 0, sizeof(OffsetTable));
#endif
	fread(&OffsetTable, sizeof(OffsetTable), 1, m_pFile);
	EndianSwap::Switch(OffsetTable.numTables);

	for(USHORT i=0; i<OffsetTable.numTables; ++i)
	{
		stTableDirectory TableDirectory{};
		memset(&TableDirectory, 0, sizeof(TableDirectory));
		fread(&TableDirectory, sizeof(TableDirectory), 1, m_pFile);
		std::string strTag;
		strTag += TableDirectory.tag[0];
		strTag += TableDirectory.tag[1];
		strTag += TableDirectory.tag[2];
		strTag += TableDirectory.tag[3];

		if(ToLower(strTag)=="name")
		{
			return ParseName(TableDirectory);
		}
	}

	if(m_pFile&&!m_bExternalFilePtr)
		fclose(m_pFile);

	m_pFile = nullptr;

	return false;
}

bool TTF::Parse(BYTE* pFile, size_t size)
{
	return Parse(pFile, size, 0);
}

bool TTF::Parse(BYTE* pFile, size_t size, size_t offset)
{
	Clear();
	if(pFile == nullptr)
		return false;

	m_pMemPtrReader = new MemPtrReader(pFile, size, offset);

	if (m_pMemPtrReader == nullptr)
		return false;

	return ParseOffsetTableMem();
}

bool TTF::ParseOffsetTableMem()
{
	stOffsetTable OffsetTable{};
	memset(&OffsetTable, 0, sizeof(OffsetTable));
	m_pMemPtrReader->Read(&OffsetTable, sizeof(OffsetTable), 1);
	EndianSwap::Switch(OffsetTable.numTables);

	for(USHORT i=0; i<OffsetTable.numTables; ++i)
	{
		stTableDirectory TableDirectory;
		memset(&TableDirectory, 0, sizeof(TableDirectory));
		m_pMemPtrReader->Read(&TableDirectory, sizeof(TableDirectory), 1);
		std::string strTag = "";
		strTag += TableDirectory.tag[0];
		strTag += TableDirectory.tag[1];
		strTag += TableDirectory.tag[2];
		strTag += TableDirectory.tag[3];

		if(ToLower(strTag)=="name")
		{
			return ParseNameMem(TableDirectory);
		}

	}

	if(m_pMemPtrReader)
		delete m_pMemPtrReader;

	m_pMemPtrReader = NULL;

	return false;
}


typedef  unsigned short UTF16;
typedef  unsigned int UTF32;

int is_surrogate(UTF16 uc) { return (uc - 0xd800u) < 2048u; }
int is_high_surrogate(UTF16 uc) { return (uc & 0xfffffc00) == 0xd800; }
int is_low_surrogate(UTF16 uc) { return (uc & 0xfffffc00) == 0xdc00; }

UTF32 surrogate_to_utf32(UTF16 high, UTF16 low) {
    return (high << 10) + low - 0x35fdc00;
}


void convert_utf16_to_utf32(const UTF16 *input,
                            size_t input_size,
                            UTF32 *output)
{
    const UTF16 * const end = input + input_size;
    while (input < end) {
        const UTF16 uc = *input++;
        if (!is_surrogate(uc)) {
            *output++ = uc;
        } else {
            if (is_high_surrogate(uc) && input < end && is_low_surrogate(*input))
                *output++ = surrogate_to_utf32(uc, *input++);
            else
            return;  //ERROR
        }
    }
}

bool TTF::ParseName(stTableDirectory& TableDirectory)
{
	EndianSwap::Switch(TableDirectory.offset);

	fseek(m_pFile, TableDirectory.offset, SEEK_SET);

	stNamingTable NamingTable{};
	memset(&NamingTable, 0, sizeof(NamingTable));
	fread(&NamingTable, sizeof(NamingTable), 1, m_pFile);
	EndianSwap::Switch(NamingTable.NumberOfNameRecords);
	EndianSwap::Switch(NamingTable.OffsetStartOfStringStorage);
	EndianSwap::Switch(NamingTable.FormatSelector);

	std::vector<stNameRecord> vec;
	for(USHORT i=0; i<NamingTable.NumberOfNameRecords; ++i)
	{
		stNameRecord NameRecord{};
		memset(&NameRecord, 0, sizeof(NameRecord));
		fread(&NameRecord, sizeof(NameRecord), 1, m_pFile);
		EndianSwap::Switch(NameRecord.NameID);
		EndianSwap::Switch(NameRecord.EncodingID);
		EndianSwap::Switch(NameRecord.OffsetFromStorageArea);
		EndianSwap::Switch(NameRecord.StringLength);
		EndianSwap::Switch(NameRecord.PlatformID);
		EndianSwap::Switch(NameRecord.LanguageID);
		vec.push_back(NameRecord);
	}

	for(size_t a=0; a<vec.size(); ++a)
	{
		if(vec[a].PlatformID==0||vec[a].PlatformID==3)
		{
			fseek(m_pFile, TableDirectory.offset+NamingTable.OffsetStartOfStringStorage+vec[a].OffsetFromStorageArea, SEEK_SET);
			size_t size = vec[a].StringLength/2+1;
			WCHAR* pwBuf = new WCHAR[size];
			memset(pwBuf,0, size*sizeof(WCHAR));
			fread(pwBuf, vec[a].StringLength, 1, m_pFile);
			EndianSwap::Switch(pwBuf, size);
#ifndef LINUX
			std::wstring szValue = (wchar_t*)pwBuf;
#else
            UTF32 * pwBuf32 = new UTF32[size];
            convert_utf16_to_utf32(pwBuf,  size,  pwBuf32);
            std::wstring szValue = (wchar_t*)pwBuf32;
#endif
			SetNameIDValue(vec[a].NameID, szValue);
			memset(pwBuf,0, size*sizeof(WCHAR));
			delete [] pwBuf;
			pwBuf = NULL;
			if(m_nInfo==0xFF)
				break;
		}
		else
		{
			fseek(m_pFile, TableDirectory.offset+NamingTable.OffsetStartOfStringStorage+vec[a].OffsetFromStorageArea, SEEK_SET);
			size_t size = vec[a].StringLength+1;
			CHAR* pwBuf = new CHAR[size];
			memset(pwBuf,0, size*sizeof(CHAR));
			fread(pwBuf, vec[a].StringLength, 1, m_pFile);
			std::string szValue = pwBuf;
			SetNameIDValue(vec[a].NameID, szValue);
			memset(pwBuf,0, size*sizeof(CHAR));
			delete [] pwBuf;
			pwBuf = NULL;
			if(m_nInfo==0xFF)
				break;
		}
	}

	if(m_pFile&&!m_bExternalFilePtr)
		fclose(m_pFile);

	m_pFile = NULL;

	return true;
}

bool TTF::ParseNameMem(stTableDirectory& TableDirectory)
{
	EndianSwap::Switch(TableDirectory.offset);

	m_pMemPtrReader->Seek(TableDirectory.offset, SEEK_SET);

	stNamingTable NamingTable{};
	memset(&NamingTable, 0, sizeof(NamingTable));
	m_pMemPtrReader->Read(&NamingTable, sizeof(NamingTable), 1);
	EndianSwap::Switch(NamingTable.NumberOfNameRecords);
	EndianSwap::Switch(NamingTable.OffsetStartOfStringStorage);
	EndianSwap::Switch(NamingTable.FormatSelector);

	std::vector<stNameRecord> vec;
	for(USHORT i=0; i<NamingTable.NumberOfNameRecords; ++i)
	{
		stNameRecord NameRecord{};
		memset(&NameRecord, 0, sizeof(NameRecord));
		m_pMemPtrReader->Read(&NameRecord, sizeof(NameRecord), 1);
		EndianSwap::Switch(NameRecord.NameID);
		EndianSwap::Switch(NameRecord.EncodingID);
		EndianSwap::Switch(NameRecord.OffsetFromStorageArea);
		EndianSwap::Switch(NameRecord.StringLength);
		EndianSwap::Switch(NameRecord.PlatformID);
		EndianSwap::Switch(NameRecord.LanguageID);
		vec.push_back(NameRecord);
	}

	for(size_t a=0; a<vec.size(); ++a)
	{
		if(vec[a].PlatformID==0||vec[a].PlatformID==3)
		{
			m_pMemPtrReader->Seek(TableDirectory.offset+NamingTable.OffsetStartOfStringStorage+vec[a].OffsetFromStorageArea, SEEK_SET);
			size_t size = vec[a].StringLength/2+1;
			WCHAR* pwBuf = new WCHAR[size];
			memset(pwBuf,0, size*sizeof(WCHAR));
			m_pMemPtrReader->Read(pwBuf, vec[a].StringLength, 1);
			EndianSwap::Switch(pwBuf, size);
			std::wstring szValue = (wchar_t*)pwBuf;
			SetNameIDValue(vec[a].NameID, szValue);
			memset(pwBuf,0, size*sizeof(WCHAR));
			delete [] pwBuf;
			pwBuf = NULL;
			if(m_nInfo==0xFF)
				break;
		}
		else
		{
			m_pMemPtrReader->Seek(TableDirectory.offset+NamingTable.OffsetStartOfStringStorage+vec[a].OffsetFromStorageArea, SEEK_SET);
			size_t size = vec[a].StringLength+1;
			CHAR* pwBuf = new CHAR[size];
			memset(pwBuf,0, size*sizeof(CHAR));
			m_pMemPtrReader->Read(pwBuf, vec[a].StringLength, 1);
			std::string szValue = (char*)pwBuf;
			SetNameIDValue(vec[a].NameID, szValue);
			memset(pwBuf,0, size*sizeof(CHAR));
			delete [] pwBuf;
			pwBuf = NULL;
			if(m_nInfo==0xFF)
				break;
		}
	}

	if(m_pMemPtrReader)
		delete m_pMemPtrReader;

	m_pMemPtrReader = NULL;

	return true;
}

std::wstring TTF::GetNameID(USHORT uNameID)
{
	switch(uNameID)
	{
	case 0:
		return L"Copyright notice";
	case 1:
		return L"Font Family name";
	case 2:
		return L"Font Subfamily name";
	case 3:
		return L"Unique font identifier";
	case 4:
		return L"Full font name";
	case 5:
		return L"Version string";
	case 6:
		return L"Postscript name for the font";
	case 7:
		return L"Trademark";
	case 8:
		return L"Manufacturer Name";
	case 9:
		return L"Designer";
	case 10:
		return L"Description";
	case 11:
		return L"URL Vendor";
	case 12:
		return L"URL Designer";
	case 13:
		return L"License Description";
	case 14:
		return L"License Info URL";
	case 15:
		return L"Reserved";
	case 16:
		return L"Preferred Family";
	case 17:
		return L"Preferred Subfamily";
	case 18:
		return L"Compatible Full (Macintosh only)";
	case 19:
		return L"Sample text";
	case 20:
		return L"PostScript CID findfont name";
	case 21:
		return L"WWS Family Name";
	case 22:
		return L"WWS Subfamily Name";
	default:
		return L"Unknown Name ID";
	}

	return L"Unknown Name ID";
}

void TTF::SetNameIDValue(USHORT uNameID, const std::wstring& str)
{
	switch(uNameID)
	{
	case 0:
		if(!(m_nInfo & (1<<0)))
			m_szCopyright = str;
		m_nInfo |= (1<<0);
		break;
	case 1:
		if(!(m_nInfo & (1<<1)))
			m_szFontFamilyName = str;
		m_nInfo |= (1<<1);
		break;
	case 2:
		if(!(m_nInfo & (1<<2)))
		{
			m_szFontSubFamilyName = str;
			ParseStyles();
		}
		m_nInfo |= (1<<2);
		break;
	case 3:
		if(!(m_nInfo & (1<<3)))
			m_szFontID = str;
		m_nInfo |= (1<<3);
		break;
	case 4:
		if(!(m_nInfo & (1<<4)))
			m_szFontName = str;
		m_nInfo |= (1<<4);
		break;
	case 5:
		if(!(m_nInfo & (1<<5)))
			m_szVersion = str;
		m_nInfo |= (1<<5);
		break;
	case 6:
		if(!(m_nInfo & (1<<6)))
			m_szPostScriptName = str;
		m_nInfo |= (1<<6);
		break;
	case 7:
		if(!(m_nInfo & (1<<7)))
			m_szTrademark = str;
		m_nInfo |= (1<<7);
		break;
	default: break;
	}
}

void TTF::SetNameIDValue(USHORT uNameID, const std::string& str)
{
	SetNameIDValue(uNameID, NarrowToWide(str));
}

typedef std::string String;
typedef std::wstring WString;

#ifdef LINUX
#include <iconv.h>

#include <iostream>
#include <set>
#include <string>
#include <locale>

#include <unistd.h>
#include <termios.h>
#endif

/*
WString TTF::NarrowToWide__(const String& str)
{
    if (str.empty())
    {
        return WString();
    }

    size_t pos;
    size_t begin = 0;
    WString ret;
#ifndef LINUX
    int size = 0;
        pos = str.find(static_cast<char>(0), begin);
        while (pos != std::string::npos) {
            std::string segment = std::string(&str[begin], pos - begin);
            std::wstring converted = std::wstring(segment.size() + 1, 0);
            size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.length());
            converted.resize(size);
            ret.append(converted);
            ret.append({ 0 });
            begin = pos + 1;
            pos = str.find(static_cast<char>(0), begin);
        }
        if (begin < str.length()) {
            std::string segment = std::string(&str[begin], str.length() - begin);
            std::wstring converted = std::wstring(segment.size() + 1, 0);
            size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, segment.c_str(), segment.size(), &converted[0], converted.length());
            converted.resize(size);
            ret.append(converted);
        }

#else
    size_t size;
        pos = str.find(static_cast<char>(0), begin);
        while (pos != String::npos)
        {
            String segment = String(&str[begin], pos - begin);
            WString converted = WString(segment.size(), 0);
            size = mbstowcs(&converted[0], &segment[0], converted.size());
            converted.resize(size);
            ret.append(converted);
            ret.append({ 0 });
            begin = pos + 1;
            pos = str.find(static_cast<char>(0), begin);
        }
        if (begin < str.length())
        {
            String segment = String(&str[begin], str.length() - begin);
            WString converted = WString(segment.size(), 0);
            size = mbstowcs(&converted[0], &segment[0], converted.size());
            converted.resize(size);
            ret.append(converted);
        }
#endif
    return ret;
}
*/
std::wstring TTF::NarrowToWide(const std::string& str)
{
    if (str.empty())
    {
        return WString();
    }

    ////printf(str.c_str());

    if (str.length()>64) return WString();

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
        char converted_[520];
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

        char segment_[512];
        char *segment_ptr;
        char converted_[1025]="";
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

std::wstring TTF::NarrowToWide_(const std::string& szNarrow)
{
int size;
int size2;
#ifndef LINUX
	 size = MultiByteToWideChar(
		CP_ACP,           // code page
		0,    // character-type options
		szNarrow.c_str(), // string to map
		szNarrow.size(),  // number of bytes in string
		NULL,  // wide-character buffer
		0      // size of buffer
		);
#else
    iconv_t cd1;
    cd1 = iconv_open("ISO8859-2", "UTF-8");  //WCHAR_T
    //cd1 = iconv_open("WCHAR_T", "UTF-8");

    char szNarrow_[255];
    char *szNarrow_ptr;
    char converted_[520*sizeof(WCHAR)];
    char *converted_ptr;
    size_t converted_size;
    size_t szNarrow_size;
    strcpy(szNarrow_,szNarrow.c_str());
    szNarrow_ptr=szNarrow_;
    converted_ptr=converted_;
    szNarrow_size=szNarrow.size();
    converted_size=2*(szNarrow_size+1);

    size_t size_c = iconv(cd1, &szNarrow_ptr, &szNarrow_size,&converted_ptr, &converted_size);
    //int size = iconv(cd1, (char**)szNarrow.c_str(), (size_t*)szNarrow.size(), NULL, 0);
    iconv_close(cd1);
    size=converted_size;
#endif

	size++;

	WCHAR* pBuf = new WCHAR[size];
	memset(pBuf,0,size*sizeof(WCHAR));

#ifndef LINUX
	size2 = MultiByteToWideChar(
		CP_ACP,           // code page
		0,    // character-type options
		szNarrow.c_str(), // string to map
		szNarrow.size(),  // number of bytes in string
		pBuf,  // wide-character buffer
		size      // size of buffer
		);
#else

    iconv_t cd2;
    cd2 = iconv_open("ISO8859-2", "UTF-8");
    //cd2 = iconv_open("WCHAR_T", "UTF-8");

    strcpy(szNarrow_,szNarrow.c_str());
    szNarrow_ptr=szNarrow_;
    converted_ptr=converted_;
    szNarrow_size=szNarrow.size();
    converted_size=2*(szNarrow_size+1);

    size_t size2_c = iconv(cd2, &szNarrow_ptr, &szNarrow_size,&converted_ptr, &converted_size);
    //int size2 = iconv(cd2, (char **)szNarrow.c_str(), (size_t*)szNarrow.size(), (char **)&pBuf, (size_t*)&size);

    iconv_close(cd2);

    size2=converted_size;

    memcpy(pBuf,converted_, converted_size);

#endif
	std::wstring szWide;
	if(size2>0)
		szWide = (wchar_t*)pBuf;

	memset(pBuf,0,size*sizeof(WCHAR));
	delete [] pBuf;
	pBuf = NULL;

	return szWide;
}

std::string TTF::WideToNarrow(const std::wstring& szWide)
{
	BOOL bUsedDefaultChar=FALSE;
    int size;
    int size2;
#ifndef LINUX
	  size = WideCharToMultiByte(
		CP_ACP,           // code page
		0,    // character-type options
		szWide.c_str(), // string to map
		szWide.size(),  // number of bytes in string
		NULL,  // character buffer
		0,      // size of buffer
		NULL,
		&bUsedDefaultChar);
#else
	iconv_t cd1 = iconv_open("UTF-8", "ISO8859-2");
    //cd1 = iconv_open("UTF-8", "WCHAR_T");

    char szWide_[255];
    char *szWide_ptr;
    char converted_[255];
    char *converted_ptr;
    size_t converted_size;
    size_t szWide_size;
    strcpy(szWide_,(char*)szWide.c_str());
    szWide_ptr=szWide_;
    converted_ptr=converted_;
    szWide_size=szWide.size();
    converted_size=2*(szWide_size+1);

    size_t size_c = iconv(cd1, &szWide_ptr, &szWide_size,&converted_ptr, &converted_size);
    //int size = iconv(cd1, (char**)szWide.c_str(), (size_t*)szWide.size(), NULL, 0);
    iconv_close(cd1);
    size=converted_size;

#endif

	size++;

	CHAR* pBuf = new CHAR[size];
	memset(pBuf,0,size*sizeof(CHAR));

	bUsedDefaultChar=FALSE;
#ifndef LINUX
	size2 = WideCharToMultiByte(
		CP_ACP,           // code page
		0,    // character-type options
		szWide.c_str(), // string to map
		szWide.size(),  // number of bytes in string
		pBuf,  // character buffer
		size,      // size of buffer
		NULL,
		&bUsedDefaultChar);
#else
	iconv_t cd2 = iconv_open("UTF-8", "ISO8859-2");
    //cd2 = iconv_open("UTF-8", "WCHAR_T");

    size_t size2_c = iconv(cd2, &szWide_ptr, &szWide_size,&converted_ptr, &converted_size);
    //size_t size2_c = iconv(cd2, (char **)szWide.c_str(), (size_t*)szWide.size(), (char **)&pBuf, (size_t*)&size);

    iconv_close(cd2);

    size2 = converted_size;
    memcpy(pBuf,converted_, converted_size);

#endif

	std::string szNarrow;
	if(size2>0)
		szNarrow = pBuf;

	memset(pBuf,0,size*sizeof(CHAR));
	delete [] pBuf;
	pBuf = NULL;

	return szNarrow;
}


void TTF::Clear()
{
	if(m_pFile&&!m_bExternalFilePtr)
		fclose(m_pFile);

	if(m_pMemPtrReader)
		delete m_pMemPtrReader;

	m_pFile = nullptr;
	m_pMemPtrReader = nullptr;
	m_szFontName = L"";
	m_szFilePath = L"";
	m_szCopyright = L"";
	m_szFontFamilyName = L"";
	m_szFontSubFamilyName = L"";
	m_szFontID = L"";
	m_szVersion = L"";
	m_szPostScriptName = L"";
	m_szTrademark = L"";
	m_nInfo = 0;
	m_bExternalFilePtr = false;
	m_bBold = false;
	m_bRegular = false;
	m_bItalic = false;
}


void TTF::ParseStyles()
{
	m_bBold = false;
	if(ToLower(m_szFontSubFamilyName).find(L"bold")!=-1)
		m_bBold = true;

	m_bItalic = false;
	if(ToLower(m_szFontSubFamilyName).find(L"italic")!=-1)
		m_bItalic = true;

	m_bRegular = false;
	if(ToLower(m_szFontSubFamilyName).find(L"regular")!=-1)
		m_bRegular = true;
	else if(ToLower(m_szFontSubFamilyName).find(L"normal")!=-1)
		m_bRegular = true;
	else if(ToLower(m_szFontSubFamilyName).find(L"standard")!=-1)
		m_bRegular = true;
}

bool TTF::IsBold()
{
	return m_bBold;
}

bool TTF::IsItalic()
{
	return m_bItalic;
}

bool TTF::IsRegular()
{
	return m_bRegular;
}

std::wstring TTF::ToLower(const std::wstring& szNormal)
{
	std::wstring szLower;
	for(size_t i=0; i<szNormal.size() ; ++i)
	{
		szLower += (wchar_t)(towlower(szNormal.at(i)));
	}

	return szLower;
}

std::string TTF::ToLower(const std::string& szNormal)
{
	std::string szLower;
	for(size_t i=0; i<szNormal.size() ; ++i)
	{
		szLower += (char)(towlower(szNormal.at(i)));
	}

	return szLower;
}
