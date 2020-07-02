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
