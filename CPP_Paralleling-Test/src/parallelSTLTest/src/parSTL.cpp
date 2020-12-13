#include "parSTL.hpp"
#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <random>
#include <string>

// https://software.intel.com/en-us/forums/intel-c-compiler/topic/777017
void parSTL(const int inputThreadNum, const int iterationCount,
            const int testSize) {

  std::random_device rd;
  // generate some random doubles:
  printf("Testing with %zu doubles...\n", testSize);
  std::vector<double> doubles(testSize);
  for (auto &d : doubles) {
    d = static_cast<double>(rd());
  }

  //   time how long it takes to sort them
  for (int i = 0; i < iterationCount; ++i) {
    std::vector<double> sorted(doubles);
    auto startTime = std::chrono::high_resolution_clock::now();
    std::sort(sorted.begin(), sorted.end());

    double timeDiff = std::chrono::duration_cast<std::chrono::duration<double>>(
                          std::chrono::high_resolution_clock::now() - startTime)
                          .count();

    std::cout << "stl finish in: " << std::to_string(timeDiff) << " s"
              << std::endl;
  }

  for (int i = 0; i < iterationCount; ++i) {
    std::vector<double> sorted(doubles);
    auto startTime = std::chrono::high_resolution_clock::now();
    std::sort(std::execution::par_unseq, sorted.begin(), sorted.end());

    double timeDiff = std::chrono::duration_cast<std::chrono::duration<double>>(
                          std::chrono::high_resolution_clock::now() - startTime)
                          .count();

    std::cout << "parallel stl finish in: " << std::to_string(timeDiff) << " s"
              << std::endl;
  }
}
