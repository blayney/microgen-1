#include "microaddr_counter_microaddr_types.h"
#include "microaddr_counter.h"
#include "verilated.h"

// uses table driven testing
struct TestCase {
	const char* name;
	uint8_t reset; // will set output to 0
	uint8_t zflag; // if addr space is 0
	uint8_t M; // new stack pointer value
	uint8_t cmd; // command pointer
	uint16_t load_addr; // input
	uint16_t expected_addr; // expected output
};

using cmd_t = microaddr_counter_microaddr_types::cmd_t;

// creates a bunch of instances of TestCase
// each one testing the named command

TestCase test_cases[] {

	// reset should have a 0 output address, regardless of input:
	{ "reset", 1, 0, 0, cmd_t::NONE, 0, 0 },

	// should increment output address, so it should be load + 1:
	{ "inc", 0, 0, 0,  cmd_t::INC, 0, 1 },

	// none command should leave output unchanged:
	{ "none", 0, cmd_t::NONE, 0, 1 },

	// reset should reset ldra back to 0:
	{ "reset2", 1, cmd_t::NONE, 0, 0 },

	// load should set output to input:
	{ "load", 0, cmd_t::LOAD, 0xFA, 0xFA },

	// increment should inc the loaded address:
	{ "inc2", 0, cmd_t::INC, 0, 0xFB },

	// reset should reset ldra back to 0 from 0xFB:
	{ "reset3", 1, cmd_t::NONE, 0, 0 },


	{}
};

int main(int argc, char **argv, char **env) {

	// passes command line arguments to verilator
	Verilated::commandArgs(argc, argv);
	// create a pointer to an instance of a microaddress class
	Microaddr* counter = new Microaddr;

	// initializes instance and calls evaluate method
	counter -> clk = 0;
	counter -> reset = 0;
	counter -> cmd = Microaddr_microaddr::cmd::NONE;
	counter -> load_addr = 0;
	counter -> eval();

	// I'm not sure how this works, Robert implements it at:
	// https://youtu.be/bxhT_btN_Cs?t=1939
	int num_test_cases = sizeof(test_cases) / sizeof(TestCase);

	// iterates through each test case
	for(int k = 0; k < num_test_cases; k++) {
		TestCase *test_case = &test_cases[k];

		// set each counter variable to the test case one
		counter -> cmd = test_case -> cmd;
		counter -> reset = test_case -> reset;
		counter -> load_addr = test_case -> load_addr;
		// clock not enabled so nothing should change
		counter -> eval();

		// tests with posedge:
		counter -> clk = 1;
		counter -> eval();
		// tests with no posedge:
		counter -> clk = 0;
		counter -> eval();

		// cpp structure of format parameters (% in prints):
		// %[flags][width][.precision][length]specifier
		// 0 flag pads values to the left with 0s,
		// width is the minimum number of values to be printed,
		// specifier X sets to an unsigned hex int
		// specifier s sets to a string or char array
		if (counter -> addr == test_case -> expected_addr) {
			printf("%s: passed\n", 
				test_case -> name);
		} else {
			printf("%s: fail (expected %04X but was %04X)\n",
				test_case -> name, 
				test_case -> expected_addr,
				counter -> addr);
		}
	}
	counter -> final();
	delete counter;
	exit(0);
}