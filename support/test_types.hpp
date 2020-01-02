#pragma once
#include <cstdint>
#include <memory>

#include "verilated.h"
#include "verilated_vcd_c.h"

namespace tbpp {

template <class Dut>
struct stop_simulation {
  auto operator()(Dut* p) {
    if (p) p->final();
    delete p;
  }
};

template <bool should_trace>
class trace_file {};

template <>
class trace_file<true> {
  VerilatedVcdC* trace = nullptr;
  uint64_t sim_time = 0;

 public:
  trace_file() : trace(new VerilatedVcdC) { Verilated::traceEverOn(true); }

  virtual ~trace_file() {
    if (trace) trace->close();
    delete trace;
  }
  auto get_trace() const noexcept { return trace; }
  void open(char const* const file_name) { trace->open(file_name); }
  void time_and_dump() {
    ++sim_time;
    trace->dump(sim_time);
  }
};

template <class Dut>
using base_tb = std::unique_ptr<Dut, stop_simulation<Dut>>;

template <class Dut, bool should_trace = false>
struct base_clocked_tb : trace_file<should_trace> {
  base_tb<Dut> dut;

  base_clocked_tb() : dut(new Dut){};

  auto trace_to(std::string const name, size_t const level = 10) {
    static_assert(should_trace, "Trace parameter must be true");
    dut->trace(this->get_trace(), level);
    this->open(name.c_str());
  }

  auto cycle(std::size_t n_cycles = 1) {
    if constexpr (should_trace) {
      cycle_with_trace(n_cycles);
    } else {
      cycle_no_trace(n_cycles);
    }
  }

 private:
  auto cycle_no_trace(std::size_t n_cycles = 1) {
    for (std::size_t executed = 0; executed < n_cycles; ++executed) {
      dut->clk = 0;
      dut->eval();
      dut->clk = 1;
      dut->eval();
    }
  }
  auto cycle_with_trace(std::size_t n_cycles = 1) {
    for (std::size_t executed = 0; executed < n_cycles; ++executed) {
      dut->clk = 0;
      dut->eval();
      this->time_and_dump();
      dut->clk = 1;
      dut->eval();
      this->time_and_dump();
    }
  }
};

}  // namespace tbpp
