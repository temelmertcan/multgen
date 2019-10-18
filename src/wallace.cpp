#include <iostream>
#include <queue>          // std::queue
#include <fstream>

using namespace std;

#include "adders.h"

#include "pp_gen.h"

#include "wallace.h"


void move_to_main_queue (queue<string>* temp,
			 queue<string>* main,
			 int size){
  for (int i = 0; i < size; i++){
    while(!temp[i].empty()){
      string cur = temp[i].front();
      temp[i].pop();
      main[i].push (cur);
    }
  }
}

int get_max_queue_size (std::queue<string>* q, int size){

  int max=0;
  for (int i = 0; i < size; i++){
    int cur = q[i].size();
    if (max<cur)
      max = cur;
  }

  return max;
}

void create_finaladder_inst (std::queue<string>*& main_queue,
			     string final_stage_adder,
			     int out_size,
			     std::queue<string>& verilog,
			     int& adder_size) {

  int final_offset = 0;
  int final_size = 0;
  bool flag = false;

  string final_in1 = " }";
  string final_in2 = " }";

  std::queue<string> fin1;
  std::queue<string> fin2;

  for (int j = 0; j < out_size; j++){
    if (flag || main_queue[j].size()>1) {
      flag = true;
      fin1.push (main_queue[j].empty() ? "1'b0" : main_queue[j].front());
      // final_in1 += (main_queue[j].empty() ? "1'b0" : main_queue[j].front());
      main_queue[j].pop();
      fin2.push (main_queue[j].empty() ? "1'b0" : main_queue[j].front());
      //final_in2 += (main_queue[j].empty() ? "1'b0" : main_queue[j].front());
      main_queue[j].pop();
      final_size++;
    } else {
      final_offset++;
      string cur = "assign result[" + to_string(j) + "] = "
	+ main_queue[j].front() + ";";
      main_queue[j].pop();
      verilog.push(cur);
    }
  }

  while (!fin1.empty()) {
    final_in1 = fin1.front() + final_in1;
    if (fin1.size() > 1)
      final_in1 = ", " + final_in1;
    else
      final_in1 = "{" + final_in1;
    fin1.pop();
  }

  while (!fin2.empty()) {
    final_in2 = fin2.front() + final_in2;
    if (fin2.size() > 1)
      final_in2 = ", " + final_in2;
    else
      final_in2 = "{" + final_in2;
    fin2.pop();
  }

  if (final_size > 0){
    string fin_inst = final_stage_adder + "_" + to_string (final_size)
      + " final_adder (" + final_in1 + ", " + final_in2;

    verilog.push
      ("logic ["
       + to_string(final_size)
       + ":0] adder_result;");

    fin_inst +=  ", adder_result );";

    verilog.push (fin_inst);

    verilog.push ("assign result[" + to_string(out_size-1) + ":"
		     + to_string(final_offset) + "] = adder_result[" +
		     to_string(out_size-1-final_offset) +":0];" );
  }

  adder_size = final_size;

}

void create_wallacetree (string** pp_matrix,
			 string final_stage_adder,
			 int pp_dim1,
			 int pp_dim2,
			 int out_size,
			 std::queue<string>& verilog,
			 int& adder_size){

  std::queue<string>* main_queue = new std::queue<string>[out_size];
  std::queue<string>* temp_queue = new std::queue<string>[out_size];

  // create the initial queue
  for (int i = 0; i < pp_dim1; i++)
    for (int j = 0; j < pp_dim2 && j< out_size; j++)
      if (pp_matrix[i][j] != "")
	main_queue[j].push (pp_matrix[i][j]);

  int cnt = 0;

  while (get_max_queue_size(main_queue, out_size) > 2) {
    // Step 1: sum the stuff in main_queue with fa/ha
    for (int j = 0; j < out_size; j++){
      std::queue<string>& cur = main_queue[j];
      while (cur.size() >= 2){
	if (cur.size() >= 3){
	  string var1 = cur.front();
	  cur.pop();
	  string var2 = cur.front();
	  cur.pop();
	  string var3 = cur.front();
	  cur.pop();
	  string inst = "logic s" + to_string(cnt)
	    + " ,c" + to_string(cnt) + "; \n";
	  inst += "fa fa" + to_string(cnt)
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
	  inst += "ha ha" + to_string(cnt)
	    + " (" + var1 + ", " + var2
	    + ", s" + to_string(cnt) + ", c" + to_string(cnt) + ");";

	  verilog.push(inst);

	  temp_queue[j].push("s" + to_string(cnt));

	  if (j+1 < out_size)
	    temp_queue[j+1].push("c" + to_string(cnt));
	  cnt++;
	}
      }
    }

    // Step 2; Move stuff from temp_queue to main_queue
    move_to_main_queue(temp_queue, main_queue, out_size);

  }

  create_finaladder_inst (main_queue,
  			  final_stage_adder,
  			  out_size,
  			  verilog,
  			  adder_size);

   delete[] temp_queue;
  // delete[] main_queue;

  // cout << "adder size " << adder_size << endl;

}
