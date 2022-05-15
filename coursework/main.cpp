#include"anz.hpp"
#include<iostream>
int main() {
    std::string answer;
    try {
        Word w("семибоярщина", "rm.stem.data");
       w.analize();
       std::cout<< "Result:" << w.result << std::endl << "Stress:" << w.stress << std::endl;
       std::cout << "RMUAnswer:" << w.RMUAnswer << std::endl; 
    } catch (Xception* pErr) {
        answer = pErr->GetNotification();
        std::cout << "FINAL_LOG" << answer << std::endl;
    }
        Word w2("черно-белое", "rm.stem.data");
       w2.analize();
       std::cout<< "Result" << w2.result<< "\n Stress" <<w2.stress;
}