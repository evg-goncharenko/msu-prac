// TestRMU.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <list>
#include <string>
using namespace std;

#ifndef NDEBUG
#define RMU_UTIL
#endif
#include "../RMUDLL/RMUUtilDLL.h"

#include "RMUDLL.h"

#include "RMUResult.h"

#include "convert.h"

/////////////////////////////////////////////////////
//
//	 helper class-wrapper aroung low-level morph dll
/////////////////////////////////////////////////////
class RMUDLL {
    RMUHandle _hRmu;

public:
    RMUDLL() {
        if (RMUInit(&_hRmu) != RMU_OK)
            throw "Error initialising RMU.DLL";
    }
    ~RMUDLL() {
        RMUFree(_hRmu);
    }
    string Analyse(const string& word) {
        RMUAnswer hAns;
        if (RMUGetAnswer(_hRmu, word.c_str(), &hAns) != RMU_OK)
            throw "RMU.DLL analysis error";
        int len = RMUGetXML(hAns, NULL, 0, 0);
        if (len <= 0) {
            RMUFreeAnswer(hAns);
            throw "RMU.DLL error getting xml buffer";
        }
        char* pAnswer = new char[len];
        if (RMUGetXML(hAns, pAnswer, len, 0) != RMU_OK) {
            RMUFreeAnswer(hAns);
            throw "RMU.DLL error getting xml";
        }
        string result(pAnswer);
        RMUFreeAnswer(hAns);
        delete[] pAnswer;
        return result;
    }
    void Analyse(const string& word, string& xml_answer, list<string>& translated_answers) {
        RMUAnswer hAns;
        if (RMUGetAnswer(_hRmu, word.c_str(), &hAns) != RMU_OK)
            throw "RMU.DLL analysis error";
        int len = RMUGetXML(hAns, NULL, 0, 0);
        if (len <= 0) {
            RMUFreeAnswer(hAns);
            throw "RMU.DLL error getting xml buffer";
        }
        char* pAnswer = new char[len];
        if (RMUGetXML(hAns, pAnswer, len, 0) != RMU_OK) {
            RMUFreeAnswer(hAns);
            throw "RMU.DLL error getting xml";
        }
        string xml(pAnswer);
        RMUFreeAnswer(hAns);
        delete[] pAnswer;
        xml_answer.swap(xml);
        vector<RMUResult> resVec;
        RMUResultGet(xml_answer, resVec);
        auto res = resVec.begin();
        while (res != resVec.end()) {
            translated_answers.push_back(res->GetTranslatedAnswer());
            ++res;
        }
    }
#ifdef RMU_UTIL
    string AnalyseQuery(const string& query) {
        RMUAnswer hAns;
        if (RMUProcessQuery(_hRmu, query.c_str(), &hAns) != RMU_OK)
            throw "RMU.DLL analysis error";
        int len = RMUGetXML(hAns, NULL, 0, 0);
        if (len <= 0) {
            RMUFreeAnswer(hAns);
            throw "RMU.DLL error getting xml buffer";
        }
        char* pAnswer = new char[len];
        if (RMUGetXML(hAns, pAnswer, len, 0) != RMU_OK) {
            RMUFreeAnswer(hAns);
            throw "RMU.DLL error getting xml";
        }
        string result(pAnswer);
        delete[] pAnswer;
        RMUFreeAnswer(hAns);
        return result;
    }
#endif
};

// main demo

static void print_answer(const string& answer) {
    cout << TranslateToConsole(answer) << endl;
}

int main(int argc, char* argv[]) {
    char warning[] = "Загрузка файлов морфологии. Ждите...";
    cout << TranslateToConsole(warning) << endl;
    try {
        RMUDLL morph;
        char word[1024];
        for (;;) {
            try {
                cout << TranslateToConsole(string("Введите словоформу или quit>")) << endl;
                cin.getline(word, sizeof word);
                if (strcmp(word, "quit") == 0) break;
                string result;
                if (word[0] != '<') {
                    //result = morph.Analyse(TranslateFromConsole(word));
                    list<string> ans_list;
                    morph.Analyse(TranslateFromConsole(word), result, ans_list);
                    if (ans_list.size() == 0)
                        cout << TranslateToConsole(string("Словоформа отсутствует в словаре")) << endl;
                    else
                        cout << TranslateToConsole(string("Варианты анализа:")) << endl;
                    for_each(ans_list.begin(), ans_list.end(), print_answer);
                }
#if defined(RMU_UTIL)
                else
                    result = morph.AnalyseQuery(TranslateFromConsole(word));
#endif
                cout << TranslateToConsole(result) << endl;
            } catch (const char* msg) {
                cout << TranslateToConsole(string("Ошибка обработки: ") + string(msg)) << endl;
            }
        }
    } catch (const char* msg) {
        cout << TranslateToConsole(string("Ошибка инициализации: ") + string(msg)) << endl;
        return 1;
    }
    return 0;
}
