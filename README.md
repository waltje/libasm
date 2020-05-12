![C/C++ CI](https://github.com/tgtakaoka/libasm/workflows/C/C++%20CI/badge.svg)

# libasm: cross assemble/disassemble library

The libasm allows for assembling and disassembling supported CPUs on a
small environment (less than 15kB on Arduino).

* Supported target CPUs.
  - 6809, 6309
  - 6800, 6801, 6301
  - 6502, 65SC02, R65C02, W65C02S
  - 65816 (Support 24bit address and LONGA/LONGI)
  - 8080, 8085
  - Z80
  - SC/MP
  - INS8070
  - TMS9900, TMS9995
  - 68000

* Supported host environment.
  - Arduino
  - Linux, macOS (`g++`, `clang++`)

More information about this library can be found at
https://github.com/tgtakaoka/libasm
