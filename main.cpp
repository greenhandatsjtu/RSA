#include <iostream>

using namespace std;

#include "utils.h"

int main() {
    srand(time(0));//随机数种子
    char text[] = "Mathematical Fundation of Information security";
    RSA(text);

    char text_2[] = "I LOVE SJTU!I LOVE MATH!";
    RSA(text_2);

    cout << "\n请继续输入需要加密的内容：(ctrl+C终止)\n";
    while (cin.getline(text, 1000)) {
        RSA(text);
        cout << "\n请继续输入需要加密的内容：(ctrl+C终止)\n";
    }
    return 0;
}
