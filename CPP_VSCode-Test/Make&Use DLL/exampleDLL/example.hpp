#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#ifdef EXAMPLELIBRARY_EXPORTS
#define EXAMPLELIBRARY_API __declspec(dllexport)
#else
#define EXAMPLELIBRARY_API __declspec(dllimport)
#endif

extern "C" EXAMPLELIBRARY_API double addNumbers(const double& a,
                                                const double& b);

#endif  // EXAMPLE_HPP