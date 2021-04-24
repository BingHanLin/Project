#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

void fun3()
{
    std::cout << "fun3" << std::endl;

    std::chrono::milliseconds timespan(2000);
    std::this_thread::sleep_for(timespan);
}

void fun2()
{
    std::cout << "fun2" << std::endl;

    std::chrono::milliseconds timespan(2000);
    std::this_thread::sleep_for(timespan);

    fun3();
}

void fun1()
{
    int test = 5;
    std::cout << "fun1" << std::endl;

    std::cout << test << std::endl;

    std::chrono::milliseconds timespan(2000);
    std::this_thread::sleep_for(timespan);

    fun2();
}

int main(int argc, char *argv[])
{
    std::cout << "main" << std::endl;

    fun1();

    return 0;
}