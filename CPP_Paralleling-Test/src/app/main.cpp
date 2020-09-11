#include "loopSchedules.hpp"
#include "loopSchedulesVisualize.hpp"
#include "threadMismatch.hpp"
#include "workSharing.hpp"

#include "parSTL.hpp"

// https://software.intel.com/en-us/forums/intel-c-compiler/topic/777017
int main() {
  // threadMismatchTest(5, 1'000'000, 10'000);
  // loopSchedules();
  // loopSchedulesVisualize();
  ompSections();
  ompSingle();
  ompMaster();
  parSTL(5, 5, 100000);
}