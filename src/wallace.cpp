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

void print_main_queue (queue<string>* main,
		       int size){
  
  cout << "Printing main queue " << endl; 
  for (int i = 0; i < size; i++){
    cout << "Col " << i << ": ";
    queue<string> copyq = main[i];
    while (!copyq.empty()){
      cout << copyq.front() << " ";
      copyq.pop();
    }
    cout << endl;
  }

}

int get_max_queue_size (std::queue<string>* q, int size){

  int max=0;
  for (int i = 0; i < size; i++){
    int cur = q[i].size();
    if (max<cur)
      max = cur;
  }

  //cout << "!!! max queue size :"  <<max << endl;
  
  return max;
}

void create_two_result_vectors (std::queue<string>*& main_queue,
                                int out_size,
                                std::queue<string>& verilog,
                                bool signed_mult,
                                int& result_vector_size,
                                bool**& zeros_in_output){

  int final_offset = 0;
  int final_size = 0;
  bool flag = false;

  string final_in1 = " }";
  string final_in2 = " }";

  std::queue<string> fin1;
  std::queue<string> fin2;

  zeros_in_output = new bool*[2];
  for (int i = 0; i < 2; i++){
    zeros_in_output[i] = new bool[out_size];
    for (int j = 0; j < out_size; j++)
      zeros_in_output[i][j] = false;
  }
  

  //cout << "hereee" << endl;

  for (int j = 0; j < out_size; j++){

    if (flag || main_queue[j].size()>1) {
      flag = true;

      bool b = false;
      for (int i = j; main_queue[i].empty() && i<out_size; i++ )
        if (i==out_size-1)
          b=true;
      if(b)
        break;

      fin1.push (main_queue[j].empty() ? "1'b0" : main_queue[j].front());

      if(main_queue[j].empty())
	zeros_in_output[0][j] = true;

      // final_in1 += (main_queue[j].empty() ? "1'b0" : main_queue[j].front());

      if (!main_queue[j].empty())
        main_queue[j].pop();

      fin2.push (main_queue[j].empty() ? "1'b0" : main_queue[j].front());
      
      if(main_queue[j].empty())
	zeros_in_output[1][j] = true;
      
      //final_in2 += (main_queue[j].empty() ? "1'b0" : main_queue[j].front());
      if (!main_queue[j].empty())
        main_queue[j].pop();
      final_size++;

    } else {
      final_offset++;
      string cur = "assign result0[" + to_string(j) + "] = "
        + main_queue[j].front() + ";";
      main_queue[j].pop();
      verilog.push(cur);
      cur = "assign result1[" + to_string(j) + "] = 1'b0;";
      zeros_in_output[1][j] = true;
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

  verilog.push ("");

  if (final_size > 0){

    verilog.push ("assign result0[" + to_string(final_size+final_offset+-1) + ":"
                  + to_string(final_offset) + "] = " + final_in1 + ";");
    verilog.push ("assign result1[" + to_string(final_size+final_offset+-1) + ":"
                  + to_string(final_offset) + "] = " + final_in2 + ";");

  }

  result_vector_size = final_size+final_offset;

  for (int i = result_vector_size; i< out_size; i++)
    if (signed_mult && false){
      verilog.push("assign result0["+to_string(i)+"] = result0["+to_string(result_vector_size-1)+"];");
      verilog.push("assign result1["+to_string(i)+"] = result1["+to_string(result_vector_size-1)+"];");
    }else{
      zeros_in_output[0][i] = true;
      zeros_in_output[1][i] = true;
      verilog.push("assign result0["+to_string(i)+"] = 1'b0;");
      verilog.push("assign result1["+to_string(i)+"] = 1'b0;");
    }

}

void create_finaladder_inst (std::queue<string>*& main_queue,
                             string final_stage_adder,
                             int out_size, int shift_amount,
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

      bool b = false;
      for (int i = j; main_queue[i].empty() && i<out_size; i++ )
        if (i==out_size-1)
          b=true;
      if(b)
        break;

      fin1.push (main_queue[j].empty() ? "1'b0" : main_queue[j].front());

      // final_in1 += (main_queue[j].empty() ? "1'b0" : main_queue[j].front());

      if (!main_queue[j].empty())
        main_queue[j].pop();

      fin2.push (main_queue[j].empty() ? "1'b0" : main_queue[j].front());
      //final_in2 += (main_queue[j].empty() ? "1'b0" : main_queue[j].front());
      if (!main_queue[j].empty())
        main_queue[j].pop();
      final_size++;

    } else {
      final_offset++;
      string cur = "assign result[" + to_string(j) + "] = "
        + main_queue[j].front() + ";";
      main_queue[j].pop();
      if (j>=shift_amount)
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

    if (shift_amount>final_offset)
      verilog.push ("assign result[" + to_string(out_size-1) + ":"
                  + to_string(shift_amount) + "] = adder_result[" +
                  to_string(out_size-1-final_offset) +":"+to_string(shift_amount-final_offset)+"];" );
    else
      verilog.push ("assign result[" + to_string(out_size-1) + ":"
		    + to_string(final_offset) + "] = adder_result[" +
		    to_string(out_size-1-final_offset) +":"+to_string(0)+"];" );
  }

  adder_size = final_size;

}

void create_wallacetree (string** pp_matrix,
                         string final_stage_adder,
                         int pp_dim1,
                         int pp_dim2,
                         int out_size,
			 int shift_amount,
                         bool create_fin_adder,
                         bool signed_mult,
                         std::queue<string>& verilog,
                         int& adder_size,
                         bool**& zeros_in_output){

  std::queue<string>* main_queue = new std::queue<string>[out_size];
  std::queue<string>* temp_queue = new std::queue<string>[out_size];

  // create the initial queue
  //for (int i = pp_dim1-1; i >= 0; i--)
  for (int i = 0; i < pp_dim1; i++)
    for (int j = 0; j < pp_dim2 && j< out_size; j++)
      if (pp_matrix[i][j] != "")
        main_queue[j].push (pp_matrix[i][j]);

  int cnt = 0;
  int stage_cnt = 0;

  while (get_max_queue_size(main_queue, out_size) > 2) {
    // Step 1: sum the stuff in main_queue with fa/ha

    verilog.push("");
    verilog.push("// Wallace Summation Stage " + to_string(++stage_cnt));
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
      }
    }

    // Step 2; Move stuff from temp_queue to main_queue
    move_to_main_queue(temp_queue, main_queue, out_size);

  }

  verilog.push("");

  if (create_fin_adder)
    create_finaladder_inst (main_queue,
                            final_stage_adder,
                            out_size, shift_amount,
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
  // delete[] main_queue;

  // cout << "adder size " << adder_size << endl;

}


