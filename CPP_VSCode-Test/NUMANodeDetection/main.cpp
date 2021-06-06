#include <windows.h>

#include <iostream>
#include <string>
#include <sysinfoapi.h>
#include <thread>
#include <winbase.h>

int main(int argc, char *argv[])
{
    std::cout << "Number of cores(std::thread::hardware_concurrency): "
              << std::thread::hardware_concurrency() << std::endl;

    std::cout << std::endl;

    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    std::cout << "Number of cores(WIN API SYSTEM_INFO): "
              << sysinfo.dwNumberOfProcessors << std::endl;

    std::cout << std::endl;

    int nNumGroups = GetActiveProcessorGroupCount();
    std::cout << "Number of processor groups: " << nNumGroups << std::endl;

    for (int i = 0; i < nNumGroups; i++)
    {
        int nNumCores = GetMaximumProcessorCount(i);

        std::cout << "Group " << i << " has number of cores: " << nNumCores
                  << std::endl;
    }

    return 0;
}