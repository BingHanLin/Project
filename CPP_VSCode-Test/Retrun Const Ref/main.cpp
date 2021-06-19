#include <iostream>
#include <string>
#include <vector>

class testA
{
   public:
    testA()
    {
        val_ = 5;
    };
    ~testA(){};

    const int &constRefValue() const
    {
        return val_;
    };

    int &refValue()
    {
        return val_;
    };

    const int value() const
    {
        return val_;
    };

   public:
    int val_;
};

int main(int argc, char *argv[])
{
    testA a;

    const int &constRefValue = a.constRefValue();
    int &refValue = a.refValue();
    const int value = a.value();

    std::cout << &constRefValue << std::endl;
    std::cout << &refValue << std::endl;
    std::cout << &value << std::endl;
    std::cout << &a.val_ << std::endl;

    return 0;
}