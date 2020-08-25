

#include <iostream>
#include <queue>          // std::queue
#include <fstream>

#include <string>
#include <sstream>

using namespace std;

#include "adders.h"

#include "pp_gen.h"

#include "wallace.h"

#include "dadda.h"

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

      cout << "1. Stand-alone Multiplier " << endl;
      cout << "2. Merged Four Multipliers " << endl;
      cout << "3. Fused Multiply-add " << endl;
      //cout << "2.  " << endl;

      cout << "Select Multiplier Type: ";
      cin >> s;
      if (s.compare ("1") == 0) {
	main_type = "StandAlone";
	break;
      }else if (s.compare ("2") == 0)  {
	main_type = "FourMult";
	break;
      }else if (s.compare ("3") == 0)  {
	main_type = "FMA";
	break;
      } else
	cout << "Invalid Selection!" << endl;

    }

    while(1) {

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

    while(1) {

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

    
    while(1) {

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

    if (main_type.compare("FMA") == 0){
      cout << "Enter IN1 (Multiplier) size: ";
      cin >> in1_size;

      cout << "Enter IN2 (Multiplicand) size: ";
      cin >> in2_size;

      cout << "Enter IN3 size: ";
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
	cout << "Enter input vector (multiplier and multiplicand) size: ";
	cin >> in1_size;
	in2_size = in1_size;
	out_size = in1_size + in2_size;
	if ((in1_size & 1) == 0)
	  break;
	cout << "Input size must be divisible by 2. ";
      }
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


int create_four_mult ( int  in_size,
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

  int out_size = 2*in_size;
  int in1_size = in_size;
  int in2_size = in_size;
  
  int one_mult_in_size = in_size/2+(signed_mult?1:0);
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
    + (out_size != (in2_size+in1_size) ? "_" + to_string(out_size) : "" );

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
  verilog.push ("output logic [" + to_string(out_size - 1) + ":0] result);" );
  verilog.push("outdent");

  string** pp_matrix;
  int pp_dim1 = 0, pp_dim2 = 0;
  

  if (!signed_mult){
    
     pp_dim1 = 8;
     pp_dim2 = out_size;
     pp_matrix = new string*[pp_dim1];
     for (int i = 0; i < pp_dim1; i++)
       pp_matrix[i] = new string[pp_dim2];
     
     for (int i = 0;  i < pp_dim2; i++){
       if (i<one_mult_out_size){
	 pp_matrix[0][i] = "m1_0[" + to_string(i) + "]";
	 pp_matrix[1][i] = "m1_1[" + to_string(i) + "]";
       }
       if (i>=one_mult_in_size && i<(one_mult_out_size+one_mult_in_size)){
	 pp_matrix[2][i] = "m2_0[" + to_string(i-one_mult_in_size) + "]";
	 pp_matrix[3][i] = "m2_1[" + to_string(i-one_mult_in_size) + "]";
	 pp_matrix[4][i] = "m3_0[" + to_string(i-one_mult_in_size) + "]";
	 pp_matrix[5][i] = "m3_1[" + to_string(i-one_mult_in_size) + "]";
       }
       if (i>=one_mult_out_size ){
	 pp_matrix[6][i] = "m4_0[" + to_string(i-one_mult_out_size) + "]";
	 pp_matrix[7][i] = "m4_1[" + to_string(i-one_mult_out_size) + "]";
       }
     }

    
    
  } else {
   
     
     verilog.push ( "wire logic const1;");
     verilog.push ( "assign const_1 = 1'b1;");
     
     pp_dim1 = 9;
     pp_dim2 = out_size;
     pp_matrix = new string*[pp_dim1];
     for (int i = 0; i < pp_dim1; i++)
       pp_matrix[i] = new string[pp_dim2];

     int extra_one_count = 0;
     for (int i = 0;  i < pp_dim2; i++){
       bool addone = false;
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
       
       if ((extra_one_count&1)==1){
	 pp_matrix[8][i] = "const_1";
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
     + (out_size != (in2_size+in1_size) ? "_" + to_string(out_size) : "" );

  verilog.push ("module " + module_name + "(");
  verilog.push("indent");
  verilog.push("indent");
  verilog.push ("input logic [" + to_string(in1_size - 1) + ":0] IN1," );
  verilog.push ("input logic [" + to_string(in2_size - 1) + ":0] IN2," );
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
      (one_mult_module_name + " m1 (IN1["+to_string (one_mult_in_size-1)+":0], " +
       "IN2["+to_string (one_mult_in_size-1)+":0], m1_0, m1_1);");
    
    verilog.push
      (one_mult_module_name + " m2 (IN1["+to_string (one_mult_in_size-1)+":0], " +
       "IN2["+to_string (one_mult_in_size+one_mult_in_size-1)+":"+
       to_string(one_mult_in_size)+"], m2_0, m2_1);");

     verilog.push
      (one_mult_module_name + " m3 (IN1["+to_string (one_mult_in_size+one_mult_in_size-1)+":"+
       to_string(one_mult_in_size)+"], " +
       "IN2["+to_string (one_mult_in_size-1)+":0], m3_0, m3_1);");

     verilog.push
      (one_mult_module_name + " m4 (IN1["+to_string (one_mult_in_size+one_mult_in_size-1)+":"+
       to_string(one_mult_in_size)+"], " +
       "IN2["+to_string (one_mult_in_size+one_mult_in_size-1)+":"+
       to_string(one_mult_in_size)+"], m4_0, m4_1);");
     verilog.push("");

  } else {
     verilog.push("");
    verilog.push
      (one_mult_module_name + " m1 ({1'b0, IN1["+to_string (one_mult_in_size-2)+":0]}, " +
       "{1'b0, IN2["+to_string (one_mult_in_size-2)+":0]}, m1_0, m1_1);");
    
    verilog.push
      (one_mult_module_name + " m2 ({1'b0, IN1["+to_string (one_mult_in_size-2)+":0]}, " +
       "{IN2["+to_string (one_mult_in_size+one_mult_in_size-3)+"], "+
       "IN2["+to_string (one_mult_in_size+one_mult_in_size-3)+":"+
       to_string(one_mult_in_size-1)+"]}, m2_0, m2_1);");

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
      (FourMultMerger_module_name + " merger (m1_0, m1_1, m2_0, m2_1, m3_0, m3_1, m4_0, m4_1, result);");
   
  verilog.push("outdent");
  verilog.push("");
  verilog.push("endmodule");
   
  verilog.push("");

  cout << endl;
  cout << "Merged Multiplier Module (" << module_name << ") is created." << endl;
  cout << "   Inputs: IN1[" << in1_size-1 << ":0], IN2[" << in2_size-1 << ":0]" << endl;
  cout << "   Output: result[" << out_size-1 << ":0]" << endl;
  cout << "   Function: result = IN1 * IN2 " << (signed_mult?"(signed)":"(unsigned)") << endl;

  

  
  return 0;
  
}




int create_fma ( int  in1_size,
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
  

  string FMA_Merger_module_name = "FMA_Merger_"
    + std::string(signed_mult ? "Signed_" : "Unsigned_")
    + to_string(in1_size) + "x"
    + to_string(in2_size)
    + (out_size != (in2_size+in1_size) ? "_" + to_string(out_size) : "" );

  verilog.push ("module " + FMA_Merger_module_name + "(");
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


   

   module_name = "FMA_" + tree + "_" + pp_encoding + "_"
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
   
   
   verilog.push (FMA_Merger_module_name + " merger (m1_0, m1_1, IN3, result);");
   
   verilog.push("outdent");
   verilog.push("");
   verilog.push("endmodule");
   
   verilog.push("");
   
   cout << endl;
   cout << "Fused Multiply-add Module (" << module_name << ") is created." << endl;
   cout << "   Inputs: IN1[" << in1_size-1 << ":0], IN2[" << in2_size-1 << ":0], IN3[" << in3_size-1 << ":0]" << endl;
   cout << "   Output: result[" << out_size-1 << ":0]" << endl;
   cout << "   Function: result = IN1 * IN2 + IN3 " << (signed_mult?"(signed)":"(unsigned)") << endl;
   
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
  int  out_size;
  string final_stage_adder;
  string pp_encoding;
  string tree;
  string main_type;
  bool signed_mult;

  interact_with_user(argc, argv, in1_size, in2_size, in3_size, out_size, signed_mult, final_stage_adder,
		     pp_encoding, tree, main_type);

 

  queue<string> verilog;
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
    create_four_mult (in1_size,
		      final_stage_adder,
		      pp_encoding,
		      tree,
		      module_name,
		      verilog,
		      adder_size);

  } else if (main_type.compare("FMA") == 0){
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
