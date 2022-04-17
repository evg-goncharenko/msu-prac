#include <cstring>
#include <iostream>
#include <string>
#include "stdafx.h"
using namespace std;
#include "convert.h"

#ifdef WIN32
#define strdup _strdup
#else
#include <cstring>
#endif

#ifdef WIN32
string TranslateToConsole(const string& message) {
    const char* msg = message.c_str();
    char* transl = strdup(msg);
    CharToOem(msg, transl);
    string result(transl);
    free(transl);
    return result;
}

string TranslateFromConsole(const string& message) {
    const char* msg = message.c_str();
    char* transl = strdup(msg);
    OemToChar(msg, transl);
    string result(transl);
    free(transl);
    return result;
}
#else
#include <iconv.h>

class CPConverter {
    iconv_t _UTF8ToWin;
    iconv_t _WinToUTF8;

public:
    CPConverter() {
        _UTF8ToWin = iconv_open("CP1251", "UTF-8");
        if (_UTF8ToWin == (iconv_t)-1)
            throw "Error opening converter to UTF-8";
        _WinToUTF8 = iconv_open("UTF-8", "CP1251");
        if (_WinToUTF8 == (iconv_t)-1)
            throw "Error opening converter to UTF-8";
    }
    ~CPConverter() {
        iconv_close(_UTF8ToWin);
        iconv_close(_WinToUTF8);
    }
    string Win2UTF(const char* msg) {
        string result;
        char* inbuf = (char*)msg;
        size_t in = strlen(msg) + 1;
        size_t len = 2 * in;
        char* buffer = (char*)malloc(len);
        char* out = buffer;
        memset(buffer, 0, len);
        size_t n = iconv(_WinToUTF8, &inbuf, &in, &out, &len);
        if (n == (size_t)-1)
            throw "Conversion error";
        result = buffer;
        free(buffer);
        return result;
    }
    string UTF2Win(const char* msg) {
        string result;
        char* inbuf = (char*)msg;
        size_t in = strlen(msg) + 1;
        size_t len = in;
        char* buffer = (char*)malloc(len);
        char* out = buffer;
        memset(buffer, 0, len);
        size_t n = iconv(_UTF8ToWin, &inbuf, &in, &out, &len);
        if (n == (size_t)-1)
            throw "Conversion error";
        result = buffer;
        free(buffer);
        return result;
    }
    bool check() {
        return !(_UTF8ToWin == (iconv_t)-1 || _WinToUTF8 == (iconv_t)-1);
    }
};

static CPConverter conv;

string TranslateToConsole(const string& message) {
    return conv.Win2UTF(message.c_str());
}

string TranslateFromConsole(const string& message) {
    return conv.UTF2Win(message.c_str());
}
#endif
