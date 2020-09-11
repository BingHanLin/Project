#include "loopSchedulesVisualize.hpp"
#include <chrono>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <string>
#include <vector>

void loopSchedulesVisualize() {

  // ====================================================
  // static
  // ====================================================
  std::cout << "------------------------" << std::endl;
  std::cout << "static, defualt chunk size" << std::endl;
  omp_set_schedule(omp_sched_t::omp_sched_static, 0);
  loopExecutionVisualize();

  // // ====================================================
  // // static with chunk n
  // // ====================================================
  std::cout << "------------------------" << std::endl;
  std::cout << "static, user defined chunk size" << std::endl;
  omp_set_schedule(omp_sched_static, 1);
  loopExecutionVisualize();

  // // ====================================================
  // // dynamic
  // // ====================================================
  std::cout << "------------------------" << std::endl;
  std::cout << "dynamic,  defualt chunk size" << std::endl;
  omp_set_schedule(omp_sched_dynamic, 0);
  loopExecutionVisualize();

  // // ====================================================
  // // guided
  // // ====================================================
  std::cout << "------------------------" << std::endl;
  std::cout << "guided,  defualt chunk size" << std::endl;
  omp_set_schedule(omp_sched_guided, 0);
  loopExecutionVisualize();
}

void loopExecutionVisualize() {

  omp_sched_t kind;
  int chunk;
  omp_get_schedule(&kind, &chunk);

  std::cout << "schedule type: " << kind << std::endl;
  std::cout << "   chunk size: " << chunk << std::endl;

  int threadNum = 0;

  int nsteps = 40;
  std::vector<int> threadTrack(nsteps);

#pragma omp parallel
  {
#pragma omp for schedule(runtime)
    for (int i = 0; i < threadTrack.size(); i++) {
      int tempSum;
      for (int j = 0; j < 100'000'000; ++j) {
        int tempA = 5;
        int tempB = 6;
        tempSum += (tempA + tempB);
        tempSum += (tempA + tempB);
      }

      threadTrack[i] = omp_get_thread_num();
    }

    threadNum = omp_get_num_threads();
  }

  for (int i = 0; i < threadNum; i++) {
    std::cout << "T " << i << ": ";

    for (int j = 0; j < threadTrack.size(); j++) {
      if (threadTrack[j] == i) {
        std::cout << "o";
      } else {
        std::cout << "-";
      }
    }
    std::cout << "\n";
  }
}
