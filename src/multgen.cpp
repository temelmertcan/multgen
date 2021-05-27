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

#include <string>
#include <sstream>
#include <cstring>

#include <cmath>

using namespace std;

#include "adders.h"

#include "pp_gen.h"

#include "wallace.h"

#include "dadda.h"



void enter_license (queue<string>& verilog) {
  
  verilog.push("// Note: The license below is based on the template at:");
  verilog.push("// http://opensource.org/licenses/BSD-3-Clause");

  verilog.push("// Copyright (C) 2020 Regents of the University of Texas");
  verilog.push("//All rights reserved.");
  verilog.push("");
  verilog.push("// Redistribution and use in source and binary forms, with or without");
  verilog.push("// modification, are permitted provided that the following conditions are");
  verilog.push("// met:");
  verilog.push("");
  verilog.push("// o Redistributions of source code must retain the above copyright");
  verilog.push("//   notice, this list of conditions and the following disclaimer.");
  verilog.push("");
  verilog.push("// o Redistributions in binary form must reproduce the above copyright");
  verilog.push("//   notice, this list of conditions and the following disclaimer in the");
  verilog.push("//   documentation and/or other materials provided with the distribution.");
  verilog.push("");
  verilog.push("// o Neither the name of the copyright holders nor the names of its");
  verilog.push("//   contributors may be used to endorse or promote products derived");
  verilog.push("//   from this software without specific prior written permission.");
  verilog.push("");
  verilog.push("// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS");
  verilog.push("// \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT");
  verilog.push("// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR");
  verilog.push("// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT");
  verilog.push("// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,");
  verilog.push("// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT");
  verilog.push("// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,");
  verilog.push("// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY");
  verilog.push("// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT");
  verilog.push("// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE");
  verilog.push("// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.");
  verilog.push("");
  verilog.push("// Original Author(s):");
  verilog.push("// Mertcan Temel         <mert@utexas.edu>");

  verilog.push("// This file is generator by a multiplier generator which can be downloaded at https://github.com/temelmertcan/multgen.");

  verilog.push("");
}




void print_strings (string* s, int size){
  for (int i = 0; i<size; i++)
    cout << s[i] << endl;
}

string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

