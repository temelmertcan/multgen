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

using namespace std;

#include "adders.h"

#include "pp_gen.h"

#include "wallace.h"

#include "dadda.h"

void get_dadda_coeffs (int max, int *& coeffs, int& size){

  size = 0;
  for (int i = 2; i<max; i = int(i*1.5)){
    size++;
  }

  coeffs = new int[size];

  int cnt = 0;
  for (int i = 2; i<max; i = int(i*1.5)){
    coeffs[size - ++cnt] = i;
  }

}
  
void print_queue (std::queue<string>* q,
		  int size){
  
  for (int i = 0; i<size; i++){

    std::queue<string> temp;

    while (!q[i].empty()){
      cout << q[i].front() << "\t";
      temp.push(q[i].front());
      q[i].pop();
    }

    q[i] = temp;
    cout << endl;
  }

  cout << endl;

}

void create_daddatree (string** pp_matrix,
		       string final_stage_adder,
		       int pp_dim1,
		       int pp_dim2,
		       int& out_size,
		       int shift_amount,
		       bool create_fin_adder,
		       bool signed_mult,
		       std::queue<string>& verilog,
		       int& adder_size,
		       bool**& zeros_in_output
		       ){


  //out_size = create_fin_adder ? out_size : pp_dim2;

  
  std::queue<string>* main_queue = new std::queue<string>[out_size];
  std::queue<string>* temp_queue = new std::queue<string>[out_size];

  // create the initial queue
  //for (int i = pp_dim1; i >= 0; i--)
  for (int i = 0; i < pp_dim1; i++)
    for (int j = 0; j < pp_dim2 && j< out_size; j++)
      if (pp_matrix[i][j] != "")
	main_queue[j].push (pp_matrix[i][j]);

  int cnt = 0;

  int* coeffs = 0;
  int coeffs_size;
  
  get_dadda_coeffs(get_max_queue_size(main_queue, out_size), coeffs, coeffs_size);
  


  // cout << "out_size " << out_size << endl;
  
  //print_queue(main_queue, out_size);

  //cout << "Dadda summmation stage count: " << coeffs_size << endl; 
  
  
  for (int phase = 0; phase < coeffs_size; phase++) {
    
    // Step 1: sum the stuff in main_queue with fa/ha
    verilog.push("");
    verilog.push("// Dadda Summation Stage " + to_string(phase+1));
    int carrysize = 0;
    
    for (int j = 0; j < out_size; j++){
      std::queue<string>& cur = main_queue[j];
      int sumsize = 0;
      while (((cur.size() + carrysize + sumsize) > coeffs[phase]) && (cur.size() >= 2)){
	if ((cur.size() + carrysize + sumsize) - coeffs[phase] > 1){
	  string var1 = cur.front();
	  cur.pop();
	  string var2 = cur.front();
	  cur.pop();
	  string var3 = cur.front();
	  cur.pop();
	  string inst = "logic s" + to_string(cnt)
	    + " ,c" + to_string(cnt) + "; \n";
	  inst += "    fa fa" + to_string(cnt)
	    + " (" + var1 + ", " + var2 + ", " + var3
	    + ", s" + to_string(cnt) + ", c" + to_string(cnt) + ");";

	  verilog.push(inst);

	  temp_queue[j].push("s" + to_string(cnt));

	  if (j+1 < out_size)
	    temp_queue[j+1].push("c" + to_string(cnt));
	  cnt++;
	  

	}else{
	  string var1 = cur.front();
	  cur.pop();
	  string var2 = cur.front();
	  cur.pop();
	  string inst = "logic s" + to_string(cnt)
	    + " ,c" + to_string(cnt) + ";\n";
	  inst += "    ha ha" + to_string(cnt)
	    + " (" + var1 + ", " + var2
	    + ", s" + to_string(cnt) + ", c" + to_string(cnt) + ");";

	  verilog.push(inst);

	  temp_queue[j].push("s" + to_string(cnt));

	  if (j+1 < out_size)
	    temp_queue[j+1].push("c" + to_string(cnt));
	  cnt++;
	}
	sumsize++;
      }
      carrysize = sumsize;
    }

    // Step 2; Move stuff from temp_queue to main_queue
    move_to_main_queue(temp_queue, main_queue, out_size);

    //print_queue(main_queue, out_size);

  }

  verilog.push("");

  if (create_fin_adder)
    create_finaladder_inst (main_queue,
			    final_stage_adder,
			    out_size,
			    shift_amount,
			    verilog,
			    adder_size);
  else
    create_two_result_vectors (main_queue,
			       out_size,
			       verilog,
			       signed_mult,
			       adder_size,
			       zeros_in_output);
  

   delete[] temp_queue;
   //delete[] coeffs;
  // delete[] main_queue;

   
  // cout << "adder size " << adder_size << endl;

}
