#include "anz.hpp"
DECLSPEC

void Word::step_3() {
    // std::cout << " step3 x1=" << x1 << " x2=" << x2 << std::endl;
    std::ifstream data_pref;
    std::string pref;
    data_pref.open("prefixes.dat");
    data_pref >> pref;
    auto t = false;
    do {
        t = !pref.compare(x.substr(0, pref.length()));
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
    // std::cout << " step4 x1=" << x1 << " x2=" << x2 << std::endl;
    std::string pref;
    auto i = numbers.begin();
    auto t = false;
    pref = *i;
    do {
        t = !pref.compare(x.substr(0, pref.length()));
        if (t) break;
        i++;
        if (i != numbers.end()) pref = *i;
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
    std::string result;
    // std::cout << " step5 x1=" << x1 << " x2=" << x2 << std::endl;
    std::string request("<analyze form=\"");
    request.append(x2);
    request.append("\">\n\n");
    RMUAnswer = RMUnit->ProcessQuery(request.c_str());

    if ((RMUAnswer.find("unrecognized")) == std::string::npos) {
        step_6();
    } else {
        step_7();
    }
}

void Word::step_6() {
    // std::cout << " step6 x1=" << x1 << " x2=" << x2 << std::endl;
    std::string st = RMUAnswer.substr(RMUAnswer.find("lexem=\"") + 7);
    st = st.substr(0, st.find('\"'));
    stress = x1.size() / 2;
    if (x1.find('-') != std::string::npos) ++stress;
    x1.append(st);
    result = x1;
    st = RMUAnswer.substr(RMUAnswer.find("stress=") + 7);
    st = st.substr(0, st.find(' '));
    stress += std::stoi(st);
}

void Word::step_7() {
    // std::cout << " step7 x1=" << x1 << " x2=" << x2 << std::endl;
    if (rec) {
        x = x2;
        rec = false;
        analize();
    } else
        _error();
}

void Word::analize() {
    // std::cout << "\n step2 x1=" << x1 << " x2=" << x2 << std::endl;
    auto t = x.find('-');
    if (t != std::string::npos) {
        x1 = x.substr(0, t + 1);
        x2 = x.substr(t + 1);
        step_5();
    } else {
        step_3();
    }
}

//*! Binary generation:
// int main() {
//     std::string answer;
//     try {
//         RMUEncode(
//             "rm"
//         );
//     } catch (Xception* pErr) {
//         answer = pErr->GetNotification();
//         std::cout << "FINAL_LOG" << answer << std::endl;
//     }
// }
