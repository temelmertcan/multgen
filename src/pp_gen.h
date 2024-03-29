// Note: The license below is based on the template at:
// http://opensource.org/licenses/BSD-3-Clause

// Copyright (C) 2020 Regents of the University of Texas
//All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:

// o Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.

// o Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.

// o Neither the name of the copyright holders nor the names of its
//   contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Original Author(s):
// Mertcan Temel         <mert@utexas.edu>



#include <iostream>
#include <queue>          // std::queue
#include <fstream>
#include <list>

using namespace std;



void add_extra_ones_to_pp_matrix (string *& last_row, int col_size, const std::list<int>& extra_ones_indices);

std::list<int> merge_extra_ones_indices (const std::list<int>& extra_ones_indices1,
                                         const std::list<int>& extra_ones_indices2,
                                         const int& offset2);

void print_pp (string** pp, int m, int n, std::queue<string>& verilog, bool to_file);

void create_signedpp (int m, int n,
		      string**& pp_matrix,
		      int& pp_dim1,
		      int& pp_dim2,
		      std::list<int>& extra_ones_indices,
		      std::queue<string>& verilog);

void create_unsignedpp (int m, int n,
			string**& pp_matrix,
			int& pp_dim1,
			int& pp_dim2,
			std::queue<string>& verilog);


void create_br2pp (int m, int n, bool signed_mul,
                   string**& pp_matrix,
                   int& pp_dim1,
                   int& pp_dim2,
		   std::list<int>& extra_ones_indices,
		   bool allowXes,
                   std::queue<string>& verilog);

void create_br4pp (int m, int n, bool signed_mul,
                   string**& pp_matrix,
                   int& pp_dim1,
                   int& pp_dim2,
		   std::list<int>& extra_ones_indices,
		   bool allowXes,
                   std::queue<string>& verilog);

void create_br8pp (int m, int n, bool signed_mul,
		   string**& pp_matrix,
		   int& pp_dim1,
		   int& pp_dim2,
		   std::list<int>& extra_ones_indices,
		   string final_stage_adder,
		   int& pp_adder_size,
		   bool allowXes,
		   std::queue<string>& verilog);

void create_br16pp (int m, int n, bool signed_mul,
		    string**& pp_matrix,
		    int& pp_dim1,
		    int& pp_dim2,
		    std::list<int>& extra_ones_indices,
		    string final_stage_adder,
		    int& pp_adder_size,
		    bool allowXes,
		    std::queue<string>& verilog);
