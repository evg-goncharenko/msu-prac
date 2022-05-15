#include <iostream>
#include "anz.hpp"

int main() {
    std::string answer;
    try {
        Word w("семибоярщина", "rm.stem.data");
        w.analize();
        std::cout << "Result: " << w.result << std::endl;
        std::cout << "Stressed letter: " << w.stress << std::endl;
        std::cout << "RMUAnswer: " << w.RMUAnswer << std::endl;
        Word w2("черно-белое", "rm.stem.data");
        w2.analize();
        std::cout << "Result: " << w2.result << std::endl;
        std::cout << "Stressed letter: " << w2.stress << std::endl;
        std::cout << "RMUAnswer:" << w2.RMUAnswer << std::endl;
    } catch (Xception* pErr) {
        answer = pErr->GetNotification();
        std::cout << "FINAL_LOG: " << answer << std::endl;
    }
}
