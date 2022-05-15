#pragma once

#define RMU_UTIL
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "rmu/Include/RMUResult.h"
#include "rmu/Include/RMUDLL.h"
#include "rmu/Include/RMUError.hpp"
#include "rmu/SourceBase/MaxMorphFiles/RMStockManager.h"
#include "rmu/RMUDLL/RMUUtilDLL.h"
#include "rmu/SourceBase/MaxMorphFiles/RMUnit.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>

#ifdef WIN32
#ifdef RMUDLL_EXPORTS
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport)
#endif
#else
#define DECLSPEC
#endif


class Word {
private:
    const std::string ANALYZE_BEFORE = "<analyze form=\\\"";
    const std::string ANALYZE_AFTER = "\">\n\n";
    IRMUnit* RMUnit = IRMUnit::Create();
    std::string x, x1, x2;
    DECLSPEC void step_4();
    DECLSPEC void step_5();
    DECLSPEC void step_6();
    DECLSPEC void step_7();
    DECLSPEC void step_3();
    bool rec;
    DECLSPEC void _error() {
        error = true;
        cout << "cant recognize";
    }


    vector<std::string> numbers = {
        "тысяче",
        "ста", "двухсот", "трехсот", "четырехсот", "пятисот",
        "шестисот", "семисот", "восьмисот", "девятисот",
        "двадцати", "тридцати", "сорока", "пятидесяти",
        "шестижесяти", "семидесяти", "восьмидесяти", "девяноста",
        "одно", "двухъ", "трехъ", "четырехъ", "двух", "трех", "четырех",
        "пяти", "шести", "семи", "восьим", "девяти",
        "десяти", "одиннадцати", "двенадцати", "тринадцати", "четырнадцати",
        "пятнадцати", "шестнадцати", "семнадцати", "восемнадцати", "девятнадцати"};

public:
    std::string result;
    std::string RMUAnswer;
    int stress = 0;
    bool error = false;
    Word(const std::string word, const char* rm) : x(word) {
        RMUnit->LoadFromFile("rm");
        rec = true;
    };
    void analize();
};