void compress_concat_vector (string* arr, int& size){

  const unsigned long max = std::string::npos;

  // cout << "Starting compress_concat_vector before:" << endl;
  // for (int i = 0 ; i <size ; i++)
  //   cout << arr[i] << " ";
  // cout << endl;
    
  
  try{
    
    int offset = 0;
    for (int i = 0; i<size-1-offset; i++){
      string& a = arr[i];
      string b = arr[i+1+offset];
      if (a.find("'b0") < max && b.find("'b0") < max){

	// !!
	//cout << "Merging zeros sizes: " << a.size() << " "<< b.size() << endl;
	//cout << "Merging zeros find res: " << a.find("'b0") << " "<< b.find("'b0") << endl;
	//cout << "Merging zeros: " << a << " "<< b << endl;
	
	int newcoef = stoi(a.substr(0,a.find("'b0"))) + stoi(b.substr(0,b.find("'b0")));
	a = to_string(newcoef) + "'b0";
	i--;
	offset++;
      }
      else if (a.find("[") <max && b.find("[") <max && a.find("]") <max && b.find("]")<max
	  && a.substr(0, a.find("[")) == b.substr(0, b.find("["))){

	int s1, e1;
	if (a.find(":")<max){
	  e1 = stoi(a.substr(a.find("[")+1,a.find(":")));
	  s1 = stoi(a.substr(a.find(":")+1,a.find("]")));
	}else {
	  s1 = stoi(a.substr(a.find("[")+1,a.find("]")));
	  e1 = s1;
	}

	int s2, e2;
	if (b.find(":")<max){
	  e2 = stoi(b.substr(b.find("[")+1,b.find(":")));
	  s2 = stoi(b.substr(b.find(":")+1,b.find("]")));
	}else {
	  s2 = stoi(b.substr(a.find("[")+1,b.find("]")));
	  e2 = s2;
	}
      
	if (s2 == (1 + e1)){
	  
	  // !!
	  //cout << "Merging vectors: " << a << " "<< b << endl;

	  
	  a = a.substr(0, a.find("[")) + "[" + to_string(e2) + ":" + to_string(s1) + "]";
	  i--;
	  offset++;
	} else {
	  arr[i+1] = b;
	}
      } else
	arr[i+1] = b;
    }

    size -= offset;
    

  }
  catch(exception &err)
    {
      cout<<"Conversion failure"<<endl;
      //cout << err << endl;
    }

  // for (int i = 0 ; i <size ; i++)
  //   cout << arr[i] << " ";
  // cout << endl;
  
}



