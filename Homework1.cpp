#include <thread>
#include <iostream>
#include <windows.h>
#include <string>

void func(std::string str) {
    Sleep(500);
    str = "Thread: " + str + "\n";
    std::cout << str;
}

int main() {
    std::thread tA(func, "A");
    std::thread tB(func, "B");
    tA.join();
    tB.join();
}