int interact_with_user (int argc, char **argv,
			int& in1_size,
			int& in2_size,
			int& in3_size,
			int& dot_size,
			int& out_size,
			bool& signed_mult,
			string& final_stage_adder,
			string& pp_encoding,
			string& tree,
			string& main_type){



  if (argc == 2 && strcmp(argv[1], "-def") == 0) {
     in1_size = 8;
     in2_size = 8;
     final_stage_adder = "KS";
     pp_encoding = "SSP";
     tree = "WT";
  }
  else if (argc == 6){
    sscanf(argv[1], "%d", &in1_size);
    sscanf(argv[2], "%d", &in2_size);
    pp_encoding = argv[3];
    tree = argv[4];
    final_stage_adder = argv[5];
  }
  else if (argc <= 1) {

    string s = "";

    while(1) {
      
      cout << "1. Stand-alone (Isolated) Multiplier  " << endl;
      cout << "2. MAC (a * b + c) " << endl;
      cout << "3. Merged Four Multipliers " << endl;     
      cout << "4. Dot Product " << endl;
      cout << "5. Only a Vector Adder " << endl;
      //cout << "2.  " << endl;
      
      cout << "Select Multiplier Type: ";
      cin >> s;
      if (s.compare ("1") == 0) {
	main_type = "StandAlone";
	break;
      } else if (s.compare ("2") == 0)  {
	main_type = "MAC";
	break;
      } else if (s.compare ("3") == 0)  {
	main_type = "FourMult";
	break;
      } else if (s.compare ("4") == 0)  {
	main_type = "DOT";
	break;
      } else if (s.compare ("5") == 0)  {
	main_type = "Adder";	
	break;
      } else
	cout << "Invalid Selection!" << endl;

    }
    
    while (main_type.compare("Adder")!=0) {

      cout << "1. Wallace Tree " << endl;
      cout << "2. Dadda Tree " << endl;

      cout << "Select Summation Tree Algorithm: ";
      cin >> s;
      if (s.compare ("1") == 0) {
	tree = "WT";
	break;
      }else if (s.compare ("2") == 0)  {
	tree = "DT";
	break;
      } else
	cout << "Invalid Selection!" << endl;

    }

    while (main_type.compare("Adder")!=0) {

      cout << "1. Signed " << endl;
      cout << "2. Unsigned" << endl;

      cout << "Signed or Unsigned Multiplication?: ";
      cin >> s;
      if (s.compare ("1") == 0) {
	signed_mult = true;
	break;
      }else  if (s.compare ("2") == 0) {
	signed_mult = false;
	break;
      }else
	cout << "Invalid Selection!" << endl;

    }

    
    while (main_type.compare("Adder")!=0) {

      cout << "1. Simple" << endl;
      cout << "2. Booth radix-2" << endl;
      cout << "3. Booth radix-4" << endl;

      cout << "Select Partial Product Generation Algorithm: ";
      cin >> s;
      if (s.compare ("1") == 0) {
	pp_encoding = "SP";
	break;
      }else  if (s.compare ("2") == 0) {
	pp_encoding = "B2";
	break;
      }else if (s.compare ("3") == 0)  {
	pp_encoding = "B4";
	break;
      }else
	cout << "Invalid Selection!" << endl;

    }

    if (signed_mult)
	pp_encoding = "S" + pp_encoding;
    else
      	pp_encoding = "U" + pp_encoding;
    
    while(1) {

      cout << "1. Ripple Carry Adder " << endl;
      cout << "2. Han-Carlson Adder " << endl;
      cout << "3. Ladner-Fischer Adder " << endl;
      cout << "4. Kogge-Stone Adder " << endl;
      //cout << "3. Booth radix-4 Signed " << endl;

      cout << "Select Final Stage Adder Algorithm: ";
      cin >> s;
      if (s.compare ("1") == 0) {
	final_stage_adder = "RP";
	break;
      }else if (s.compare ("2") == 0)  {
	final_stage_adder = "HC";
	break;
      }else if (s.compare ("3") == 0)  {
	final_stage_adder = "LF";
	break;
      } else if (s.compare ("4") == 0)  {
	final_stage_adder = "KS";
	break;
      }
      else
	cout << "Invalid Selection!" << endl;

    }

    if (main_type.compare("Adder")==0) {
      cout << "Enter input size: ";
      cin >> in1_size;
      return 0;
    }
    
    if (main_type.compare("StandAlone") == 0){
      cout << "Enter IN1 (Multiplier) size: ";
      cin >> in1_size;

      cout << "Enter IN2 (Multiplicand) size: ";
      cin >> in2_size;

      cout << "Enter Output size (any value less than \""<< in1_size+in2_size << "\" will truncate the result): ";
      string in = "";
      cin >> in;
      if (std::stringstream(in) >> out_size) {
      }
      else out_size = in1_size+in2_size;

      if (out_size > in1_size+in2_size)
	out_size = in1_size+in2_size;
    }

    if (main_type.compare("MAC") == 0){
      cout << "Enter IN1 (Multiplier) size: ";
      cin >> in1_size;

      cout << "Enter IN2 (Multiplicand) size: ";
      cin >> in2_size;

      cout << "Enter IN3 (Addend) size: ";
      cin >> in3_size;

      cout << "Enter Output size (any value less than \""
	   << max(in1_size+in2_size+1,in3_size+1)
	   << "\" will truncate the result): ";
      string in = "";
      cin >> in;
      if (std::stringstream(in) >> out_size) {
      }
      else out_size = max(in1_size+in2_size+1,in3_size+1);

      if (out_size > max(in1_size+in2_size+1,in3_size+1))
	out_size = max(in1_size+in2_size+1,in3_size+1);
    }

    if (main_type.compare("FourMult") == 0){
      while(1){
	cout << "Enter input vector (IN1 multiplier and IN2 multiplicand) size: ";
	cin >> in1_size;
	in2_size = in1_size;
	//out_size = in1_size + in2_size;
	if ((in1_size & 1) == 0)
	  break;
	cout << "Input size must be divisible by 2. ";
      }

      cout << "Enter IN3 (Addend, number to be added after multiplication) size (enter 0 to omit): ";
      cin >> in3_size;


      cout << "Enter Output size (any value less than \""
	   << max(in1_size+in2_size+(in3_size>0?1:0),in3_size+1)
	   << "\" will truncate the result): ";
      string in = "";
      cin >> in;
      if (std::stringstream(in) >> out_size) {
      }
      else out_size = max(in1_size+in2_size+(in3_size>0?1:0),in3_size+1);
      
      if (out_size > max(in1_size+in2_size+(in3_size>0?1:0),in3_size+1))
	out_size = max(in1_size+in2_size+(in3_size>0?1:0),in3_size+1); 
    }

    if (main_type.compare("DOT") == 0){
      cout << "Enter IN1_i (Multiplier) size: ";
      cin >> in1_size;

      cout << "Enter IN2_i (Multiplicand) size: ";
      cin >> in2_size;

      cout << "Enter dot product size: ";
      cin >> dot_size;

      if(dot_size<1) {
	cout << "Dot product size should be a positive number." << endl;
	return 1;
      }

      cout << "Enter IN3 (Addend, number to be added after multiplication) size (enter 0 to omit): ";
      cin >> in3_size;

      cout << "Enter Output size (any value less than \""
	   << max(in1_size+in2_size+int((dot_size+1)/2)+(in3_size>0&&(dot_size&1)==0?1:0),in3_size+1)
	   << "\" will truncate the result): ";
      string in = "";
      cin >> in;
      if (std::stringstream(in) >> out_size) {
      }
      else out_size = max(in1_size+in2_size+int((dot_size+1)/2)+(in3_size>0&&(dot_size&1)==0?1:0),in3_size+1);

      if (out_size > max(in1_size+in2_size+int((dot_size+1)/2)+(in3_size>0&&(dot_size&1)==0?1:0),in3_size+1))
	out_size = max(in1_size+in2_size+int((dot_size+1)/2)+(in3_size>0&&(dot_size&1)==0?1:0),in3_size+1);
    }
    
  } else {
    for (int i = 0; i< argc; i++){
      cout << "Arg " << i << ": " << argv[i] << endl;
    }
    cout << "bad arguments!" << endl;
    return 1;
  }

  return 0;

}



