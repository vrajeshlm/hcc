
// RUN: %hc %s -o %t.out && %t.out

// Parallel STL headers
#include <coordinate>
#include <experimental/algorithm>
#include <experimental/numeric>
#include <experimental/execution_policy>

#define _DEBUG (0)
#include "test_base.h"

template<typename T, size_t SIZE>
bool test(void) {

  auto op = [](const T &x) { return x+1; };
  auto binary_op = std::plus<T>();
  auto init = T{};

  using std::experimental::parallel::par;

  bool ret = true;

  // std::array
  typedef std::array<T, SIZE> stdArray;
  ret &= run_and_compare<T, SIZE, stdArray>([op, binary_op, init]
                                            (stdArray &input, stdArray &output1,
                                                              stdArray &output2) {
    // transform_inclusive_scan = transform + partial_sum (inclusive)
    std::transform(std::begin(input), std::end(input), std::begin(output1), op);
    std::partial_sum(std::begin(output1), std::end(output1), std::begin(output1), binary_op);

    // parallel::transform_inclusive_scan
    std::experimental::parallel::
    transform_inclusive_scan(par, std::begin(input), std::end(input),
                                  std::begin(output2), op, binary_op, init);
  });
  return ret;
}

int main() {
  bool ret = true;

  ret &= test<int, TEST_SIZE>();
  ret &= test<unsigned, TEST_SIZE>();
  ret &= test<float, TEST_SIZE>();
  ret &= test<double, TEST_SIZE>();

  return !(ret == true);
}

