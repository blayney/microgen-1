# microgen-1
## overview
This is a learning exercise in building microarchitecture with verilog for an FPGA. Some of the code is adapted from Robert Baruch's "plugh-1", which is a softcore CPU designed to run a 70s CLI adventure game called Zork. The repo is here and he has a youtube series documenting the project here, and credit for the verilator testbench style goes to him, I used his microaddr_counter_test as a baseline to build test case enums and structs.
## dependencies and building
The testing and verification is done using Verilator:
'apt-get install verilator' for the version built into the package manager repo which is unlikely to be the most up to date, or it can be built:

`git clone https://git.veripool.org/git/verilator`

`unsetenv VERILATOR_ROOT`

`unset VERILATOR_ROOT`

`cd verilator`

`git pull`

`autoconf`

`./configure`

`make`

`sudo make install`

## about the project

I’m going to deploy onto a CMOD A7, which is effectively a breadboardable development board built around a Xilinx Artix 7 with USB JTAG + UART, some SRAM, Quad-SPI flash allowing me to save the configuration on the board instead of bitstreaming every time the device powercycles, and some IO like buttons and LEDs which make testing easier.

I’ll be porting a small number of instructions from RISC-V over to this CPU, using a combination of microcode and hardwired implementation - a lot of FPGA RISC-V microarchitectures that seem or claim to be hardwired use some amount of (what I would call) microcode to run routines / control flow having “interpreted” an instruction or set of instructions. Keeping a small number of hardware operations (and modules) means verification and development is easier, so any instructions that can be simplified or implimented as basic routines made of other instructions in the set (using what I would call a microengine by the definition in this paper) are a bonus.


#### Development Environment - Verilator and SystemVerilog Style

I'm following [Robert Baruch](https://www.youtube.com/channel/UCBcljXmuXPok9kT_VGA3adg)'s style of CPU-on-FPGA development, because it's the most approachable (and well documented) workflow for an FPGA noob like me. My interpretation of this workflow is as follows:

1. Make a list of "commands" / operations you want to work with, and the functionality you want each one to have. Hardware operations have to be kept inside commands, with a few exceptions like address resets and things that can be put in flags.
2. Define this list of operations as types, keeping this in it's own package with just the enum and no functionality or modules.
3. Define a module importing the typedef package and define the interaction between ports / signals and the functionality you wannt the operations to have, sticking only to the types you declared previously and not adding more operations without adding them to the typedef
4. "Compile" this verilog with verilator and build a testbench that verifies expected outputs with real outputs, pulled from the verilated.h header. This should ideally verify every combination of relevant flags, signals and clock status (posedge and no posedge), and the modules should only be elaborated / go through DRC and implimented if they've passed these verilator tests.

Obviously once a new set of modules has been implemented that interacts with the types defined before, the verification done beforehand is to some extent invalidated, and it doesn't always make sense to verify the new blocks exhaustively, especially formally. Some amount of regression testing is important, but how much will hopefully be answered as I get further into this project.

#### Microcoding for RISC-V

Because I'm working from an existing, carefully defined instruction set, I can look at existing microcode solutions for RISC-V and port it over to the FPGA, with a couple of changes to things like stack structure and pipelining to take advantage of the fact that I'm using an FPGA (with RAM blocks inside). RISC-V's specification deliberately avoids "over-architecting", keeping the spec within the bounds of the instruction set and leaving target architecture and microarchitecture decisions to the person implementing them. It's designed to be as universal as possible, and avoids catering to one target architecture, making it ideal for FPGA implementation.

RISC-V has some key advantages when it comes to easy microcode development - things like variable instruction lengths mean that I can avoid wasting lots of space in the stack from having to zero extend either instructions or addresses. It also has a modular instruction set - there are 4 base modules, RV32I (base integer, 47 instructions), RV32E (a reduced version of RV32I for embedded systems, with reduced numbers of counters and integer registers) , RV64I (an expansion of RV32I with 64 bit integer registers and user address space) and RV128I (like RV64I but with a 128 bit flat address space, which is extremely large!) although RV32I alone is enough to get a working CPU and emulate most of the extension modules. These are all layed out in detail in the specification [here](https://content.riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf) although this may not be the most up to date spec for long.
