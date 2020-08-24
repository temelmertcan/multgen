
#include <iostream>
#include <queue>          // std::queue
#include <fstream>

using namespace std;


void move_to_main_queue (queue<string>* temp,
			 queue<string>* main,
			 int size);



int get_max_queue_size (std::queue<string>* q, int size);


void create_finaladder_inst (std::queue<string>*& main_queue,
			     string final_stage_adder,
			     int out_size,
			     std::queue<string>& verilog,
			     int& adder_size);


void create_two_result_vectors (std::queue<string>*& main_queue,
				int out_size,
				std::queue<string>& verilog,
				bool signed_mult,
				int& result_vector_size);

void create_wallacetree (string** pp_matrix,
			 string final_stage_adder,
			 int pp_dim1,
			 int pp_dim2,
			 int out_size,
			 bool create_fin_adder,
			 bool signed_mult,
			 std::queue<string>& verilog,
			 int& adder_size);
