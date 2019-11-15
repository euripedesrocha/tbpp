#pragma once
#include <memory>

namespace SUT {

template <class VerilatedSut>
class Tester {
  struct EndSimulation {
    void operator()(VerilatedSut* p) {
      if (p) p->final();
      delete p;
    };
  };

  std::unique_ptr<VerilatedSut, EndSimulation> system_model;

 public:
  Tester() : system_model(new VerilatedSut) {}

  void ClockCycle() {
    system_model->clk = 1;
    system_model->eval();
    system_model->clk = 0;
  };
};
}  // namespace SUT
