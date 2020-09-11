#include "workSharing.hpp"
#include <iostream>
#include <omp.h>
#include <unistd.h>
#include <vector>

void ompSections() {
  // http://jakascorner.com/blog/2016/05/omp-sections.html

  // Each thread executes one block at a time. Each block is executed only once
  // by one thread.

  double y = 0.0;
#pragma omp parallel
  {
#pragma omp sections reduction(+ : y)
    {
#pragma omp section
      {
        // sleep(1);
        y += 1;
        std::cout << "thread in section 1: " << omp_get_thread_num()
                  << std::endl;
      }
#pragma omp section
      {
        y += 2;
        std::cout << "thread in section 2: " << omp_get_thread_num()
                  << std::endl;
      }
    } // end of omp sections
  }   // end of omp parallel

  std::cout << "result of y: " << y << std::endl;
}

void ompSingle() {

  int vecNum = 10;
  std::vector<int> vec(vecNum);

#pragma omp parallel
  {
    std::cout << "here is TID: " << omp_get_thread_num() << std::endl;

#pragma omp single
    {
      std::cout << " vec initialized  with TID:" << omp_get_thread_num()
                << std::endl;

      for (int i = 0; i < vecNum; i++) {
        vec[i] = i;
      }

      std::cout << " vec has been initialized with TID:" << omp_get_thread_num()
                << std::endl;

    } // end of omp single

#pragma omp for
    for (int i = 0; i < vecNum; i++) {

      std::cout << " TID:" << omp_get_thread_num() << ", vec[" << i
                << "] = " << vec[i] << std::endl;
    }

  } // end of omp parallel
}

void ompMaster() {

  int vecNum = 10;
  std::vector<int> vec(vecNum);

#pragma omp parallel
  {
    std::cout << "here is TID: " << omp_get_thread_num() << std::endl;

#pragma omp master
    {
      std::cout << " vec initialized  with TID:" << omp_get_thread_num()
                << std::endl;

      for (int i = 0; i < vecNum; i++) {
        vec[i] = i;
      }

      std::cout << " vec has been initialized with TID:" << omp_get_thread_num()
                << std::endl;

    } // end of omp single

#pragma omp for
    for (int i = 0; i < vecNum; i++) {

      std::cout << " TID:" << omp_get_thread_num() << ", vec[" << i
                << "] = " << vec[i] << std::endl;
    }

  } // end of omp parallel
}
