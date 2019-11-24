#include <memory>

#include "Vregisters.h"
#include "Vregisters_cornigera_pkg.h"
#include "catch.hpp"
#include "test_types.hpp"

TEST_CASE("Functional Verification of Registers", "[Reg]") {
  tbpp::BaseClockedTB<Vregisters> TB;

  SECTION("Read from Register R0 must always return zero") {
    // Write random data to zero
    // run cycle
    TB.sut->read_from = Vregisters_cornigera_pkg::zero;
    TB.Cycle();
    REQUIRE(TB.sut->read_data == 0);
  }
  SECTION("Write to R0 must not change it's value") {}
  SECTION("Write to R0 must not change it's value") {}
  SECTION("Write to any register") {}
  SECTION("Write to R0 must not change it's value") {}
}
