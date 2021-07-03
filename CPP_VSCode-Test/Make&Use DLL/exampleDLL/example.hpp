#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

extern "C"
{
    __declspec(dllexport) double addNumbers(const double& a, const double& b);
}

#endif  // EXAMPLE_HPP