#pragma once

#include <experimental/type_traits>
#include <memory>
#include <string>
#include <type_traits>

#include "verilated.h"
#include "verilated_vcd_c.h"

namespace tbpp {

namespace detail {

template <typename T>
using time_and_dump_t = decltype(std::declval<T>().time_and_dump());

template <typename T>
using supports_trace = std::experimental::is_detected<time_and_dump_t, T>;

template <class Dut>
struct stop_simulation {
  auto operator()(Dut* p) {
    if (p) p->final();
    delete p;
  }
};

template <class ClockedDut>
auto simulation_step(ClockedDut& dut) {
  dut->eval();
  if constexpr (supports_trace<ClockedDut>::value) {
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
    auto& dut = static_cast<Dut&>(*this);
    dut->trace(trace, level);
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
class clocked_tb
    : public detail::trace_file<clocked_tb<Dut, should_trace>, should_trace> {
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
