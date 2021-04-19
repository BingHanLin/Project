#include "Base.hpp"
#include "Derived.hpp"

int main()
{
    // Derived d;
    // d.show();
    // d.vShow();

    Base* ptB = new Derived();
    std::cout << "===================" << std::endl;

    ptB->show();
    ptB->vShow();
    std::cout << "===================" << std::endl;

    delete ptB;

    return 0;
}