int create_mult ( int  in1_size,
		  int  in2_size,
		  int  out_size,
		  string final_stage_adder,
		  string pp_encoding,
		  string tree,
		  bool create_fin_adder,
		  string& module_name,
		  queue<string>& verilog,
		  int& adder_size){


  int pp_dim1, pp_dim2;
  string** pp_matrix;
  
   module_name = tree + "_" + pp_encoding + "_"
     + (create_fin_adder?final_stage_adder + "_" : "")
		+ to_string(in1_size) + "x"
		+ to_string(in2_size)
     + (out_size != (in2_size+in1_size) ? "_" + to_string(out_size) : "" );

  verilog.push ("module " + module_name + "(");
  verilog.push("indent");
  verilog.push("indent");
  verilog.push ("input logic [" + to_string(in1_size - 1) + ":0] IN1," );
  verilog.push ("input logic [" + to_string(in2_size - 1) + ":0] IN2," );
  if (create_fin_adder)
    verilog.push ("output logic [" + to_string(out_size - 1) + ":0] result);" );
  else
    {
      verilog.push ("output logic [" + to_string(out_size - 1) + ":0] result0," );
      verilog.push ("output logic [" + to_string(out_size - 1) + ":0] result1);" );
    }
  verilog.push("outdent");
  verilog.push("");

  verilog.push("\n// Creating Partial Products \n");

  bool signed_mult = true;
  
  if (pp_encoding.compare ("USP") == 0) {
    create_unsignedpp (in1_size, in2_size, pp_matrix,
		       pp_dim1, pp_dim2, verilog);
    signed_mult = false;
  } else if (pp_encoding.compare ("SSP") == 0) {
    create_signedpp (in1_size, in2_size, pp_matrix,
		     pp_dim1, pp_dim2, verilog);
  } else if (pp_encoding.compare ("SB4") == 0) {
    create_signedbr4pp (in1_size, in2_size, pp_matrix,
			pp_dim1, pp_dim2, verilog);
  } else if (pp_encoding.compare ("SB2") == 0) {
    create_signedbr2pp (in1_size, in2_size, pp_matrix,
			pp_dim1, pp_dim2, verilog);
  } else if (pp_encoding.compare ("UB2") == 0) {
    create_unsignedbr2pp (in1_size, in2_size, pp_matrix,
			pp_dim1, pp_dim2, verilog);
    signed_mult = false;
  } else if (pp_encoding.compare ("UB4") == 0) {
    create_unsignedbr4pp (in1_size, in2_size, pp_matrix,
			  pp_dim1, pp_dim2, verilog);
    signed_mult = false;
  } else {
    cout << "Bad PP Selection!" << endl;
    return 1;
  }

  verilog.push("\n// Creating Summation Tree \n");

  if (tree.compare ("WT") == 0) {
    create_wallacetree (pp_matrix, final_stage_adder, pp_dim1, pp_dim2,
			out_size,
			create_fin_adder,
			signed_mult,
			verilog,
			adder_size);

  } else if (tree.compare ("DT") == 0){
     create_daddatree (pp_matrix, final_stage_adder, pp_dim1, pp_dim2,
		       out_size,
		       create_fin_adder,
		       signed_mult,
		       verilog,
		       adder_size);
  } else {
    cout << "Bad Summation tree Selection!" << endl;
    return 1;
  }

  verilog.push("outdent");

  verilog.push ("endmodule\n\n");

  verilog.push("");


  cout << endl;
  cout << "Multiplier Module (" << module_name << ") is created." << endl;
  cout << "   Inputs: IN1[" << in1_size-1 << ":0], IN2[" << in2_size-1 << ":0]" << endl;
  if (create_fin_adder){
    cout << "   Output: result[" << out_size-1 << ":0]" << endl;
    cout << "   Function: result = IN1 * IN2 " << (signed_mult?"(signed)":"(unsigned)") << endl;
  }else{
    cout << "   Outputs: result0[" << out_size-1 << ":0], result1[" << out_size-1 << ":0]" << endl;
    cout << "   Function: result0+result1 = IN1 * IN2 " << (signed_mult?"(signed)":"(unsigned)") << endl;
  }
  return 0;

}


int create_adder (string final_stage_adder,
		  int adder_size,
		  queue<string>& verilog){

  if (final_stage_adder.compare ("RP") == 0)
    create_rp_adder (adder_size, verilog);
  else if (final_stage_adder.compare ("HC") == 0)
    create_hc_adder (adder_size, verilog);
  else if (final_stage_adder.compare ("LF") == 0)
    create_lf_adder (adder_size, verilog);
  else if (final_stage_adder.compare ("KS") == 0)
    create_ks_adder (adder_size, verilog);
  else{
    cout << "Bad Final Stage Adder Selection!" << endl;
    return 1;
  }

  cout << endl;
  cout << "Adder Module (" << final_stage_adder << "_" << adder_size << ") is created." << endl;
  cout << "   Inputs: IN1[" << adder_size-1 << ":0], IN2[" << adder_size-1 << ":0]" << endl;
  cout << "   Output: result[" <<adder_size << ":0]" << endl;
  cout << "   Function: result=IN1+IN2" << endl;

  return 0;

}


