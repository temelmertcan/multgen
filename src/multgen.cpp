

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

int main() {
  string** pp_matrix;

  int  in1_size = 64;
  int  in2_size = 64;
  string final_stage_adder = "RP";
  string pp_encoding = "UB4";
  string tree = "DT";

  int pp_dim1, pp_dim2;

  queue<string> verilog;
  int adder_size;

  create_ha_fa (verilog);

  string module_name = tree + "_" + pp_encoding + "_"
		+ final_stage_adder + "_"
		+ to_string(in1_size) + "_"
		+ to_string(in2_size);

  verilog.push ("module " + module_name + "(");
  verilog.push ("input logic [" + to_string(in1_size - 1) + ":0] IN1," );
  verilog.push ("input logic [" + to_string(in2_size - 1) + ":0] IN2," );
  verilog.push ("output logic [" + to_string(in2_size + in1_size - 1) + ":0] result);" );

  verilog.push("\n// Creating Partial Products \n");
  
  if (pp_encoding.compare ("USP") == 0) {
    create_unsignedpp (in1_size, in2_size, pp_matrix,
		      pp_dim1, pp_dim2, verilog);
  } else if (pp_encoding.compare ("SB4") == 0) {
    create_signedbr4pp (in1_size, in2_size, pp_matrix,
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

  create_rp_adder (adder_size, verilog);

  ofstream myfile;
  string file_name = module_name + "_multgen.sv";

  myfile.open (file_name);

  while(!verilog.empty()) {
    myfile << verilog.front() << endl;
    verilog.pop();
  }

  myfile.close();

  cout << "Written to " << file_name << " with adder size = " << adder_size << "\n";

  return 0;
}
