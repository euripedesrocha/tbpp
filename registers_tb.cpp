#include <memory>

#include "Vregisters.h"
#include "catch.hpp"
#include "test_types.hpp"

TEST_CASE("Testing Catch + Verilator", "[]") {
  tbpp::BaseClockedTB<Vregisters> TB;
  TB.sut->write_data = true;
  TB.Run();
  REQUIRE(TB.sut->write_data == TB.sut->read_data);
}
