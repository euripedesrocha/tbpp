# tbpp : A Simple test bench support library for Verilator

This is a simple support library for Verilator based test benchs.

Verilator is a Verilog simulator that generates a C++ model from the Verilog sources. 
This librarys adds some helper classes and functions to simplify testbench creation.

This library is for C++ study and intended to use in my projects. No promise of stability on the interface for the next months, this is also very experimental. 

This can be used as a fusesoc core. 

## simple_tb

For simple combinational designs tbpp::base_tb provides a simple class that eliminates the need to delete the model when it goes out of scope.

# clocked_tb

Most of designs are complex and use a clock to work. This class aims to help on writing complex tests. At this moment the class only adds helper functions to execute cycles of clock and to enable tracing to a file when needed. 

The original idea of this code is to use with [Catch2](https://github.com/catchorg/Catch2) as in the example below. 
Usage

```cpp
constexpr auto enable_tracing = true;
using testbench_type = tbpp::clocked_tb<VERILATED_DESIGN, enable_tracing>;

TEST_CASE("Example") {
  testbench_type dut;
  // Select file to trace 
  dut.trace_to("file_name.vcd");
  dut->module_port_access = 0;
  // Execute 1 clock cycle
  tbpp::run(dut);
  // Execute 50 clock cycle
  tbpp::run(dut, 50);
} 
```
