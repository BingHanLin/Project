#include "loopSchedules.hpp"
#include <chrono>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <string>
#include <vector>

void loopSchedules() {

  // ====================================================
  // static
  // ====================================================
  std::cout << "------------------------" << std::endl;
  std::cout << "static, defualt chunk size" << std::endl;
  omp_set_schedule(omp_sched_t::omp_sched_static, 0);
  loopExecution();

  // ====================================================
  // static with chunk n
  // ====================================================
  std::cout << "------------------------" << std::endl;
  std::cout << "static, user defined chunk size" << std::endl;
  omp_set_schedule(omp_sched_static, 1);
  loopExecution();

  // ====================================================
  // dynamic
  // ====================================================
  std::cout << "------------------------" << std::endl;
  std::cout << "dynamic,  defualt chunk size" << std::endl;
  omp_set_schedule(omp_sched_dynamic, 0);
  loopExecution();
  std::cout << "------------------------" << std::endl;
  std::cout << "dynamic, user defined chunk size" << std::endl;
  omp_set_schedule(omp_sched_dynamic, 100);
  loopExecution();

  // ====================================================
  // guided
  // ====================================================
  std::cout << "------------------------" << std::endl;
  std::cout << "guided,  defualt chunk size" << std::endl;
  omp_set_schedule(omp_sched_guided, 0);
  loopExecution();

  // ====================================================
  // auto
  // ====================================================
  std::cout << "------------------------" << std::endl;
  std::cout << "auto,  defualt chunk size" << std::endl;
  omp_set_schedule(omp_sched_auto, 0);
  loopExecution();
}

void loopExecution() {

  omp_sched_t kind;
  int chunk;
  omp_get_schedule(&kind, &chunk);

  double quarterpi = 0;
  int nsteps = 100'000'000;
  double h = 1.0 / nsteps;

  std::cout << "schedule type: " << kind << std::endl;
  std::cout << "   chunk size: " << chunk << std::endl;

  auto startTime = std::chrono::high_resolution_clock::now();

#pragma omp parallel for reduction(+ : quarterpi) schedule(runtime)
  for (int i = 0; i < nsteps; i++) {
    double x = i * h;
    double x2 = (i + 1) * h;
    double y = sqrt(1.0 - x * x);
    double y2 = sqrt(1.0 - x2 * x2);
    double slope = (y - y2) / h;

    if (slope > 1000) {
      slope = 1000;
    }

    int samples = 1 + (int)slope;
    int is;

    for (int is = 0; is < samples; is++) {
      double hs = h / samples;
      double xs = x + is * hs;
      double ys = sqrt(1 - xs * xs);
      quarterpi += hs * ys;
    }
  }

  double pi = 4 * quarterpi;

  double timeDiff = std::chrono::duration_cast<std::chrono::duration<double>>(
                        std::chrono::high_resolution_clock::now() - startTime)
                        .count();

  std::cout << "computing pi " << pi << " in: " << std::to_string(timeDiff)
            << " s" << std::endl;
}
