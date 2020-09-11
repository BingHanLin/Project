#include "threadMismatch.hpp"
#include <chrono>
#include <iostream>
#include <omp.h>
#include <string>
#include <vector>

void threadMismatchTest(const int testIteration, const int outterLoopNum,
                        const int innerLoopNum) {

  // ====================================================
  // thread mismatch
  // ====================================================
  for (int it = 0; it < testIteration; ++it) {
    std::vector<double> testVec(outterLoopNum);

    auto startTime = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
    for (int i = 0; i < outterLoopNum; ++i) {
      for (int j = 0; j < innerLoopNum; ++j) {
        int tempA = 5;
        int tempB = 6;
        testVec[i] += (tempA + tempB);
      }
    }

    double timeDiff = std::chrono::duration_cast<std::chrono::duration<double>>(
                          std::chrono::high_resolution_clock::now() - startTime)
                          .count();

    std::cout << "mismacth finish in: " << std::to_string(timeDiff) << " s"
              << std::endl;
  }

  // ====================================================
  // none thread mismatch
  // ====================================================
  for (int it = 0; it < testIteration; ++it) {
    std::vector<double> testVec(outterLoopNum);

    auto startTime = std::chrono::high_resolution_clock::now();
#pragma omp parallel for
    for (int i = 0; i < outterLoopNum; ++i) {
      int tempSum = 0;
      for (int j = 0; j < innerLoopNum; ++j) {
        int tempA = 5;
        int tempB = 6;
        tempSum += (tempA + tempB);
      }
      testVec[i] = tempSum;
    }
    double timeDiff = std::chrono::duration_cast<std::chrono::duration<double>>(
                          std::chrono::high_resolution_clock::now() - startTime)
                          .count();

    std::cout << "none-mismacth finish in: " << std::to_string(timeDiff) << " s"
              << std::endl;
  }
}
