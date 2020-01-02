#include <cstdint>
#include <memory>

#include "Vregisters.h"
#include "catch.hpp"
#include "test_types.hpp"

enum class reg : uint8_t { zero, ra };

class register_tb : public tbpp::base_clocked_tb<Vregisters, true> {
 public:
  void write(reg reg_name, std::uint32_t data) {
    dut->write_en = true;
    dut->write_to = static_cast<uint8_t>(reg_name);
    dut->write_data = data;
    cycle();
  }
  auto read(reg reg_name) {
    dut->read_from = static_cast<uint8_t>(reg_name);
    cycle();
    return dut->read_data;
  }
};

TEST_CASE("Functional Verification of Registers", "[Reg]") {
  register_tb tb;
  SECTION("Write to R0 must not change it's value") {
    tb.write(reg::zero, 5);
    auto read = tb.read(reg::zero);
    REQUIRE(read == 0);
  }
  SECTION("Write to any register") {
    tb.trace_to("registers.vcd");
    tb.write(reg::ra, 5);
    auto read = tb.read(reg::ra);
    REQUIRE(read == 5);
  }
}