void create_4to2tree (string** pp_matrix,
		      string final_stage_adder,
		      int pp_dim1,
		      int pp_dim2,
		      int out_size,
		      int shift_amount,
		      bool create_fin_adder,
		      bool signed_mult,
		      std::queue<string>& verilog,
		      int& adder_size,
		      bool**& zeros_in_output){

  std::queue<string>* main_queue = new std::queue<string>[out_size];
  std::queue<string>* temp_queue = new std::queue<string>[out_size];

  // create the initial queue
  //for (int i = pp_dim1-1; i >= 0; i--)
  for (int i = 0; i < pp_dim1; i++)
    for (int j = 0; j < pp_dim2 && j< out_size; j++)
      if (pp_matrix[i][j] != "")
        main_queue[j].push (pp_matrix[i][j]);

  int cnt = 0;
  int stage_cnt = 0;

  struct four2twoi {
    four2twoi(string var1,string var2,string var3,string var4,string var5,int col,int maxsize){
      in1 = new string[maxsize];
      in2 = new string[maxsize];
      in3 = new string[maxsize];
      in4 = new string[maxsize];
      in1[0]=var1;
      in2[0]=var2;
      in3[0]=var3;
      in4[0]=var4;
      cin=var5;
      this->col = col;
      this->size=1;
    }
    ~four2twoi(){
      delete in1; delete in2; delete in3; delete in4;
    }
    void add(string var1,string var2,string var3,string var4){
      in1[size] = var1;
      in2[size] = var2;
      in3[size] = var3;
      in4[size] = var4;
      size++;
      
    }
    int col = 0; // starting col num.
    int size = 0;
    string cin = "";
    string *in1,*in2,*in3,*in4;  
   };

  int max_q_size;
  //print_main_queue(main_queue, out_size);

  while ((max_q_size = get_max_queue_size(main_queue, out_size)) > 2) {

    
    // Step 1: sum the stuff in main_queue with fa/ha

    int start = -1;
    for (int j = 0; j < out_size; j++){
      std::queue<string>& cur = main_queue[j];
      if (cur.size()>=(max_q_size>5?5:max_q_size>4?4:3)){
	start = j; 
	break;
      }
    }
    if (start < 0) // should never happen bcs of the while condition.
      break;

    // !!
    //cout<< ("// 4to2 compressor tree Stage " + to_string(stage_cnt)) << endl;
    
    // !!
    //cout << "Starting column is " << start << endl;

    std::queue<four2twoi*> avl_four2twoi;
    std::queue<four2twoi*> used_four2twoi;
    
    
    
    verilog.push("");
    verilog.push("// 4to2 compressor tree Stage " + to_string(++stage_cnt));
    for (int j = start; j < out_size; j++){
      // !!
      //cout << "Working on j=" << j << endl;
      
      std::queue<string>& cur = main_queue[j];
      while (cur.size() >= 2){
	//cout << "cur.size() " << cur.size() << endl;
	string var1 = cur.front();
	cur.pop();
	string var2 = cur.front();
	cur.pop();
	string var3 = "1'b0";
	if (cur.size() >= 1){
	  var3 = cur.front();
	  cur.pop();
	}
	string var4 = "1'b0";
	if (cur.size() >= 1){
	  var4 = cur.front();
	  cur.pop();
	}

	if (avl_four2twoi.empty()){
	  string var5 = "1'b0";
	  if (cur.size() >= 1){
	    var5 = cur.front();
	    cur.pop();
	  }
	  //cout << "Creating a new start for:"<<var1<<var2<<var3<<var4<<var5<<endl;
	  four2twoi * f_obj = new four2twoi(var1,var2,var3,var4,var5,j,out_size);
	  used_four2twoi.push(f_obj);
	}else{
	  four2twoi * f_obj = avl_four2twoi.front();
	  avl_four2twoi.pop();
	  f_obj->add(var1,var2,var3,var4);
	  used_four2twoi.push(f_obj);
	}
      }
      //!!
      //cout << "Filling unused avls " <<endl;
      // unused available ones should be filled with zeros for this column
      while(!avl_four2twoi.empty()){
	string var1 = "1'b0";
	if (cur.size() >= 1){
	  var1 = cur.front();
	  cur.pop();
	}
	four2twoi * f_obj = avl_four2twoi.front();
	avl_four2twoi.pop();
	f_obj->add(var1,"1'b0","1'b0","1'b0");
	used_four2twoi.push(f_obj);
      }

      // Moving onto the next column, make all used now available.
      avl_four2twoi.swap(used_four2twoi);

      
    }

    // !!
    //cout << "Time to dump 4to2-compressor instances " << avl_four2twoi.size() << endl;
    
    // Time to dump 4to2-compressor instances
    while(!avl_four2twoi.empty()){

      // !!
      //cout << "Total # of compressors to add: " << avl_four2twoi.size() << endl;
      
      four2twoi * f_obj = avl_four2twoi.front();
      avl_four2twoi.pop();

      // get rid of trailing 1'b0 stuff.
      int trailing_size = 0;
      for (int i = f_obj->size-1; i>=0; i--){
	if (f_obj->in1[i]=="1'b0" && f_obj->in2[i]=="1'b0" && f_obj->in3[i]=="1'b0" && f_obj->in4[i]=="1'b0"){
	  trailing_size++;
	}else
	  break;
      }
      int localsize = f_obj->size - trailing_size;

      // !!
      //cout << "trailing_size: " << trailing_size << endl;


      string* in1 = f_obj->in1;
      string* in2 = f_obj->in2;
      string* in3 = f_obj->in3;
      string* in4 = f_obj->in4;
      int in1size = localsize;
      int in2size = localsize;
      int in3size = localsize;
      int in4size = localsize;

      // !!
      //cout << "merge input vector for a more readable and compact verilog output.... " << endl;
      
      // merge input vector for a more readable and compact verilog output.
      compress_concat_vector(in1, in1size);
      // !!
      // cout << "merge in2 for a more readable and compact verilog output.... " << endl;
      compress_concat_vector(in2, in2size);
       // !!
      //cout << "merge in3 for a more readable and compact verilog output.... " << endl;
      compress_concat_vector(in3, in3size);
       // !!
      //cout << "merge in4 for a more readable and compact verilog output.... " << endl;
      compress_concat_vector(in4, in4size);

      verilog.push("");
      verilog.push("wire cout" + to_string(cnt) + ";");
      verilog.push("wire [" + to_string(localsize-1) + ":0] sum" + to_string(cnt) + ";");
      verilog.push("wire [" + to_string(localsize-1) + ":0] carry" + to_string(cnt) + ";");
      verilog.push("Four2Two #("+to_string(localsize)+") cmp42_"+to_string(cnt)+"(");
      verilog.push("indent");
      verilog.push("indent");
      string inst = ".in1({";
      for (int k = in1size-1; k>=0; k--){
	inst += in1[k] + (k>0 ? ", ":"}),");
      }
      verilog.push(inst);
      inst = ".in2({";
      for (int k = in2size-1; k>=0; k--){
	inst += in2[k] + (k>0 ? ", ":"}),");
      }
      verilog.push(inst);
      inst = ".in3({";
      for (int k = in3size-1; k>=0; k--){
	inst += in3[k] + (k>0 ? ", ":"}),");
      }
      verilog.push(inst);
      inst = ".in4({";
      for (int k = in4size-1; k>=0; k--){
	inst += in4[k] + (k>0 ? ", ":"}),");
      }
      verilog.push(inst);
      verilog.push(".cin("+f_obj->cin+"),");
      verilog.push(".sum(sum" + to_string(cnt) + "),");
      verilog.push(".carry(carry" + to_string(cnt) + "),");
      verilog.push(".cout(cout" + to_string(cnt) + "));");
      verilog.push("outdent");
      verilog.push("outdent");

      // Now put the new result in temp_queue
      for (int i = 0; i<localsize; i++){
	int c = i+f_obj->col;
	if (c < out_size)
	  temp_queue[c].push("sum" + to_string(cnt) + "[" + to_string(i) + "]");
	if (c+1 < out_size){
	  temp_queue[c+1].push("carry" + to_string(cnt) + "[" + to_string(i) + "]");
	  if (i == localsize-1)
	    temp_queue[c+1].push("cout" + to_string(cnt));
	}
      }

      //delete f_obj; f_obj=NULL;
      cnt++;
      
    }
     	
    // Step 2; Move stuff from temp_queue to main_queue
    move_to_main_queue(temp_queue, main_queue, out_size);

    //print_main_queue(main_queue, out_size);

  }

  verilog.push("");

  if (create_fin_adder)
    create_finaladder_inst (main_queue,
                            final_stage_adder,
                            out_size, shift_amount,
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
  // delete[] main_queue;

  // cout << "adder size " << adder_size << endl;

}
