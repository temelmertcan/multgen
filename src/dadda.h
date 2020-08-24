
#include <iostream>
#include <queue>          // std::queue
#include <fstream>

using namespace std;

void get_dadda_coeffs (int max, int*& coeffs, int& size);

void create_daddatree (string** pp_matrix,
		       string final_stage_adder,
		       int pp_dim1,
		       int pp_dim2,
		       int out_size,
		       bool create_fin_adder,
		       bool signed_mult,
		       std::queue<string>& verilog,
		       int& adder_size);
