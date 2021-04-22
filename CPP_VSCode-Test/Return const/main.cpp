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

    int val_;
};

class testB
{
   public:
    testB(){};
    ~testB(){};

    const testA *getConstAPointer()
    {
        A_.val_ = 10;
        return &A_;
    };

    testA *const getCosntPointerA()
    {
        A_.val_ = 10;
        return &A_;
    };

    testA A_;
};

void testFun()
{
    int a = 5, b = 10, c = 15;

    const int *foo;  // pointer to constant int.
    foo = &a;        // assignment to where foo points to.

    /* dummy statement*/
    *foo = 6;  // the value of a can't get changed through the pointer.

    foo = &b;  // the pointer foo can be changed.

    int *const bar = &c;  // constant pointer to int
                          // note, you actually need to set the pointer
                          // here because you can't change it later ;)

    *bar = 16;  // the value of c can be changed through the pointer.

    /* dummy statement*/
    bar = &a;
}

int main(int argc, char *argv[])
{
    testB b;
    testA oa;

    testA *const pa = b.getCosntPointerA();

    const testA *ap = b.getConstAPointer();

    pa->val_ = 11;
    ap->val_ = 11;

    pa = &oa;
    ap = &oa;

    return 0;
}