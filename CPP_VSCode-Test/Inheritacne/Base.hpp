#pragma once

#include <iostream>
#include <string>

class Base
{
   protected:
    std::string name_;
    std::string data_;

   public:
    Base();
    Base(const std::string data);
    // ~Base();

    void show();
    virtual void vShow();
};

Base::Base()
{
    name_ = "Base";
    data_ = "defualt data";
    std::cout << "Base empty " << name_ << " constructed. " << std::endl;
}

Base::Base(const std::string data) : data_(data)
{
    name_ = "Base";
    std::cout << "Base " << name_ << " constructed. " << std::endl;
}

// Base::~Base()
// {
//     std::cout << "Base " << name_ << " deleted " << std::endl;
// }

void Base::show()
{
    std::cout << "Base's name " << name_ << std::endl;
}

void Base::vShow()
{
    std::cout << "vShow Base's name " << name_ << std::endl;
}
