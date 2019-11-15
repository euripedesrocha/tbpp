#include <memory>

#include "Vregisters.h"
#include "catch.hpp"
#include "test_types.hpp"

TEST_CASE("Testing Catch + Verilator", "[]") {
  SUT::Tester<Vregisters> Sut;
  /*Vour* top = new Vour;
  while (!Verilated::gotFinish()) {
    top->eval();
  }
  delete top;*/
  REQUIRE(true == true);
}
