#pragma once

#include "Base.hpp"

class Derived : public Base
{
   private:
    /* data */
   public:
    Derived(/* args */);
    ~Derived();

    void show();
    virtual void vShow();
};

Derived::Derived(/* args */)
{
    // name_ = "Derived";
}

Derived::~Derived()
{
    std::cout << "Derived " << name_ << " deleted " << std::endl;
}

void Derived::show()
{
    std::cout << "Derived's name " << name_ << std::endl;
    std::cout << "Derived's data " << data_ << std::endl;
}

void Derived::vShow()
{
    std::cout << "vShow Derived's name " << name_ << std::endl;
}
