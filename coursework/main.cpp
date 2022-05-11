#pragma once
#include "rmu/Include/RMUDLL.h"
#include "rmu/Include/RMUResult.h"
#include "rmu/SourceBase/MaxMorphFiles/RMUnit.h"
#include <fstream>
#include "rmu/Include/RMUError.hpp"
#include <string>
#include <cstdio>
#include <vector>


class Word {
private:
    const std::string ANALYZE_BEFORE = "<analyze form=\\\"";
    const std::string ANALYZE_AFTER = "\\\">\n\n";
    IRMUnit* RMUnit = IRMUnit::Create();
    std::string x, x1, x2;
    void step_3();
    void step_4();
    void step_5();
    void step_6();
    void step_7();
    bool rec;
    void _error() {
        error = true;
        throw "Fuck";
    }

    std::string RMUAnswer;

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
    bool error = false;
    Word(const std::string word, const char* rm) : x(word) {
        RMUnit->LoadFromFile(rm);
        rec = true;
    };
    ~Word();
    void analize();
};

void Word::step_3() {
    std::ifstream data_pref;
    std::string pref;
    data_pref.open("prefixes.dat");
    data_pref >> pref;
    auto t = false;
    do {
        t = pref.compare(x.substr(0, pref.length()));
        if (t) break;
        data_pref >> pref;
    } while (!data_pref.eof());
    data_pref.close();
    if (t) {
        x1 = pref;
        x2 = x.substr(pref.size());
        step_5();
    } else {
        step_4();
    }
}

void Word::step_4() {
    std::string pref;
    auto i = numbers.begin();
    auto t = false;
    pref = *i;
    do {
        t = pref.compare(x.substr(0, pref.length()));
        if (t) break;
        i++;
        pref = *i;
    } while (i != numbers.end());

    if (t) {
        x1 = pref;
        x2 = x.substr(pref.size());
        step_5();
    } else {
        _error();
    }
}

void Word::step_5() {
    std::string request("<analyze form=\"");
    request.append(x2);
    request.append(ANALYZE_AFTER);
    try {
        RMUAnswer = RMUnit->ProcessQuery(request.c_str());
    } catch (...) {
        if (rec) step_7(); else _error();
    }
}

void Word::step_6() {

}

void Word::step_7() {
    x = x2;
    rec = false;
    analize();
}

void Word::analize() {
    auto t = x.find('-');
    if (t != std::string::npos) {
        x1 = x.substr(0, t - 1);
        x2 = x.substr(t + 1);
        step_5();
    } else {
        step_3();
    }
}

int main() {
}