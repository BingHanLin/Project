#include <iostream>
#include <string>

class Base
{
   public:
    Base(int size) : mBaseSize(size)
    {
        mBase = new int[size];
    }

    virtual ~Base();  // ensure polymorphic delete operation results (derived
                      // destructor is executed.)

    // ~Base();

   private:
    int *mBase;
    int mBaseSize;
};

Base::~Base()
{
    std::cout << "delete mBase" << std::endl;
    delete[] mBase;
}

class Derived : public Base
{
   public:
    Derived(int a_size, int b_size) : Base(a_size), mDerivedSize(b_size)
    {
        mDerived = new int[b_size];
    }
    ~Derived();

   private:
    int *mDerived;
    int mDerivedSize;
};

Derived::~Derived()
{
    std::cout << "delete mDerived" << std::endl;
    delete[] mDerived;
}
int main()
{
    Base *a = new Derived(5, 10);
    delete a;
    return 0;
}
