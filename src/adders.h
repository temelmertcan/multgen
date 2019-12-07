
#include <iostream>
#include <queue>          // std::queue
#include <fstream>

using namespace std;

void create_ha_fa (std::queue<string>& verilog);


void create_rp_adder (int size, std::queue<string>& verilog);

void create_hc_adder (int size, std::queue<string>& verilog);

void create_lf_adder (int size, std::queue<string>& verilog);
