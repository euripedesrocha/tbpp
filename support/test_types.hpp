#pragma once
#include <memory>

namespace tbpp {

template <class Verilated>
struct BaseClockedTB {
  struct StopSimulation {
    auto operator()(Verilated* p) {
      if (p) p->final();
      delete p;
    }
  };
  std::unique_ptr<Verilated, StopSimulation> sut;
  BaseClockedTB() : sut(new Verilated){};

  void Cycle(std::size_t nCycles = 1) {
    for (std::size_t executed = 0; executed < nCycles; ++executed) {
      sut->clk = 1;
      sut->eval();
      sut->clk = 0;
    }
  }
};

}  // namespace tbpp
