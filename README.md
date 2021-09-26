# RVSim
A simple RISC-V ISA simulator

## Highlights
1. Supports RV32I [pending], RV64I[pending], RVE[pending].
2. Supported extenstions: 
    1. M extension. [pending]
    2. A extension. [pending]
    3. F extension. [pending]
    4. D extension. [pending]
    5. C extension. [pending]

3. Interactive Debug Mode. [pending]
4. Fully Compliant with `riscv_arch_tests`. [pending]


## Build Instructions
1. Create a build directory
    ```bash
    $ mkdir build
    $ cd build
    ```

2. Run CMAKE to generate Makefile
    ```bash
    $ cmake ..
    ```

3. Run Make to build RVSim
    ```bash
    $ make
    ```

4. Run the executable
    ```bash
    $ ./rvsim --help
    ```