#pragma once

#include <memory>
#include <string>
#include <type_traits>

#include "verilated.h"
#include "verilated_vcd_c.h"

namespace tbpp {

namespace detail {
template <class Dut>
struct stop_simulation {
  auto operator()(Dut* p) {
    if (p) p->final();
    delete p;
  }
};

template <class Dut>
class has_trace_file {
 public:
  enum { value = true };
};

template <class ClockedDut>
auto simulation_step(ClockedDut& dut) {
  dut->eval();
  if constexpr (has_trace_file<ClockedDut>::value) {
    dut.time_and_dump();
  };
}

template <class Dut, bool should_trace>
class trace_file {};

template <class Dut>
class trace_file<Dut, true> {
  VerilatedVcdC* trace;
  uint64_t sim_time = 0;

  void open(char const* const file_name) { trace->open(file_name); }

 public:
  trace_file() : trace(new VerilatedVcdC) { Verilated::traceEverOn(true); }

  virtual ~trace_file() {
    if (trace) trace->close();
    delete trace;
  }
  auto start(std::string const file_name, std::size_t const level) {
    static_cast<Dut>(*this)->trace(trace, level);
    open(file_name.c_str());
  }
  void time_and_dump() {
    ++sim_time;
    trace->dump(sim_time);
  }
};

}  // namespace detail

template <class Dut>
using base_tb = std::unique_ptr<Dut, detail::stop_simulation<Dut>>;

template <class Dut>
auto run(Dut& dut, std::size_t n_cycles = 1) {
  for (std::size_t executed = 0; executed < n_cycles; ++executed) {
    dut->clk = 0;
    detail::simulation_step(dut);
    dut->clk = 1;
    detail::simulation_step(dut);
  }
}

template <class Dut, bool should_trace = false>
class clocked_tb : public detail::trace_file<Dut, should_trace> {
 public:
  clocked_tb() : dut(new Dut){};

  auto trace_to(std::string const name, size_t const level = 10) {
    static_assert(should_trace,
                  "Tracing isn´t enabled. Use tbpp::clocked_tb<DUT, true>");
    this->start(name, level);
  }

  auto operator-> () { return dut.get(); }

 private:
  base_tb<Dut> dut;
};

}  // namespace tbpp
