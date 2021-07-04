#include "example.hpp"
#include <iostream>

// DLL function signature
typedef double (*importFunction)(double, double);

int main()
{
    const double result = addNumbers(10, 20);
    std::cout << "The result is: " << result << std::endl;
    return 0;
}