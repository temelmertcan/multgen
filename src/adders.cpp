

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

void ppx_preprocess (int size, int& index, string addertype,
		     string*& g, string*& p, std::queue<string>& verilog){

  verilog.push ("module " + addertype + "_" + to_string(size) + " ( ");
  verilog.push("indent");
  verilog.push("indent");
  verilog.push ("input logic [" + to_string(size-1) + ":0] IN1,");
  verilog.push ("input logic [" + to_string(size-1) + ":0] IN2,");
  verilog.push ("output logic [" + to_string(size) + ":0] OUT);");
  verilog.push("outdent");
  verilog.push("");
  
  verilog.push ( "wire logic ["+ to_string(size-1) + ":0] p_0;");
  verilog.push ( "wire logic ["+ to_string(size-1) + ":0] g_0;");

  verilog.push ( "assign g_0 = IN1 & IN2;");
  verilog.push ( "assign p_0 = IN1 ^ IN2;");
  
  for (int i = 0; i<size; i++){
    g[i] = "g_0[" + to_string(i) + "]";
    p[i] = "p_0[" + to_string(i) + "]";
  }

}

void ppx_postprocess (int size, int& index,
		     string*& g, string*& p, std::queue<string>& verilog){


  verilog.push ("assign OUT[0] = p_0[0];");

  for (int i = 1; i<size; i+=1){

    verilog.push ("assign OUT[" + to_string(i) + "] = p_0[" + to_string(i) + "] ^ " + g[i-1] + ";");
  }

  verilog.push ("assign OUT[" + to_string(size) + "] = " + g[size-1] + ";");

  verilog.push("outdent");
  verilog.push("endmodule");

  verilog.push("");

}

// void available_pg (string* s, int size){
//   string res = "";

//   for (int i = 0; i<size; i++){
//     res += 
//   }

void pgtmp_to_pg (int size, string*& from, string*& to){
  for (int i = 0; i<size; i++)
    to[i] = from[i];
}

void create_hc_adder (int size, std::queue<string>& verilog){
 
  int index = 0;
  
  string* g = new string[size];
  string* p = new string[size];

  
  string* gtmp = new string[size];
  string* ptmp = new string[size];

  ppx_preprocess(size, index, "HC", gtmp, ptmp, verilog);
  pgtmp_to_pg(size, gtmp, g);
  pgtmp_to_pg(size, ptmp, p);
  
  // hc step1 :
  verilog.push("");
  verilog.push("// HC stage 1");
  
  for (int i = 1; i<size; i+=2){

    string new_p = "p_1_"+ to_string(i);
    string new_g = "g_1_"+ to_string(i);
    
    verilog.push ("wire logic " + new_p + ";");
    verilog.push ("wire logic "+ new_g + ";");

    verilog.push ("assign "+ new_p + " = " + p[i] + " & " + p[i-1] + ";");
    verilog.push ("assign "+ new_g + " = (" + p[i] + " & " + g[i-1] + ") | " + g[i] + ";");
    
    gtmp[i] =  new_g;
    ptmp[i] =  new_p;
    //index++;
  }

  pgtmp_to_pg(size, gtmp, g);
  pgtmp_to_pg(size, ptmp, p);

  int stage=1;
  
  // hc midsteps
  for (int diff = 2; diff < size; diff *= 2){
    verilog.push("");
    verilog.push("// HC stage " + to_string(++stage));
    for (int i=1+diff; i < size; i+=2){

      string new_p = "p_" + to_string(stage)+ "_" + to_string(i);
      string new_g = "g_" + to_string(stage)+ "_" + to_string(i);
      
      verilog.push ("wire logic "+ new_p + ";");
      verilog.push ("wire logic "+ new_g + ";");
      
      verilog.push ("assign "+ new_p + " = " + p[i] + " & " + p[i-diff] + ";");
      verilog.push ("assign "+ new_g + " = (" + p[i] + " & " + g[i-diff] + ") | " + g[i] + ";");
      
      gtmp[i] =  new_g;
      ptmp[i] =  new_p;
      index++;
    }
    pgtmp_to_pg(size, gtmp, g);
    pgtmp_to_pg(size, ptmp, p);
  }

  //hc final step
  verilog.push("");
  verilog.push("// HC stage " + to_string(++stage));
  for (int i = 2; i<size; i+=2){

    string new_p = "p_" + to_string(stage)+ "_" + to_string(i);
    string new_g = "g_" + to_string(stage)+ "_" + to_string(i);
    
    verilog.push ("wire logic "+ new_p + ";");
    verilog.push ("wire logic "+ new_g + ";");

    verilog.push ("assign "+ new_p + " = " + p[i] + " & " + p[i-1] + ";");
    verilog.push ("assign "+ new_g + " = (" + p[i] + " & " + g[i-1] + ") | " + g[i] + ";");
    
    gtmp[i] =  new_g;
    ptmp[i] =  new_p;
    index++;
  }

  pgtmp_to_pg(size, gtmp, g);
  pgtmp_to_pg(size, ptmp, p);

  verilog.push("");
  verilog.push("// HC postprocess ");

  ppx_postprocess (size, index, g, p, verilog);

  delete[] p;
  delete[] g;

  delete[] ptmp;
  delete[] gtmp;
  
}



void create_lf_adder (int size, std::queue<string>& verilog){
 
  int index = 0;
  
  string* g = new string[size];
  string* p = new string[size];

  
  string* gtmp = new string[size];
  string* ptmp = new string[size];

  ppx_preprocess(size, index, "LF", gtmp, ptmp, verilog);
  pgtmp_to_pg(size, gtmp, g);
  pgtmp_to_pg(size, ptmp, p);
  
  int stage=0;


  // lf midsteps
  for (int diff = 1; diff < size; diff *= 2){
    verilog.push("");
    verilog.push("// LF stage " + to_string(++stage));
    
    
    for (int i=diff; i < size; i+=diff*2){

      for (int j = 0; j<diff && (i+j)<size; j++){
	string new_p = "p_" + to_string(stage)+ "_" + to_string(i+j);
	string new_g = "g_" + to_string(stage)+ "_" + to_string(i+j);
	
	verilog.push ("wire logic "+ new_p + ";");
	verilog.push ("wire logic "+ new_g + ";");
	
	verilog.push ("assign "+ new_p + " = " + p[i+j] + " & " + p[i-1] + ";");
	verilog.push ("assign "+ new_g + " = (" + p[i+j] + " & " + g[i-1] + ") | " + g[i+j] + ";");
	
	gtmp[i+j] =  new_g;
	ptmp[i+j] =  new_p;
      }
    }
    pgtmp_to_pg(size, gtmp, g);
    pgtmp_to_pg(size, ptmp, p);
  }
  
  verilog.push("");
  verilog.push("// LF postprocess ");

  ppx_postprocess (size, index, g, p, verilog);

  delete[] p;
  delete[] g;

  delete[] ptmp;
  delete[] gtmp;
  
}
