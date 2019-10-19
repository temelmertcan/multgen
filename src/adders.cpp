

#include <iostream>
#include <queue>          // std::queue
#include <fstream>

using namespace std;


#include "adders.h"


void create_ha_fa (std::queue<string>& verilog){

  verilog.push ("module ha (");
  verilog.push ("input logic a,");
  verilog.push ("input logic b,");
  verilog.push ("output logic s,");
  verilog.push ("output logic c);");
  verilog.push ("assign s = a ^ b;");
  verilog.push ("assign c = a & b;");
  verilog.push ("endmodule");

  verilog.push ("\n");


  verilog.push ("module fa (");
  verilog.push ("input logic x,");
  verilog.push ("input logic y,");
  verilog.push ("input logic z,");

  verilog.push ("output logic s,");
  verilog.push ("output logic c);");
  verilog.push ("assign {c,s} = x + y + z;");
  verilog.push ("endmodule");

  verilog.push ("");

}

void create_rp_adder (int size, std::queue<string>& verilog){

  verilog.push ("module RP_" + to_string(size) + " ( ");

  verilog.push ("input logic [" + to_string(size-1) + ":0] IN1,");
  verilog.push ("input logic [" + to_string(size-1) + ":0] IN2,");
  verilog.push ("output logic [" + to_string(size) + ":0] OUT);");

  string carry_decs = "logic ";
  for (int i=0; i < size+1; i++){
    carry_decs+= "C" + to_string(i);
    if (i == size)
      carry_decs += ";";
    else
      carry_decs += ", ";
  }

  verilog.push (carry_decs);

  for (int i=0; i < size; i++){
    if (i == 0)
      verilog.push ("ha m0 (IN1[0], IN2[0], OUT[0], C0);");
    else
      verilog.push ("fa m" + to_string(i) + " (IN1[" +  to_string(i) + "], IN2["
		    + to_string(i) + "], C" + to_string(i-1) + ", OUT[" + to_string(i) + "], C"
		    + to_string(i) + ");");
  }

  verilog.push ("assign OUT[" + to_string(size) + "] = C" + to_string(size-1) + ";");

  verilog.push("endmodule");

  verilog.push("");

}

