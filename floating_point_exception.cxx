#include "doctest/doctest.h"
#include "vtkFloatingPointExceptions.h"

#include <iostream>

TEST_CASE("Test if this will be executed before exception") {
  REQUIRE(1 = 1);
}

auto main(int argc, char** argv) -> int {
  vtkFloatingPointExceptions::Enable();
  const auto x = 0.0;
  const auto y = 1.0 / x;

  std::cout << "x: " << x << " y: " << y << std::endl;
}
