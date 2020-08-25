


#include <iostream>
#include <queue>          // std::queue
#include <fstream>

using namespace std;


void print_pp (string** pp, int m, int n, std::queue<string>& verilog, bool to_file);

void create_signedpp (int m, int n,
		      string**& pp_matrix,
		      int& pp_dim1,
		      int& pp_dim2,
		      std::queue<string>& verilog);

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



void create_unsignedbr4pp (int m, int n,
			   string**& pp_matrix,
			   int& pp_dim1,
			   int& pp_dim2,
			   std::queue<string>& verilog);



void create_signedbr2pp (int m, int n,
			string**& pp_matrix,
			int& pp_dim1,
			int& pp_dim2,
			 std::queue<string>& verilog);

void create_unsignedbr2pp (int m, int n,
			   string**& pp_matrix,
			   int& pp_dim1,
			   int& pp_dim2,
			   std::queue<string>& verilog);



void create_unsignedbr2pp (int m, int n,
			   string**& pp_matrix,
			   int& pp_dim1,
			   int& pp_dim2,
			   std::queue<string>& verilog);
