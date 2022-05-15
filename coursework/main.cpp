#include"anz.hpp"

int main() {
    std::string answer;
    try {
        Word w("семибоярщина", "rm.stem.data");
       w.analize();
       std::cout<< "Result" << w.result<< "\n Stress" <<w.stress; 
    } catch (Xception* pErr) {
        answer = pErr->GetNotification();
        std::cout << "FINAL_LOG" << answer << std::endl;
    }
        Word w2("черно-белое", "rm.stem.data");
       w2.analize();
       std::cout<< "Result" << w2.result<< "\n Stress" <<w2.stress;
}