int create_four_mult (int  in_size,
		      int  in3_size,
		      int  out_size,
		      string final_stage_adder,
		      string pp_encoding,
		      string tree,
		      string& module_name,
		      queue<string>& verilog,
		      int& adder_size){

  bool signed_mult =
    (pp_encoding.compare ("SSP") == 0) ||
    (pp_encoding.compare ("SB4") == 0) ||
    (pp_encoding.compare ("SB2") == 0);

  //int out_size = max(2*in_size+(in3_size>0?1:0), in3_size+1);
  int in1_size = in_size;
  int in2_size = in_size;
  
  int one_mult_in_size = in_size/2+1;//(signed_mult?1:0);
  int one_mult_out_size = one_mult_in_size*2;

  string one_mult_module_name;
  int to_be_ignored;
  
  int retval = create_mult (one_mult_in_size,
			    one_mult_in_size,
			    one_mult_out_size,
			    "",
			    pp_encoding,
			    tree,
			    false,
			    one_mult_module_name,
			    verilog,
			    to_be_ignored);

  if (retval!=0)
    return retval;

  string FourMultMerger_module_name = "FourMultMerger_"
    + std::string(signed_mult ? "Signed_" : "Unsigned_")
    + to_string(in1_size) + "x"
    + to_string(in2_size)
    + (in3_size>0 ? "_plus_" + to_string(in3_size) : "")
    + (out_size != (max(in1_size+in2_size+(in3_size>0?1:0),in3_size+1)) ? "_" + to_string(out_size) : "" );

  verilog.push ("module " + FourMultMerger_module_name + "(");
  verilog.push("indent");
  verilog.push("indent");
  verilog.push ("input logic [" + to_string(one_mult_out_size-1) + ":0] m1_0,");
  verilog.push ("input logic [" + to_string(one_mult_out_size-1) + ":0] m1_1,");
  verilog.push ("input logic [" + to_string(one_mult_out_size-1) + ":0] m2_0,");
  verilog.push ("input logic [" + to_string(one_mult_out_size-1) + ":0] m2_1,");
  verilog.push ("input logic [" + to_string(one_mult_out_size-1) + ":0] m3_0,");
  verilog.push ("input logic [" + to_string(one_mult_out_size-1) + ":0] m3_1,");
  verilog.push ("input logic [" + to_string(one_mult_out_size-1) + ":0] m4_0,");
  verilog.push ("input logic [" + to_string(one_mult_out_size-1) + ":0] m4_1,");
  if (in3_size>0)
    verilog.push ("input logic [" + to_string(in3_size-1) + ":0] IN3,");
  verilog.push ("output logic [" + to_string(out_size - 1) + ":0] result);" );
  verilog.push("outdent");

  string** pp_matrix;
  int pp_dim1 = 0, pp_dim2 = 0;
  

  if (!signed_mult && false){
    
     pp_dim1 = 10;
     pp_dim2 = out_size;
     pp_matrix = new string*[pp_dim1];
     int extra_one_count = 0;
      
     for (int i = 0; i < pp_dim1; i++)
       pp_matrix[i] = new string[pp_dim2];
     
     for (int i = 0;  i < pp_dim2; i++){
       if (i<one_mult_out_size){
	 pp_matrix[0][i] = "m1_0[" + to_string(i) + "]";
	 pp_matrix[1][i] = "m1_1[" + to_string(i) + "]";
       } else if (i==one_mult_out_size)
	 extra_one_count ++;

     
       if (i>=one_mult_in_size && i<(one_mult_out_size+one_mult_in_size)){
	 pp_matrix[2][i] = "m2_0[" + to_string(i-one_mult_in_size) + "]";
	 pp_matrix[3][i] = "m2_1[" + to_string(i-one_mult_in_size) + "]";
	 pp_matrix[4][i] = "m3_0[" + to_string(i-one_mult_in_size) + "]";
	 pp_matrix[5][i] = "m3_1[" + to_string(i-one_mult_in_size) + "]";
       }
       else if ( i==(one_mult_out_size+one_mult_in_size))
	 extra_one_count+=2;
	 
       if (i>=one_mult_out_size ){
	 pp_matrix[6][i] = "m4_0[" + to_string(i-one_mult_out_size) + "]";
	 pp_matrix[7][i] = "m4_1[" + to_string(i-one_mult_out_size) + "]";
       }
      
       
       if (i<in3_size) {
	 pp_matrix[8][i] = "IN3["+to_string(i)+"]";
       }

       if ((extra_one_count&1)==1){
	 pp_matrix[9][i] = "const_1";
	 extra_one_count++;
       }
       extra_one_count= extra_one_count>>1;

       
     }

    
    
  } else {
   
     
     verilog.push ( "wire logic const1;");
     verilog.push ( "assign const_1 = 1'b1;");
     
     pp_dim1 = 10;
     pp_dim2 = out_size;
     pp_matrix = new string*[pp_dim1];
     for (int i = 0; i < pp_dim1; i++)
       pp_matrix[i] = new string[pp_dim2];

     int extra_one_count = 0;
     for (int i = 0;  i < pp_dim2; i++){
       
       if (i<one_mult_out_size){
	 pp_matrix[0][i] = "m1_0[" + to_string(i) + "]";
	 pp_matrix[1][i] = "m1_1[" + to_string(i) + "]";
       } else if (i==one_mult_out_size)
	 extra_one_count ++;

       if (i>=one_mult_in_size-1){
	 if ( i<(one_mult_out_size+one_mult_in_size-1)){
	   bool flip_cond = false&&i==one_mult_out_size+one_mult_in_size-4;
	   pp_matrix[2][i] = std::string(flip_cond?"~":"") + "m2_0[" + to_string(i-one_mult_in_size+1) + "]";
	   pp_matrix[3][i] = std::string(flip_cond?"~":"") + "m2_1[" + to_string(i-one_mult_in_size+1) + "]";
	   pp_matrix[4][i] = std::string(flip_cond?"~":"") + "m3_0[" + to_string(i-one_mult_in_size+1) + "]";
	   pp_matrix[5][i] = std::string(flip_cond?"~":"") + "m3_1[" + to_string(i-one_mult_in_size+1) + "]";
	 } else if ( i==(one_mult_out_size+one_mult_in_size-1))
	   extra_one_count+=2;
       }
       if (i>=one_mult_out_size-2) {
	 if ( i < pp_dim2){
	   pp_matrix[6][i] = "m4_0[" + to_string(i-one_mult_out_size+2) + "]";
	   pp_matrix[7][i] = "m4_1[" + to_string(i-one_mult_out_size+2) + "]";
	 } else if ( i == pp_dim2)
	   extra_one_count++;
       }

       if (i < in3_size-1){
     	 pp_matrix[8][i] = "IN3[" + to_string(i) + "]";
       } else if (i==in3_size-1){
	 pp_matrix[8][i] = "~IN3[" + to_string(i) + "]";
     	 extra_one_count ++;
       }
       
       if ((extra_one_count&1)==1){
	 pp_matrix[9][i] = "const_1";
	 extra_one_count++;
       }
       extra_one_count= extra_one_count>>1;
     }
     //cout << "extra_one_count" << extra_one_count << endl;

  }


  print_pp(pp_matrix, pp_dim1, pp_dim2, verilog, true);

  if (tree.compare("WT") == 0)
    create_wallacetree (pp_matrix,
			final_stage_adder,
			pp_dim1,
			pp_dim2,
			out_size,
			true,
			signed_mult,
			verilog,
			adder_size);
  else
    create_daddatree (pp_matrix,
		      final_stage_adder,
		      pp_dim1,
		      pp_dim2,
		      out_size,
		      true,
		      signed_mult,
		      verilog,
		      adder_size);
  

   //print_pp(pp_matrix, pp_dim1, pp_dim2);

   verilog.push("outdent");
   verilog.push("");
   verilog.push("endmodule");


   

  module_name = "Merged_" + tree + "_" + pp_encoding + "_"
     + final_stage_adder + "_"
    + to_string(in1_size) + "x"
    + to_string(in2_size)
    + (in3_size>0 ? "_plus_" + to_string(in3_size) : "")
    + (out_size != (max(in1_size+in2_size+(in3_size>0?1:0),in3_size+1)) ? "_" + to_string(out_size) : "" );

  verilog.push ("module " + module_name + "(");
  verilog.push("indent");
  verilog.push("indent");
  verilog.push ("input logic [" + to_string(in1_size - 1) + ":0] IN1," );
  verilog.push ("input logic [" + to_string(in2_size - 1) + ":0] IN2," );
  if (in3_size>0)
    verilog.push ("input logic [" + to_string(in3_size - 1) + ":0] IN3," );
  verilog.push ("output logic [" + to_string(out_size - 1) + ":0] result);" );
  verilog.push("outdent");

  verilog.push ("wire logic [" + to_string(one_mult_out_size-1) + ":0] m1_0;");
  verilog.push ("wire logic [" + to_string(one_mult_out_size-1) + ":0] m1_1;");
  verilog.push ("wire logic [" + to_string(one_mult_out_size-1) + ":0] m2_0;");
  verilog.push ("wire logic [" + to_string(one_mult_out_size-1) + ":0] m2_1;");
  verilog.push ("wire logic [" + to_string(one_mult_out_size-1) + ":0] m3_0;");
  verilog.push ("wire logic [" + to_string(one_mult_out_size-1) + ":0] m3_1;");
  verilog.push ("wire logic [" + to_string(one_mult_out_size-1) + ":0] m4_0;");
  verilog.push ("wire logic [" + to_string(one_mult_out_size-1) + ":0] m4_1;");


  if(!signed_mult){

     verilog.push("");
    verilog.push
      (one_mult_module_name + " m1 ({1'b0, IN1["+to_string (one_mult_in_size-2)+":0]}, " +
       "{1'b0, IN2["+to_string (one_mult_in_size-2)+":0]}, m1_0, m1_1);");
    
    verilog.push
      (one_mult_module_name + " m2 (" + 
       "{1'b0, "+
       "IN2["+to_string (one_mult_in_size+one_mult_in_size-3)+":"+
       to_string(one_mult_in_size-1)+"]}, "+
       "{1'b0, IN1["+to_string (one_mult_in_size-2)+":0]}, " +
       "m2_0, m2_1);");

     verilog.push
      (one_mult_module_name + " m3 ({1'b0, "+
       "IN1["+to_string (one_mult_in_size+one_mult_in_size-3)+":"+
       to_string(one_mult_in_size-1)+"]}, " +
       "{1'b0, IN2["+to_string (one_mult_in_size-2)+":0]}, m3_0, m3_1);");

     verilog.push
      (one_mult_module_name + " m4 ({1'b0, "
       +"IN1["+to_string (one_mult_in_size+one_mult_in_size-3)+":"+
       to_string(one_mult_in_size-1)+"]}, " +
       "{1'b0, "
       +"IN2["+to_string (one_mult_in_size+one_mult_in_size-3)+":"+
       to_string(one_mult_in_size-1)+"]}, m4_0, m4_1);");

     verilog.push("");
    
    // verilog.push("");
    // verilog.push
    //   (one_mult_module_name + " m1 (IN1["+to_string (one_mult_in_size-1)+":0], " +
    //    "IN2["+to_string (one_mult_in_size-1)+":0], m1_0, m1_1);");
    
    // verilog.push
    //   (one_mult_module_name + " m2 (IN1["+to_string (one_mult_in_size-1)+":0], " +
    //    "IN2["+to_string (one_mult_in_size+one_mult_in_size-1)+":"+
    //    to_string(one_mult_in_size)+"], m2_0, m2_1);");

    //  verilog.push
    //   (one_mult_module_name + " m3 (IN1["+to_string (one_mult_in_size+one_mult_in_size-1)+":"+
    //    to_string(one_mult_in_size)+"], " +
    //    "IN2["+to_string (one_mult_in_size-1)+":0], m3_0, m3_1);");

    //  verilog.push
    //   (one_mult_module_name + " m4 (IN1["+to_string (one_mult_in_size+one_mult_in_size-1)+":"+
    //    to_string(one_mult_in_size)+"], " +
    //    "IN2["+to_string (one_mult_in_size+one_mult_in_size-1)+":"+
    //    to_string(one_mult_in_size)+"], m4_0, m4_1);");
    //  verilog.push("");

  } else {
     verilog.push("");
    verilog.push
      (one_mult_module_name + " m1 ({1'b0, IN1["+to_string (one_mult_in_size-2)+":0]}, " +
       "{1'b0, IN2["+to_string (one_mult_in_size-2)+":0]}, m1_0, m1_1);");
    
    verilog.push
      (one_mult_module_name + " m2 (" + 
       "{IN2["+to_string (one_mult_in_size+one_mult_in_size-3)+"], "+
       "IN2["+to_string (one_mult_in_size+one_mult_in_size-3)+":"+
       to_string(one_mult_in_size-1)+"]}, "+
       "{1'b0, IN1["+to_string (one_mult_in_size-2)+":0]}, " +
       "m2_0, m2_1);");

     verilog.push
      (one_mult_module_name + " m3 ({IN1["+to_string (one_mult_in_size+one_mult_in_size-3)+"], "+
       "IN1["+to_string (one_mult_in_size+one_mult_in_size-3)+":"+
       to_string(one_mult_in_size-1)+"]}, " +
       "{1'b0, IN2["+to_string (one_mult_in_size-2)+":0]}, m3_0, m3_1);");

     verilog.push
      (one_mult_module_name + " m4 ({IN1["+to_string (one_mult_in_size+one_mult_in_size-3)+"],"
       +"IN1["+to_string (one_mult_in_size+one_mult_in_size-3)+":"+
       to_string(one_mult_in_size-1)+"]}, " +
       "{IN2["+to_string (one_mult_in_size+one_mult_in_size-3)+"], "
       +"IN2["+to_string (one_mult_in_size+one_mult_in_size-3)+":"+
       to_string(one_mult_in_size-1)+"]}, m4_0, m4_1);");

     verilog.push("");
  }


  verilog.push
      (FourMultMerger_module_name
       + " merger (m1_0, m1_1, m2_0, m2_1, m3_0, m3_1, m4_0, m4_1,"
       + (in3_size>0 ? "IN3, " : "")
       + " result);");
   
  verilog.push("outdent");
  verilog.push("");
  verilog.push("endmodule");
   
  verilog.push("");

  cout << endl;
  cout << "Merged Multiplier Module (" << module_name << ") is created." << endl;
  cout << "   Inputs: IN1[" << in1_size-1 << ":0], IN2[" << in2_size-1 << ":0]";
  if (in3_size>0)
    cout << ", IN3[" << in3_size-1 << ":0]";
  cout << endl;
  cout << "   Output: result[" << out_size-1 << ":0]" << endl;
  cout << "   Function: result = IN1 * IN2 ";
  if (in3_size>0)
    cout << "+ IN3";
  cout << (signed_mult?"(signed)":"(unsigned)") << endl;

  

  
  return 0;
  
}




