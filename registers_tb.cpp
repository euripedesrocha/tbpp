#include <memory>

#include "Vregisters.h"
#include "support/catch.hpp"
//#include "verilated.h"

TEST_CASE("Testing Catch + Verilator", "[]") {
  auto sut = std::make_unique<Vregisters>();
  /*Vour* top = new Vour;
  while (!Verilated::gotFinish()) {
    top->eval();
  }
  delete top;*/
  REQUIRE(true == true);
}
