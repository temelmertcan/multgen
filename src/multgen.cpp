

#include <iostream>
#include <queue>          // std::queue
#include <fstream>

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

void interact_with_user (int argc, char **argv,
			 int& in1_size,
			 int& in2_size,
			 string& final_stage_adder,
			 string& pp_encoding,
			 string& tree){

  
 
  
  if (argc == 2 && strcmp(argv[1], "-def") == 0) {
     in1_size = 8;
     in2_size = 8;
     final_stage_adder = "KS";
     pp_encoding = "SSP";
     tree = "WT";
  } else if (argc == 6){
    sscanf(argv[1], "%d", &in1_size);
    sscanf(argv[2], "%d", &in2_size);
    pp_encoding = argv[3];
    tree = argv[4];
    final_stage_adder = argv[5];
  } else if (argc <= 1) {

    string s = "";
    
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
      
      cout << "1. Simple Unsigned " << endl;
      cout << "2. Simple Signed (Baugh-Wooley)" << endl;
      cout << "3. Booth radix-4 Unsigned  " << endl;
      cout << "4. Booth radix-4 Signed " << endl;
      cout << "5. Booth radix-2 Signed " << endl;

      cout << "Select Partial Product Generation Algorithm: ";   
      cin >> s;
      if (s.compare ("1") == 0) {
	pp_encoding = "USP";
	break;
      }else  if (s.compare ("2") == 0) {
	pp_encoding = "SSP";
	break;
      }else if (s.compare ("3") == 0)  {
	pp_encoding = "UB4";
	break;
      } else if (s.compare ("4") == 0) {
	pp_encoding = "SB4";
	break;
      } else if (s.compare ("5") == 0) {
	pp_encoding = "SB2";
	break;
      }
      else
	cout << "Invalid Selection!" << endl;

    }

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

    
    cout << "Enter IN1 size: ";
    cin >> in1_size;

    
    cout << "Enter IN2 size: ";
    cin >> in2_size;
    
  } else {
    for (int i = 0; i< argc; i++){
      cout << "Arg " << i << ": " << argv[i] << endl;
    }
    cout << "bad arguments!" << endl;
  }
   
  
}


int main(int argc, char **argv) {
  string** pp_matrix;

  int  in1_size;
  int  in2_size;
  string final_stage_adder;
  string pp_encoding;
  string tree;

  interact_with_user(argc, argv, in1_size, in2_size, final_stage_adder,
		     pp_encoding, tree);
  
  
  int pp_dim1, pp_dim2;

  queue<string> verilog;
  int adder_size;

  create_ha_fa (verilog);


  


  
  string module_name = tree + "_" + pp_encoding + "_"
		+ final_stage_adder + "_"
		+ to_string(in1_size) + "_"
		+ to_string(in2_size);

  verilog.push ("module " + module_name + "(");
  verilog.push("indent");
  verilog.push("indent");
  verilog.push ("input logic [" + to_string(in1_size - 1) + ":0] IN1," );
  verilog.push ("input logic [" + to_string(in2_size - 1) + ":0] IN2," );
  verilog.push ("output logic [" + to_string(in2_size + in1_size - 1) + ":0] result);" );
  verilog.push("outdent");
  verilog.push("");
  
  verilog.push("\n// Creating Partial Products \n");
  
  if (pp_encoding.compare ("USP") == 0) {
    create_unsignedpp (in1_size, in2_size, pp_matrix,
		      pp_dim1, pp_dim2, verilog);
  } else if (pp_encoding.compare ("SSP") == 0) {
    create_signedpp (in1_size, in2_size, pp_matrix,
			pp_dim1, pp_dim2, verilog);
  } else if (pp_encoding.compare ("SB4") == 0) {
    create_signedbr4pp (in1_size, in2_size, pp_matrix,
		      pp_dim1, pp_dim2, verilog);
  } else if (pp_encoding.compare ("SB2") == 0) {
    create_signedbr2pp (in1_size, in2_size, pp_matrix,
		      pp_dim1, pp_dim2, verilog);
  } else if (pp_encoding.compare ("UB4") == 0) {
    create_unsignedbr4pp (in1_size, in2_size, pp_matrix,
			  pp_dim1, pp_dim2, verilog);
  } else {
    cout << "Bad PP Selection!" << endl;
    return 1;
  }
    

  
  
  verilog.push("\n// Creating Summation Tree \n");

  if (tree.compare ("WT") == 0) {
  
    create_wallacetree (pp_matrix, final_stage_adder, pp_dim1, pp_dim2,
			in1_size + in2_size,
			verilog,
			adder_size);
    
  } else if (tree.compare ("DT") == 0){
     create_daddatree (pp_matrix, final_stage_adder, pp_dim1, pp_dim2,
			in1_size + in2_size,
			verilog,
			adder_size);
  } else {
    cout << "Bad Summation tree Selection!" << endl;
    return 1;
  }
  
  verilog.push ("endmodule\n\n");

  verilog.push("outdent");
  verilog.push("");

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
  
  ofstream myfile;
  string file_name = module_name + "_multgen.sv";

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

  cout << "Written to " << file_name << " with adder (" + final_stage_adder + ") size = " << adder_size << " \n";

  return 0;
}