int create_fma (int  in1_size,
		int  in2_size,
		int  in3_size,
		int  out_size,
		string final_stage_adder,
		string pp_encoding,
		string tree,
		string& module_name,
		queue<string>& verilog,
		int& adder_size){

  bool signed_mult =
    (pp_encoding.compare ("SSP") == 0) ||
    (pp_encoding.compare ("SB4") == 0) ||
    (pp_encoding.compare ("SB2") == 0);



  string mult_module_name;
  int to_be_ignored;

  int mult_out_size = min(out_size, in1_size+in2_size);
  
  int retval = create_mult (in1_size,
			    in2_size,
			    mult_out_size,
			    "",
			    pp_encoding,
			    tree,
			    false,
			    mult_module_name,
			    verilog,
			    to_be_ignored);

  if (retval!=0)
    return retval;
  

  string MAC_Merger_module_name = "MAC_Merger_"
    + std::string(signed_mult ? "Signed_" : "Unsigned_")
    + to_string(in1_size) + "x"
    + to_string(in2_size) + "_"
    + to_string(in3_size)
    + (out_size != max(in1_size+in2_size+1,in3_size+1) ? "_" + to_string(out_size) : "" );

  verilog.push ("module " + MAC_Merger_module_name + "(");
  verilog.push("indent");
  verilog.push("indent");
  verilog.push ("input logic [" + to_string(mult_out_size-1) + ":0] m1_0,");
  verilog.push ("input logic [" + to_string(mult_out_size-1) + ":0] m1_1,");
  verilog.push ("input logic [" + to_string(in3_size-1) + ":0] IN3,");
  verilog.push ("output logic [" + to_string(out_size - 1) + ":0] result);" );
  verilog.push("outdent");

  string** pp_matrix;
  int pp_dim1 = 0, pp_dim2 = 0;
  

  if (!signed_mult){
    
     pp_dim1 = 3;
     pp_dim2 = out_size;
     pp_matrix = new string*[pp_dim1];
     for (int i = 0; i < pp_dim1; i++)
       pp_matrix[i] = new string[pp_dim2];
     
     for (int i = 0;  i < pp_dim2; i++){
       if (i<mult_out_size){
	 pp_matrix[0][i] = "m1_0[" + to_string(i) + "]";
	 pp_matrix[1][i] = "m1_1[" + to_string(i) + "]";
       }
       if (i < in3_size ){
	 pp_matrix[2][i] = "IN3[" + to_string(i) + "]";
       }
     }
  } else {
     verilog.push ( "wire logic const1;");
     verilog.push ( "assign const_1 = 1'b1;");
     
     pp_dim1 = 4;
     pp_dim2 = out_size;
     pp_matrix = new string*[pp_dim1];
     for (int i = 0; i < pp_dim1; i++)
       pp_matrix[i] = new string[pp_dim2];

     int extra_one_count = 0;
     for (int i = 0;  i < pp_dim2; i++){
       
       if (i < mult_out_size){
     	 pp_matrix[0][i] = "m1_0[" + to_string(i) + "]";
     	 pp_matrix[1][i] = "m1_1[" + to_string(i) + "]";
       } else if (i==mult_out_size)
     	 extra_one_count ++;
       if (i < in3_size-1){
     	 pp_matrix[2][i] = "IN3[" + to_string(i) + "]";
       } else if (i==in3_size-1){
	 pp_matrix[2][i] = "~IN3[" + to_string(i) + "]";
     	 extra_one_count ++;
       }
       
       if ((extra_one_count&1)==1){
     	 pp_matrix[3][i] = "const_1";
     	 extra_one_count++;
       }
       extra_one_count= extra_one_count/2;
     }
     //cout << "extra_one_count" << extra_one_count << endl;

  }


  print_pp(pp_matrix, pp_dim1, pp_dim2, verilog, true);

  if (tree.compare("WT") == 0)
    create_wallacetree (pp_matrix,
			final_stage_adder,
			pp_dim1,
			pp_dim2,
			out_size,
			true,
			signed_mult,
			verilog,
			adder_size);
  else
    create_daddatree (pp_matrix,
		      final_stage_adder,
		      pp_dim1,
		      pp_dim2,
		      out_size,
		      true,
		      signed_mult,
		      verilog,
		      adder_size);
  

   //print_pp(pp_matrix, pp_dim1, pp_dim2);

   verilog.push("outdent");
   verilog.push("");
   verilog.push("endmodule");

   verilog.push("");
   

   module_name = "MAC_" + tree + "_" + pp_encoding + "_"
     + final_stage_adder + "_"
     + to_string(in1_size) + "x"
     + to_string(in2_size) + "_plus_"
     + to_string(in3_size)
     + (out_size != max(in2_size+in1_size+1, in3_size+1) ? "_" + to_string(out_size) : "" );
   
   verilog.push ("module " + module_name + "(");
   verilog.push("indent");
   verilog.push("indent");
   verilog.push("input logic [" + to_string(in1_size - 1) + ":0] IN1," );
   verilog.push("input logic [" + to_string(in2_size - 1) + ":0] IN2," );
   verilog.push("input logic [" + to_string(in3_size - 1) + ":0] IN3," );
   verilog.push("output logic [" + to_string(out_size - 1) + ":0] result);" );
   verilog.push("outdent");
   
   verilog.push ("wire logic [" + to_string(mult_out_size-1) + ":0] m1_0;");
   verilog.push ("wire logic [" + to_string(mult_out_size-1) + ":0] m1_1;");
   
   
   
   verilog.push
     (mult_module_name + " m1 (IN1["+to_string (in1_size-1)+":0], " +
      "IN2["+to_string (in2_size-1)+":0], m1_0, m1_1);");
   
   verilog.push("");
   
   
   verilog.push (MAC_Merger_module_name + " merger (m1_0, m1_1, IN3, result);");
   
   verilog.push("outdent");
   verilog.push("");
   verilog.push("endmodule");
   
   verilog.push("");
   
   cout << endl;
   cout << "Multiply-Accumulate Module (" << module_name << ") is created." << endl;
   cout << "   Inputs: IN1[" << in1_size-1 << ":0], IN2[" << in2_size-1 << ":0], IN3[" << in3_size-1 << ":0]" << endl;
   cout << "   Output: result[" << out_size-1 << ":0]" << endl;
   cout << "   Function: result = IN1 * IN2 + IN3 " << (signed_mult?"(signed)":"(unsigned)") << endl;
   
   return 0;
   
}


