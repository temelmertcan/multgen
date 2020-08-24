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
		       int out_size,
		       bool create_fin_adder,
		       bool signed_mult,
		       std::queue<string>& verilog,
		       int& adder_size
		       ){

  std::queue<string>* main_queue = new std::queue<string>[out_size];
  std::queue<string>* temp_queue = new std::queue<string>[out_size];

  // create the initial queue
  for (int i = 0; i < pp_dim1; i++)
    for (int j = 0; j < pp_dim2 && j< out_size; j++)
      if (pp_matrix[i][j] != "")
	main_queue[j].push (pp_matrix[i][j]);

  int cnt = 0;

  int* coeffs = 0;
  int coeffs_size;
  
  get_dadda_coeffs(get_max_queue_size(main_queue, out_size), coeffs, coeffs_size);
  

  //print_queue(main_queue, out_size);

  
  
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
			    verilog,
			    adder_size);
  else
    create_two_result_vectors (main_queue,
			       out_size,
			       verilog,
			       signed_mult,
			       adder_size);
  

   delete[] temp_queue;
   //delete[] coeffs;
  // delete[] main_queue;

   
  // cout << "adder size " << adder_size << endl;

}
