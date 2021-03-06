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


void create_ha_fa (std::queue<string>& verilog){

  verilog.push ("module ha (");
  verilog.push("indent");
  verilog.push("indent");
  verilog.push ("input logic a,");
  verilog.push ("input logic b,");
  verilog.push ("output logic s,");
  verilog.push ("output logic c);");
  verilog.push("outdent");
  verilog.push("");
  verilog.push ("assign s = a ^ b;");
  verilog.push ("assign c = a & b;");
  verilog.push ("endmodule");
  verilog.push("outdent");
  verilog.push("");

  verilog.push ("\n");


  verilog.push ("module fa (");
  verilog.push("indent");
  verilog.push("indent");
  verilog.push ("input logic x,");
  verilog.push ("input logic y,");
  verilog.push ("input logic z,");
  verilog.push ("output logic s,");
  verilog.push ("output logic c);");
  verilog.push("outdent");
  verilog.push("");
  verilog.push ("assign s = x ^ y ^ z;");
  verilog.push ("assign c = (x & y) | (x & z) | (y & z);");
  verilog.push ("endmodule");
  verilog.push("outdent");
  verilog.push("");

  verilog.push ("");

}

void create_rp_adder (int size, std::queue<string>& verilog){

  verilog.push ("module RP_" + to_string(size) + " ( ");
  verilog.push("indent");
  verilog.push("indent");
  verilog.push ("input logic [" + to_string(size-1) + ":0] IN1,");
  verilog.push ("input logic [" + to_string(size-1) + ":0] IN2,");
  verilog.push ("output logic [" + to_string(size) + ":0] OUT);");
  verilog.push("outdent");
   verilog.push("");
 
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


  verilog.push("outdent");
  verilog.push("");
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



void create_ks_adder (int size, std::queue<string>& verilog){
 
  int index = 0;
  
  string* g = new string[size];
  string* p = new string[size];

  
  string* gtmp = new string[size];
  string* ptmp = new string[size];

  ppx_preprocess(size, index, "KS", gtmp, ptmp, verilog);
  pgtmp_to_pg(size, gtmp, g);
  pgtmp_to_pg(size, ptmp, p);
  
  int stage=0;


  // lf midsteps
  for (int diff = 1; diff < size; diff *= 2){
    verilog.push("");
    verilog.push("// KS stage " + to_string(++stage));
    
    
    for (int i=diff; i < size; i++){

      string new_p = "p_" + to_string(stage)+ "_" + to_string(i);
      string new_g = "g_" + to_string(stage)+ "_" + to_string(i);
      
      verilog.push ("wire logic "+ new_p + ";");
      verilog.push ("wire logic "+ new_g + ";");
      
      verilog.push ("assign "+ new_p + " = " + p[i] + " & " + p[i-diff] + ";");
      verilog.push ("assign "+ new_g + " = (" + p[i] + " & " + g[i-diff] + ") | " + g[i] + ";");
      
      gtmp[i] =  new_g;
      ptmp[i] =  new_p;
    }
    
    pgtmp_to_pg(size, gtmp, g);
    pgtmp_to_pg(size, ptmp, p);
  }
  
  verilog.push("");
  verilog.push("// KS postprocess ");

  ppx_postprocess (size, index, g, p, verilog);

  delete[] p;
  delete[] g;

  delete[] ptmp;
  delete[] gtmp;
  
}