int create_dot (int  in1_size,
		int  in2_size,
		int  in3_size,
		int  dot_size,
		int  out_size,
		string final_stage_adder,
		string pp_encoding,
		string tree,
		string& module_name,
		queue<string>& verilog,
		int& adder_size){

  bool signed_mult =
    (pp_encoding.compare ("SSP") == 0) ||
    (pp_encoding.compare ("SB4") == 0) ||
    (pp_encoding.compare ("SB2") == 0);



  string mult_module_name;
  int to_be_ignored;

  int mult_out_size = min(out_size, in1_size+in2_size);
  
  int retval = create_mult (in1_size,
			    in2_size,
			    mult_out_size,
			    "",
			    pp_encoding,
			    tree,
			    false,
			    mult_module_name,
			    verilog,
			    to_be_ignored);

  if (retval!=0)
    return retval;
  

  string DOT_Merger_module_name = "DOT_Merger_"
    + std::string(signed_mult ? "Signed_" : "Unsigned_")
    + to_string(dot_size) + "_"
    + to_string(in1_size) + "x"
    + to_string(in2_size)
    + (in3_size>0 ? "_" + to_string(in3_size) : "")
    + (out_size != max(in1_size+in2_size+int((dot_size+1)/2)+(in3_size>0&&(dot_size&1)==0?1:0),in3_size+1)
       ? "_" + to_string(out_size) : "" );

  verilog.push ("module " + DOT_Merger_module_name + "(");
  verilog.push("indent");
  verilog.push("indent");
  for (int i = 0; i < dot_size; i++){
    string m = "m" + to_string(i);
    verilog.push ("input logic [" + to_string(mult_out_size-1) + ":0] "+m+"_0,");
    verilog.push ("input logic [" + to_string(mult_out_size-1) + ":0] "+m+"_1,");
  }
  if (in3_size>0)
    verilog.push ("input logic [" + to_string(in3_size-1) + ":0] IN3,");
  verilog.push ("output logic [" + to_string(out_size - 1) + ":0] result);" );
  verilog.push("outdent");

  string** pp_matrix;
  int pp_dim1 = 0, pp_dim2 = 0;
  

  if (!signed_mult){
    
     pp_dim1 = 1+2*dot_size;
     pp_dim2 = out_size;
     pp_matrix = new string*[pp_dim1];
     for (int i = 0; i < pp_dim1; i++)
       pp_matrix[i] = new string[pp_dim2];
     
     for (int i = 0;  i < pp_dim2; i++){
       if (i < in3_size ){
	 pp_matrix[0][i] = "IN3[" + to_string(i) + "]";
       }
       if (i<mult_out_size){
	 for (int j = 0; j < dot_size; j++){
	   string m = "m" + to_string(j);
	   pp_matrix[j*2+1][i] = m + "_0[" + to_string(i) + "]";
	   pp_matrix[j*2+2][i] = m + "_1[" + to_string(i) + "]";
	 }
       }
     }
  } else {
     verilog.push ( "wire logic const1;");
     verilog.push ( "assign const_1 = 1'b1;");
     
     pp_dim1 = 2+2*dot_size;;
     pp_dim2 = out_size;
     pp_matrix = new string*[pp_dim1];
     for (int i = 0; i < pp_dim1; i++)
       pp_matrix[i] = new string[pp_dim2];

     int extra_one_count = 0;
     for (int i = 0;  i < pp_dim2; i++){
       
       if (i < mult_out_size){
	 for (int j = 0; j < dot_size; j++){
	   string m = "m" + to_string(j);
	   pp_matrix[j*2+2][i] = m + "_0[" + to_string(i) + "]";
	   pp_matrix[j*2+3][i] = m + "_1[" + to_string(i) + "]";
	 }
       } else if (i==mult_out_size)
     	 extra_one_count += dot_size;
       if (i < in3_size-1){
     	 pp_matrix[0][i] = "IN3[" + to_string(i) + "]";
       } else if (i==in3_size-1){
	 pp_matrix[0][i] = "~IN3[" + to_string(i) + "]";
     	 extra_one_count ++;
       }
       
       if ((extra_one_count&1)==1){
     	 pp_matrix[1][i] = "const_1";
     	 extra_one_count++;
       }
       extra_one_count= extra_one_count/2;
     }
     //cout << "extra_one_count" << extra_one_count << endl;

  }


  print_pp(pp_matrix, pp_dim1, pp_dim2, verilog, true);

  if (tree.compare("WT") == 0)
    create_wallacetree (pp_matrix,
			final_stage_adder,
			pp_dim1,
			pp_dim2,
			out_size,
			true,
			signed_mult,
			verilog,
			adder_size);
  else
    create_daddatree (pp_matrix,
		      final_stage_adder,
		      pp_dim1,
		      pp_dim2,
		      out_size,
		      true,
		      signed_mult,
		      verilog,
		      adder_size);
  

   //print_pp(pp_matrix, pp_dim1, pp_dim2);

   verilog.push("outdent");
   verilog.push("");
   verilog.push("endmodule");

   verilog.push("");


   

   module_name = "DOT_Product_" + tree + "_" + pp_encoding + "_"
     + final_stage_adder + "_"
     + to_string(dot_size) + "_"
     + to_string(in1_size) + "x"
     + to_string(in2_size)
     + (in3_size>0 ? "_plus_" + to_string(in3_size) : "")
     + (out_size != max(in1_size+in2_size+int((dot_size+1)/2)+(in3_size>0&&(dot_size&1)==0?1:0),in3_size+1)
	? "_" + to_string(out_size) : "" );
   
   verilog.push ("module " + module_name + "(");
   verilog.push("indent");
   verilog.push("indent");
   for (int i = 0; i < dot_size; i++){
     verilog.push("input logic [" + to_string(in1_size - 1) + ":0] IN1_"+to_string(i)+"," );
     verilog.push("input logic [" + to_string(in2_size - 1) + ":0] IN2_"+to_string(i)+"," );
   }

   if (in3_size>0)
     verilog.push("input logic [" + to_string(in3_size - 1) + ":0] IN3," );
   verilog.push("output logic [" + to_string(out_size - 1) + ":0] result);" );
   verilog.push("outdent");


   verilog.push("");
   
   for (int j = 0; j < dot_size; j++){
     string m = "m" + to_string(j);
     verilog.push ("wire logic [" + to_string(mult_out_size-1) + ":0] "+m+"_0;");
     verilog.push ("wire logic [" + to_string(mult_out_size-1) + ":0] "+m+"_1;");
   }

   verilog.push("");

   for (int j = 0; j < dot_size; j++){
     string m = "m" + to_string(j);
     verilog.push
       (mult_module_name + " "+m+" (IN1_"+to_string(j)+"["+to_string (in1_size-1)+":0], " +
	"IN2_"+to_string(j)+"["+to_string (in2_size-1)+":0], "+m+"_0, "+m+"_1);");
   }
   
   verilog.push("");
   

   string ms = "";
   for (int j = 0; j < dot_size; j++)
     ms +=  "m" + to_string(j) + "_0, " + "m" + to_string(j) + "_1, "; 
   verilog.push (DOT_Merger_module_name + " merger ("+ms+(in3_size>0?" IN3,":"")+" result);");
   
   verilog.push("outdent");
   verilog.push("");
   verilog.push("endmodule");
   
   verilog.push("");
   
   cout << endl;
   cout << "Dot Product Module (" << module_name << ") is created." << endl;
   cout << "   Inputs: IN1_i[" << in1_size-1 << ":0], IN2_i[" << in2_size-1 << ":0] (i from 0 to "+to_string(dot_size-1)+")" <<
     (in3_size>0?+", IN3[" + to_string(in3_size-1) +  ":0]" :"")  << endl;
   cout << "   Output: result[" << out_size-1 << ":0]" << endl;
   cout << "   Function: result = IN1_0 * IN2_0 + ... + IN1_"+to_string(dot_size-1)+" * IN2_"+to_string(dot_size-1)+
     " + IN3 " << (signed_mult?"(signed)":"(unsigned)") << endl;
   
   return 0;
   
}


