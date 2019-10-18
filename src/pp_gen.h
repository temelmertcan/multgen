


#include <iostream>
#include <queue>          // std::queue
#include <fstream>

using namespace std;



void create_unsignedpp (int m, int n,
			string**& pp_matrix,
			int& pp_dim1,
			int& pp_dim2,
			std::queue<string>& verilog);

void create_signedbr4pp (int m, int n,
			string**& pp_matrix,
			int& pp_dim1,
			int& pp_dim2,
			 std::queue<string>& verilog);