void write_to_file (string file_name,
		    queue<string>& verilog){

  ofstream myfile;
  
  myfile.open (file_name);
  
  int indent_count = 0;
  
  while(!verilog.empty()) {
    string line = verilog.front();
    if (line.compare("indent") == 0)
      indent_count++;
    else if (line.compare("outdent") == 0)
      indent_count--;
    else {
      for (int i = 0; i<indent_count; i++)
	myfile << "    ";
      myfile << line << endl;
    }
    verilog.pop();
  }
  
  myfile.close();
  
}


int main(int argc, char **argv) {
  

  int  in1_size;
  int  in2_size;
  int  in3_size;
  int  dot_size;
  int  out_size;
  string final_stage_adder;
  string pp_encoding;
  string tree;
  string main_type;
  bool signed_mult;

  interact_with_user(argc, argv, in1_size, in2_size, in3_size, dot_size, out_size, signed_mult, final_stage_adder,
		     pp_encoding, tree, main_type);

 

  queue<string> verilog;

  enter_license(verilog);
  
  int adder_size;
  string module_name;

  int retval0 = 0, retval1 = 0;

  bool create_fin_adder = true;

  
  create_ha_fa (verilog);

  if (main_type.compare("StandAlone") == 0){
    retval0 = create_mult(in1_size, in2_size,
			  out_size,
			  final_stage_adder,
			  pp_encoding,
			  tree,
			  create_fin_adder,
			  module_name,
			  verilog,
			  adder_size);
  } else if (main_type.compare("FourMult") == 0) {
    create_four_mult (in1_size, in3_size,
		      out_size,
		      final_stage_adder,
		      pp_encoding,
		      tree,
		      module_name,
		      verilog,
		      adder_size);

  } else if (main_type.compare("MAC") == 0){
    create_fma (in1_size,
		in2_size,
		in3_size,
		out_size,
		final_stage_adder,
		pp_encoding,
		tree,
		module_name,
		verilog,
		adder_size);
  } else if (main_type.compare("DOT") == 0){
    create_dot (in1_size,
		in2_size,
		in3_size,
		dot_size,
		out_size,
		final_stage_adder,
		pp_encoding,
		tree,
		module_name,
		verilog,
		adder_size);
  } else if (main_type.compare("Adder") == 0) {
    create_fin_adder = true;
    module_name = final_stage_adder + "_" + to_string (in1_size);
    adder_size = in1_size;
  } else
    cout << "not implemented yet!" << endl;
  

  if (create_fin_adder){
    retval1 = create_adder(final_stage_adder, adder_size, verilog);
  } 
  

  if (retval0 == 1 || retval0 == 1)
    return 1;

  string file_name = module_name + "_multgen.sv";  
  write_to_file(file_name, verilog);
 

  cout << endl;
  cout << "Written to " << file_name << endl;
  cout << endl;
 
  return 0;
}
