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
#include <algorithm>

using namespace std;

#include "pp_gen.h"


// Should merge the two extra indices list by carrying over the ones.
std::list<int> merge_extra_ones_indices (const std::list<int>& extra_ones_indices1,
                                         const std::list<int>& extra_ones_indices2,
                                         const int& offset2){

  int carry = -1;

  std::list<int> result;

  for (auto it1 = extra_ones_indices1.begin(), it2 = extra_ones_indices2.begin() ;
       it1 != extra_ones_indices1.end() || it2 != extra_ones_indices2.end();){

    bool end1 = (it1 == extra_ones_indices1.end());
    bool end2 = (it2 == extra_ones_indices2.end());

    int val1 = *it1;
    int val2 = *it2 + offset2;

    //cout << "lst1: " << val1 << ", lst2:" << val2 << ", carry: " << carry << "." << endl;

    if ((end1 || carry < val1) && (end2 || carry < val2) && carry != -1){
      result.push_back(carry);
      carry = -1;
    } else if (!end1 && carry == val1 && (end2 || carry <= val2)) {
      carry = val1 + 1;
      it1++;
    } else if (!end2 && carry == val2 && (end1 || carry <= val1)) {
      carry = val2 + 1;
      it2++;
    } else if (!end1 && (val1 < val2 || end2)) {
      result.push_back(val1);
      it1++;
    } else if (!end2 && (val2 < val1 || end1)) {
      result.push_back(val2);
      it2++;
    } else {
      //cout << "here" << endl;
      carry = val1 + 1;
      ++it1;
      ++it2;
    }
  }

  if (carry != -1)
    result.push_back(carry);

  return result;

}

// calcualtes necessary extra ones to be added so that extra ones would be carried over from the result.
void add_extra_ones_to_pp_matrix (string *& last_row, int col_size, const std::list<int>& extra_ones_indices){

  last_row  = new string[col_size];

  // make sure it is empty..
  for (int i = 0; i<col_size; i++)
    last_row[i] = "";

  for (auto e : extra_ones_indices)
    if (e<col_size)
      last_row[e] = "1'b1";

  // for (int i = 0; i<col_size; i++)
  //   cout << i << " " << last_row[i] << ". ";
  // cout << endl;

  bool carry = false;
  for (int i = 0; i < col_size; i++){

    if (!carry && last_row[i] != ""){
      last_row[i] = "1'b1";
      carry = true;
    } else if (carry && last_row[i] == "") {
      last_row[i] = "1'b1";
    } else if (carry) {
      last_row[i] = "";
    }
  }

  // for (int i = 0; i<col_size; i++)
  //   cout << i << " " << last_row[i] << ". ";
  // cout << endl;
}

void print_pp (string** pp, int m, int n, std::queue<string>& verilog, bool to_file){

  if (n > 64)
    return;

  if (to_file){
    verilog.push("");
    verilog.push("// The values to be summed in the summation tree, from LSB (left) to MSB:");

  }

  unsigned long len = 4;
  for (int i=0; i<m; i++){
    for (int j=0; j<n; j++){
      len = max (pp[i][j].size(), len);
    }
  }
  len++;

  for (int i=0; i<m; i++){
    string line = (to_file ? " // " : "");
    for (int j=0; j<n; j++){
      if (pp[i][j] != ""){
        line += pp[i][j];

        for (int k = pp[i][j].size(); k<len; k++)
          line += " ";
      }
      else{
        line += "  --";
        for (int k = 4; k<len; k++)
          line+= " ";
      }
    }
    if (to_file) verilog.push(line);
    else cout << line << endl;
  }

  // cout << "here" << endl;

}

void create_unsignedpp (int m, int n,
                        string**& pp_matrix,
                        int& pp_dim1,
                        int& pp_dim2,
                        std::queue<string>& verilog){

  pp_dim1 = m;
  pp_dim2 = n+m-1;

  pp_matrix = new string*[pp_dim1];

  for (int i = 0; i<m; i++)
    verilog.push ( "wire logic [" + to_string(n-1) + ":0] pp" + to_string(i) + ";");

  for (int i = 0; i < m; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < i; j++){
      pp_matrix[i][j] = "";
    }
    for (int j = 0; j < n; j++){
      string cur;
      cur = "pp" + to_string(i) + "[" + to_string(j) + "]";
      pp_matrix[i][j+i] = cur;
    }
    for (int j = n+i; j < n+m-1; j++){
      pp_matrix[i][j] = "";
    }

    verilog.push (
                  "assign pp" + to_string(i)  + " = {"
                  + to_string(n)
                  + "{IN1["
                  + to_string(i)
                  + "]}} & IN2;");
  }

  //print_pp(pp_matrix, pp_dim1, pp_dim2, verilog, true);

}

void create_signedpp (int m, int n,
                      string**& pp_matrix,
                      int& pp_dim1,
                      int& pp_dim2,
		      std::list<int>& extra_ones_indices,
		      std::queue<string>& verilog){

  pp_dim1 = m+1;
  pp_dim2 = n+m;

   pp_matrix = new string*[pp_dim1];
  for (int i = 0; i < pp_dim1; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < pp_dim2; j++)
      pp_matrix[i][j] = "";  			 
  }

  // if (m != n) {
  //   cout << "Simple signed partial products are only supported when IN1 and IN2 has the same dimensions." << endl;
  //   return;
  // }


 
 
  for (int i = 0; i<m; i++)
    verilog.push ( "wire logic [" + to_string(n-1) + ":0] pp" + to_string(i) + ";");

  for (int i = 0; i < m; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < i; j++){
      pp_matrix[i][j] = "";
    }
    for (int j = 0; j < n; j++){
      string cur;
      cur = "pp" + to_string(i) + "[" + to_string(j) + "]";
      pp_matrix[i][j+i] = cur;
    }

    for (int j = n+i; j < pp_dim2; j++){
      pp_matrix[i][j] = "";
    }

    if (i==m-1)
      verilog.push ("assign pp" + to_string(i)  + " = ~({"
                    + to_string(n)
                    + "{IN1["
                    + to_string(i)
                    + "]}} & IN2) ^ ((1'b1)<<" + to_string(n-1) + ");");
    else
      verilog.push ("assign pp" + to_string(i)  + " = ({"
                    + to_string(n)
                    + "{IN1["
                    + to_string(i)
                    + "]}} & IN2) ^ ((1'b1)<<" + to_string(n-1) + ");");
  }


  std::list<int> extra_ones_indices1;
  for (int i = n-1; i<m-1+n-1; i++)
    extra_ones_indices1.push_back (i);
  std::list<int> extra_ones_indices2;
  for (int i = m-1; i<m-1+n-1; i++)
    extra_ones_indices2.push_back (i);

  extra_ones_indices = merge_extra_ones_indices(extra_ones_indices1,extra_ones_indices2,0);

  
  // extra_ones_indices.push_back(n);
  // extra_ones_indices.push_back(m+n-1);
  // pp_matrix[m][n] = "1'b1";
  // pp_matrix[m][m+n-1] = "1'b1";

  // print_pp(pp_matrix, pp_dim1, pp_dim2, verilog, true);

  //print_pp(pp_matrix, pp_dim1, pp_dim2);

}

void create_unsignedbr2pp (int m, int n,
                           string**& pp_matrix,
                           int& pp_dim1,
                           int& pp_dim2,
                           std::queue<string>& verilog){

  verilog.push ( "wire logic const1;");
  verilog.push ( "assign const1 = 1'b1;");

  pp_dim1 = m+3; // number of rows
  pp_dim2 = (n+m+1); // number of columns

  pp_matrix = new string*[pp_dim1];

  /////
  verilog.push ("wire [" + to_string(n) + ":0] IN2_1x;");
  verilog.push ("assign IN2_1x = { 1'b0,  IN2};");
  /////

  for (int i = 0; i < m+1; i++){
    verilog.push ("");
    verilog.push ("// Signed Booth Radix-2 Partial Products Row " + to_string(i+1));
    string m0 = (i == 0 ? "1'b0" : "IN1[" + to_string(i - 1) + "]");
    string m1 = (i==m?"1'b0":"IN1[" + to_string(i) + "]");

    string select_e = "select_e_" + to_string(i);
    string select_ne = "select_ne_" + to_string(i);

    string cmp = "t_comp" + to_string(i);

    verilog.push ( "wire logic " + select_e
                   + ", " + select_ne
                   + ", " + cmp
                   + ";");

    verilog.push ("assign " + select_e +
                  " = (~ " + m1 + ") & (" + m0 + ");");

    verilog.push ("assign " + select_ne +
                  " = ( " + m1 + ") & (~ " + m0 + ");");

    string cur_pp = "pp_" + to_string(i);
    verilog.push ("wire ["+ to_string(n+1) +":0] " + cur_pp + ";");
    verilog.push ("assign " + cur_pp + " = (1<<" +
                  to_string(n) +
                  ") ^ (" +
                  "("+ select_e  + " ? IN2_1x : 0) | " +
                  "(" +  select_ne + " ? (~ IN2_1x) : 0)" +
                  ");");
    verilog.push ("assign " + cmp + " = " + select_ne  + ";");

  }

  for (int i = 0; i < pp_dim1-2; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < i; j++){
      pp_matrix[i][j] = "";
    }
    for (int j = 0; j < n+1 && j+i < pp_dim2 ; j++){
      string cur;
      cur = "pp_" + to_string(i) + "[" + to_string(j) + "]";
      pp_matrix[i][j+i] = cur;
    }
  }

  // add tcomps for the extra 1 needed for twos complemented partial products
  pp_matrix[pp_dim1-2] = new string[pp_dim2];
  for (int j = 0; j < m; j+=1){
    pp_matrix[pp_dim1-2][j] = "t_comp"  + to_string(j);
  }

  // for sign extension trick, add const1 to msb of the first pp
  pp_matrix[pp_dim1-1] = new string[pp_dim2];
  pp_matrix[pp_dim1-1][n] = "const1";

  print_pp(pp_matrix, pp_dim1, pp_dim2, verilog, true);

}

void create_signedbr2pp (int m, int n,
                         string**& pp_matrix,
                         int& pp_dim1,
                         int& pp_dim2,
                         std::queue<string>& verilog){

  verilog.push ( "wire logic const1;");
  verilog.push ( "assign const1 = 1'b1;");

  pp_dim1 = m+2; // number of rows
  pp_dim2 = (n+m); // number of columns

  pp_matrix = new string*[pp_dim1];

  /////
  verilog.push ("wire [" + to_string(n) + ":0] IN2_1x;");
  verilog.push ("assign IN2_1x = { IN2["+ to_string(n-1) +"],  IN2};");
  /////

  for (int i = 0; i < m; i++){
    verilog.push ("");
    verilog.push ("// Signed Booth Radix-2 Partial Products Row " + to_string(i+1));
    string m0 = (i == 0 ? "1'b0" : "IN1[" + to_string(i - 1) + "]");
    string m1 = "IN1[" + to_string(i) + "]";

    string select_e = "select_e_" + to_string(i);
    string select_ne = "select_ne_" + to_string(i);

    string cmp = "t_comp" + to_string(i);

    verilog.push ( "wire logic " + select_e
                   + ", " + select_ne
                   // + ", " + select_2x
                   + ", " + cmp
                   //+ ", " + select_n2x
                   + ";");

    verilog.push ("assign " + select_e +
                  " = (~ " + m1 + ") & (" + m0 + ");");// +
    //" | ((~ " + m2 + ") & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_ne +
                  " = ( " + m1 + ") & (~ " + m0 + ");");// +

    string cur_pp = "pp_" + to_string(i);
    verilog.push ("wire ["+ to_string(n+1) +":0] " + cur_pp + ";");
    verilog.push ("assign " + cur_pp + " = (1<<" +
                  to_string(n) +
                  ") ^ (" +
                  "("+ select_e  + " ? IN2_1x : 0) | " +
                  "(" +  select_ne + " ? (~ IN2_1x) : 0)" +
                  ");");
    verilog.push ("assign " + cmp + " = " + select_ne  + ";");

  }

  for (int i = 0; i < pp_dim1-2; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < i; j++){
      pp_matrix[i][j] = "";
    }
    for (int j = 0; j < n+1 && j+i < pp_dim2 ; j++){
      string cur;
      cur = "pp_" + to_string(i) + "[" + to_string(j) + "]";
      pp_matrix[i][j+i] = cur;
    }
  }

  // add tcomps for the extra 1 needed for twos complemented partial products
  pp_matrix[pp_dim1-2] = new string[pp_dim2];
  for (int j = 0; j < m; j+=1){
    pp_matrix[pp_dim1-2][j] = "t_comp"  + to_string(j);
  }

  // for sign extension trick, add const1 to msb of the first pp
  pp_matrix[pp_dim1-1] = new string[pp_dim2];
  pp_matrix[pp_dim1-1][n] = "const1";

  print_pp(pp_matrix, pp_dim1, pp_dim2, verilog, true);

}

void create_signedbr4pp (int m, int n,
                         string**& pp_matrix,
                         int& pp_dim1,
                         int& pp_dim2,
                         std::queue<string>& verilog){

  // m multiplier size, will round up to even.
  // n multiplicand size, will be padded.

  // m = 6;
  // n = 6;

  // verilog.push ( "wire logic [" + to_string(n+1) + ":0] eIN2;");
  // verilog.push ( "wire logic [" + to_string(n+1) + ":0] neIN2;");
  // verilog.push ( "wire logic [" + to_string(n+1) + ":0] twoxIN2;");
  // verilog.push ( "wire logic [" + to_string(n+1) + ":0] n2xIN2;");

  verilog.push ( "wire logic const1;");

  verilog.push ( "assign const1 = 1'b1;");

  // verilog.push ( "assign eIN2 = {IN2["
  //             + to_string(n-1) + "], IN2["
  //             + to_string(n-1)+ "], IN2[" + to_string(n-1) + ":0]};");

  // verilog.push ( "assign neIN2 = ~ eIN2;");
  // verilog.push ( "assign twoxIN2 = {eIN2[" + to_string(n) + ":0], 1'b0};");

  // verilog.push ( "assign n2xIN2 = ~ twoxIN2;");

  pp_dim1 = (m+1)/2+2; // number of rows
  pp_dim2 = (n+m); // number of columns

  pp_matrix = new string*[pp_dim1];

  // for (int i = 0; i< pp_dim1; i++)
  //   verilog.push ( "wire logic [" + to_string(n+1) + ":0] pp" + to_string(i) + ";");

  // cout << "here" << endl;

  //string e_lower = "eIN2[" + to_string(n) + ":0]";
  // string ne_lower = "neIN2[" + to_string(n) + ":0]";
  // string twox_lower = "twoxIN2[" + to_string(n) + ":0]";
  // string n2x_lower = "n2xIN2[" + to_string(n) + ":0]";

  // string e_upper = "( eIN2[" + to_string(n+1) + "])";
  // string ne_upper = "( neIN2[" + to_string(n+1) + "])";
  // string twox_upper = "( twoxIN2[" + to_string(n+1) + "])";
  // string n2x_upper = "( n2xIN2[" + to_string(n+1) +  "])";

  /////
  verilog.push ("wire [" + to_string(n+1) + ":0] IN2_1x;");
  verilog.push ("wire [" + to_string(n+1) + ":0] IN2_2x;");
  verilog.push ("assign IN2_1x = {IN2["+ to_string(n-1) +"],  IN2["+ to_string(n-1) +"],  IN2};");
  verilog.push ("assign IN2_2x = {IN2["+ to_string(n-1) +"],  IN2, 1'b0};");
  /////

  for (int i = 0; i < (m+1)/2; i++){
    verilog.push ("");
    verilog.push ("// Signed Booth Radix-4 Partial Products Row " + to_string(i+1));
    string m0 = (i == 0 ? "1'b0" : "IN1[" + to_string(i*2 - 1) + "]");
    string m1 = "IN1[" + to_string(i*2) + "]";
    string m2 = (i*2 == m-1 ? "IN1[" + to_string(i*2) + "]" : "IN1[" + to_string(i*2+1) + "]");

    string select_e = "select_e_" + to_string(i);
    string select_ne = "select_ne_" + to_string(i);
    string select_2x = "select_2x_" + to_string(i);
    string select_n2x = "select_n2x_" + to_string(i);
    string cmp = "tcomp" + to_string(i);

    verilog.push ( "wire logic " + select_e
                   + ", " + select_ne
                   + ", " + select_2x
                   + ", " + cmp
                   + ", " + select_n2x + ";");

    verilog.push ("assign " + select_e +
                  " = ((~ " + m2 + ") & (" + m1 + " ^ " + m0 + "));");// +
    //" | ((~ " + m2 + ") & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_ne +
                  " = " + m2 + " &  (" + m1 + " ^ " + m0 + ");");// +
    //" | (" + m2 + " & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_2x +
                  " = (~ " + m2 + ") & " + m1 + " & " + m0 + ";");

    verilog.push ("assign " + select_n2x +
                  " = " + m2 + " & (~ " + m1 + ") & (~ " + m0 + ");");

    //////
    string cur_pp = "pp_" + to_string(i);
    verilog.push ("wire ["+ to_string(n+1) +":0] " + cur_pp + ";");
    verilog.push ("assign " + cur_pp + " = (1<<" +
                  to_string(n+1) +
                  ") ^ (" +
                  "("+ select_e  + " ? IN2_1x : 0) | " +
                  "(" +  select_2x + " ? IN2_2x : 0) | " +
                  "(" +  select_n2x+ " ? (~ IN2_2x) : 0) | " +
                  "(" +  select_ne + " ? (~ IN2_1x) : 0)" +
                  ");");
    ////////

    // for (int j = 0; j < n+2; j++){
    //   string cur_pp = "pp_" + to_string(i) + "_" + to_string(j);

    //   string e_lower = "IN2[" + to_string(min(j,n-1)) + "]";
    //   string ne_lower = "(~ IN2[" + to_string(min(j,n-1)) + "])";
    //   string twox_lower = (j==0 ? "1'b0" : "IN2[" + to_string(min(j-1,n-1)) + "]");
    //   string n2x_lower = (j==0 ? "1'b1" : "(~ IN2[" + to_string(min(j-1,n-1)) + "])");
    //  //"n2xIN2[" + to_string(j) + "]";

    //   verilog.push ("wire logic " + cur_pp + ";");
    //   verilog.push ("assign " + cur_pp + " = " +
    //              (j==n+1 ? "~ " : "") +
    //              "(" +
    //              select_e  + " & " + e_lower    + " | " +
    //              select_2x + " & " + twox_lower + " | " +
    //              select_n2x+ " & " + n2x_lower  + " | " +
    //              select_ne + " & " + ne_lower   + "  " +

    //              ");");
    // }

    verilog.push ("assign " + cmp + " = " + select_ne + " | " + select_n2x + ";");

  }

  for (int i = 0; i < pp_dim1-2; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < i*2; j++){
      pp_matrix[i][j] = "";
    }
    for (int j = 0; j < n+2 && j+i*2 < pp_dim2 ; j++){
      string cur;
      cur = "pp_" + to_string(i) + "[" + to_string(j) + "]";
      pp_matrix[i][j+i*2] = cur;
    }
  }

  // add tcomps for the extra 1 needed for twos complemented partial products
  pp_matrix[pp_dim1-2] = new string[pp_dim2];
  for (int j = 0; j < m; j+=2){
    pp_matrix[pp_dim1-2][j] = "tcomp"  + to_string(j/2);
  }

  // add the const1's for sign extention trick
  for (int j = n+2, i = 0; j < pp_dim2 && i < pp_dim1; i++, j+=2){
    pp_matrix[i][j] = "const1";
  }

  // for sign extension trick, add const1 to msb of the first pp
  pp_matrix[pp_dim1-1] = new string[pp_dim2];
  pp_matrix[pp_dim1-1][n+1] = "const1";

  // cout << endl << "Printing the verilog part:" << endl << endl;
  //  while(!verilog.empty()) {
  //   cout << verilog.front() << endl;
  //   verilog.pop();
  // }

  //  cout << endl;

  print_pp(pp_matrix, pp_dim1, pp_dim2, verilog, true);

  //print_pp(pp_matrix, pp_dim1, pp_dim2);

}

void create_unsignedbr4pp (int m, int n,
                           string**& pp_matrix,
                           int& pp_dim1,
                           int& pp_dim2,
                           std::queue<string>& verilog){

  // m multiplier size, will round up to even.
  // n multiplicand size, will be padded.

  // m = 6;
  // n = 6;

  verilog.push ( "wire logic [" + to_string(m) + ":0] pIN1;");
  verilog.push ( "wire logic [" + to_string(n) + ":0] pIN2;");
  // verilog.push ( "wire logic [" + to_string(n+1) + ":0] neIN2;");
  // verilog.push ( "wire logic [" + to_string(n+1) + ":0] twoxIN2;");
  // verilog.push ( "wire logic [" + to_string(n+1) + ":0] n2xIN2;");

  verilog.push ( "wire logic const1;");

  verilog.push ( "assign const1 = 1'b1;");
  // verilog.push ( "assign const0 = 1'b0;");

  verilog.push ( "assign pIN1 = {1'b0, IN1[" + to_string(m-1) + ":0]};");
  verilog.push ( "assign pIN2 = {1'b0, IN2[" + to_string(n-1) + ":0]};");

  // verilog.push ( "assign eIN2 = {IN2["
  //             + to_string(n-1) + "], IN2["
  //             + to_string(n-1)+ "], IN2[" + to_string(n-1) + ":0]};");

  // verilog.push ( "assign neIN2 = ~ eIN2;");
  // verilog.push ( "assign twoxIN2 = {eIN2[" + to_string(n) + ":0], 1'b0};");

  // verilog.push ( "assign n2xIN2 = ~ twoxIN2;");

  m++; n++; //pad with zeros.

  pp_dim1 = (m+1)/2+2; // number of rows
  pp_dim2 = (n+m); // number of columns

  pp_matrix = new string*[pp_dim1];

  // for (int i = 0; i< pp_dim1; i++)
  //   verilog.push ( "wire logic [" + to_string(n+1) + ":0] pp" + to_string(i) + ";");

  // cout << "here" << endl;

  //string e_lower = "eIN2[" + to_string(n) + ":0]";
  // string ne_lower = "neIN2[" + to_string(n) + ":0]";
  // string twox_lower = "twoxIN2[" + to_string(n) + ":0]";
  // string n2x_lower = "n2xIN2[" + to_string(n) + ":0]";

  // string e_upper = "( eIN2[" + to_string(n+1) + "])";
  // string ne_upper = "( neIN2[" + to_string(n+1) + "])";
  // string twox_upper = "( twoxIN2[" + to_string(n+1) + "])";
  // string n2x_upper = "( n2xIN2[" + to_string(n+1) +  "])";

  for (int i = 0; i < (m+1)/2; i++){
    verilog.push ("");
    verilog.push ("// Signed Booth Radix-4 Partial Products Row " + to_string(i+1));
    string m0 = (i == 0 ? "1'b0" : "pIN1[" + to_string(i*2 - 1) + "]");
    string m1 = "pIN1[" + to_string(i*2) + "]";
    string m2 = (i*2 == m-1 ? "pIN1[" + to_string(i*2) + "]" : "pIN1[" + to_string(i*2+1) + "]");

    string select_e = "select_e_" + to_string(i);
    string select_ne = "select_ne_" + to_string(i);
    string select_2x = "select_2x_" + to_string(i);
    string select_n2x = "select_n2x_" + to_string(i);
    string cmp = "tcomp" + to_string(i);

    verilog.push ( "wire logic " + select_e
                   + ", " + select_ne
                   + ", " + select_2x
                   + ", " + cmp
                   + ", " + select_n2x + ";");

    verilog.push ("assign " + select_e +
                  " = ((~ " + m2 + ") & (" + m1 + " ^ " + m0 + "));");// +
    //" | ((~ " + m2 + ") & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_ne +
                  " = " + m2 + " &  (" + m1 + " ^ " + m0 + ");");// +
    //" | (" + m2 + " & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_2x +
                  " = (~ " + m2 + ") & " + m1 + " & " + m0 + ";");

    verilog.push ("assign " + select_n2x +
                  " = " + m2 + " & (~ " + m1 + ") & (~ " + m0 + ");");

    for (int j = 0; j < n+2; j++){
      string cur_pp = "pp_" + to_string(i) + "_" + to_string(j);

      string e_lower = "pIN2[" + to_string(min(j,n-1)) + "]";
      string ne_lower = "(~ pIN2[" + to_string(min(j,n-1)) + "])";
      string twox_lower = (j==0 ? "1'b0" : "pIN2[" + to_string(min(j-1,n-1)) + "]");
      string n2x_lower = (j==0 ? "1'b1" : "(~ pIN2[" + to_string(min(j-1,n-1)) + "])");
      //"n2xIN2[" + to_string(j) + "]";

      verilog.push ("wire logic " + cur_pp + ";");
      verilog.push ("assign " + cur_pp + " = " +
                    (j==n+1 ? "~ " : "")
                    +
                    "(" +
                    select_e  + " & " + e_lower    + " | " +
                    select_2x + " & " + twox_lower + " | " +
                    select_n2x+ " & " + n2x_lower  + " | " +
                    select_ne + " & " + ne_lower   + "  " +

                    ");");
    }

    verilog.push ("assign " + cmp + " = " + select_ne + " | " + select_n2x + ";");

  }

  for (int i = 0; i < pp_dim1-1; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < i*2; j++){
      pp_matrix[i][j] = "";
    }
    for (int j = 0; j < n+2 && j+i*2 < pp_dim2 ; j++){
      string cur;
      cur = "pp_" + to_string(i) + "_" + to_string(j) + "";
      pp_matrix[i][j+i*2] = cur;
    }
  }

  // // add tcomps for the extra 1 needed for twos complemented partial products
  pp_matrix[pp_dim1-2] = new string[pp_dim2];
  for (int j = 0; j < m; j+=2){
    pp_matrix[pp_dim1-2][j] = "tcomp"  + to_string(j/2);
  }

  // // add the const1's for sign extention trick
  for (int j = n+2, i = 0; j < pp_dim2 && i < pp_dim1; i++, j+=2){
    pp_matrix[i][j] = "const1";
  }

  // // for sign extension trick, add const1 to msb of the first pp
  pp_matrix[pp_dim1-1] = new string[pp_dim2];
  pp_matrix[pp_dim1-1][n+1] = "const1";

  // cout << endl << "Printing the verilog part:" << endl << endl;
  //  while(!verilog.empty()) {
  //   cout << verilog.front() << endl;
  //   verilog.pop();
  // }

  //  cout << endl;

  //print_pp(pp_matrix, pp_dim1, pp_dim2);

  print_pp(pp_matrix, pp_dim1, pp_dim2, verilog, true);

}








void create_br2pp (int m, int n, bool signed_mul,
                   string**& pp_matrix,
                   int& pp_dim1,
                   int& pp_dim2,
		   std::list<int>& extra_ones_indices,
                   std::queue<string>& verilog){

  // m multiplier size,
  // n multiplicand size,

  if (signed_mul) {
    verilog.push ("wire [" + to_string(m-1) + ":0] mult = IN1;");
    verilog.push ("wire [" + to_string(n-1) + ":0] mcand = IN2;");
  } else {
    verilog.push ("wire [" + to_string(m) + ":0] mult = {1'b0, IN1};");
    verilog.push ("wire [" + to_string(n) + ":0] mcand = {1'b0, IN2};");
    m++;
    n++;
  }

  int pp_msb = n-1;
  pp_dim1 = m+2; // number of rows
  int size_to_fit_all_pp = pp_msb + m;
  pp_dim2 = max(n+m, size_to_fit_all_pp); // number of columns

  pp_matrix = new string*[pp_dim1];
  // make sure it starts empty.  
  for (int i = 0; i < pp_dim1; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < pp_dim2; j++)
      pp_matrix[i][j] = ""; 			 
  }
  

  /////
  // verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_1x;");
  // verilog.push ("assign mcand_1x = {{"+to_string(pp_msb - n + 1)+"{mcand["+ to_string(n-1) +"]}},  mcand};");
  
  /////

  for (int i = 0; i < m; i++){
    verilog.push ("");
    string m0 = (i == 0 ? "1'b0" : "mult[" + to_string(i-1) + "]");
    string m1 = "mult[" + to_string(i) + "]";
   
    verilog.push ("// Signed Booth Radix-2 Partial Products. Multiplier selectors: " + m1 + " " + m0);

    string select_e = "select_e_" + to_string(i);
    string select_ne = "select_ne_" + to_string(i);
    string cmp = "tcomp" + to_string(i);

    verilog.push ( "wire logic " + select_e
                   + ", " + cmp
                   + ", " + select_ne
                   + ";");

   verilog.push ("assign " + select_e +
                  " = ~" + m1 + " & " + m0 + ";");// +
    //" | ((~ " + m2 + ") & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_ne +
                  " = " + m1 + " & ~" + m0 + ";");// +

   
    //////
    string cur_pp = "pp_" + to_string(i);
    verilog.push ("wire ["+ to_string(pp_msb) +":0] " + cur_pp + ";");
    verilog.push ("assign " + cur_pp + " = (1<<" + to_string(pp_msb) + ") ^ // flip the MSB ");
    verilog.push ("               ((" +  select_e   + " ? mcand : 0) | ");
    verilog.push ("                (" +  select_ne  + " ? (~ mcand) : 0)); " );

    verilog.push ("assign " + cmp + " = " + select_ne + ";");

  }

  for (int i = 0; i < pp_dim1-2; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < i; j++){
      pp_matrix[i][j] = "";
    }
    for (int j = 0; j <= pp_msb && j+i < pp_dim2 ; j++){
      string cur;
      cur = "pp_" + to_string(i) + "[" + to_string(j) + "]";
      pp_matrix[i][j+i] = cur;
    }
  }

  // add tcomps for the extra 1 needed for twos complemented partial products
  pp_matrix[pp_dim1-2] = new string[pp_dim2];
  for (int j = 0; j < m; j+=1){
    pp_matrix[pp_dim1-2][j] = "tcomp"  + to_string(j);
  }

  for (int j = pp_msb; j < size_to_fit_all_pp; j += 1){
    extra_ones_indices.push_back(j);
  }
}



void create_br4pp (int m, int n, bool signed_mul,
                   string**& pp_matrix,
                   int& pp_dim1,
                   int& pp_dim2,
		   std::list<int>& extra_ones_indices,
                   std::queue<string>& verilog){

  // m multiplier size,
  // n multiplicand size,

  if (signed_mul) {
    verilog.push ("wire [" + to_string(m-1) + ":0] mult = IN1;");
    verilog.push ("wire [" + to_string(n-1) + ":0] mcand = IN2;");
  } else {
    verilog.push ("wire [" + to_string(m) + ":0] mult = {1'b0, IN1};");
    verilog.push ("wire [" + to_string(n) + ":0] mcand = {1'b0, IN2};");
    m++;
    n++;
  }

  int pp_msb = n;
  pp_dim1 = (m+1)/2+2; // number of rows
  int size_to_fit_all_pp = pp_msb + 1 + 2*((m+1)/2-1);
  pp_dim2 = max(n+m, size_to_fit_all_pp); // number of columns

  pp_matrix = new string*[pp_dim1];
  // make sure it starts empty.  
  for (int i = 0; i < pp_dim1; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < pp_dim2; j++)
      pp_matrix[i][j] = ""; 			 
  }
  

  /////
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_1x;");
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_2x;");
  verilog.push ("assign mcand_1x = {{"+to_string(pp_msb - n + 1)+"{mcand["+ to_string(n-1) +"]}},  mcand};");
  verilog.push ("assign mcand_2x = {{"+to_string(pp_msb - n)+"{mcand["+ to_string(n-1) +"]}},  mcand, 1'b0};");
  
  /////

  for (int i = 0; i < m; i+=2){
    verilog.push ("");
    string m0 = (i == 0 ? "1'b0" : "mult[" + to_string(i-1) + "]");
    string m1 = "mult[" + to_string(i) + "]";
    string m2 = (i >= m-1 ? "mult[" + to_string(m-1) + "]" : "mult[" + to_string(i+1) + "]");

    verilog.push ("// Signed Booth Radix-4 Partial Products. Multiplier selectors: " + m2 + " " + m1 + " " + m0);

    string select_e = "select_e_" + to_string(i/2);
    string select_ne = "select_ne_" + to_string(i/2);
    string select_2x = "select_2x_" + to_string(i/2);
    string select_n2x = "select_n2x_" + to_string(i/2);
    string cmp = "tcomp" + to_string(i/2);

    verilog.push ( "wire logic " + select_e
                   + ", " + select_2x
                   + ", " + cmp
                   + ", " + select_ne
                   + ", " + select_n2x
                   + ";");

   verilog.push ("assign " + select_e +
                  " = ((~ " + m2 + ") & (" + m1 + " ^ " + m0 + "));");// +
    //" | ((~ " + m2 + ") & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_ne +
                  " = " + m2 + " &  (" + m1 + " ^ " + m0 + ");");// +
    //" | (" + m2 + " & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_2x +
                  " = (~ " + m2 + ") & " + m1 + " & " + m0 + ";");

    verilog.push ("assign " + select_n2x +
                  " = " + m2 + " & (~ " + m1 + ") & (~ " + m0 + ");");

    //////
    string cur_pp = "pp_" + to_string(i/2);
    verilog.push ("wire ["+ to_string(pp_msb) +":0] " + cur_pp + ";");
    verilog.push ("assign " + cur_pp + " = (1<<" + to_string(pp_msb) + ") ^ // flip the MSB ");
    verilog.push ("               ((" +  select_e   + " ? mcand_1x : 0) | ");
    verilog.push ("                (" +  select_2x  + " ? mcand_2x : 0) | " );
    verilog.push ("                (" +  select_n2x + " ? (~ mcand_2x) : 0) | " );
    verilog.push ("                (" +  select_ne  + " ? (~ mcand_1x) : 0)); " );

    verilog.push ("assign " + cmp + " =  " + select_n2x +" | " + select_ne + ";");

  }

  for (int i = 0; i < pp_dim1-2; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < i*2; j++){
      pp_matrix[i][j] = "";
    }
    for (int j = 0; j <= pp_msb && j+i*2 < pp_dim2 ; j++){
      string cur;
      cur = "pp_" + to_string(i) + "[" + to_string(j) + "]";
      pp_matrix[i][j+i*2] = cur;
    }
  }

  // add tcomps for the extra 1 needed for twos complemented partial products
  pp_matrix[pp_dim1-2] = new string[pp_dim2];
  for (int j = 0; j < m; j+=2){
    pp_matrix[pp_dim1-2][j] = "tcomp"  + to_string(j/2);
  }

  for (int j = pp_msb; j < size_to_fit_all_pp; j += 2){
    extra_ones_indices.push_back(j);
  }
}


void create_br8pp (int m, int n, bool signed_mul,
                   string**& pp_matrix,
                   int& pp_dim1,
                   int& pp_dim2,
		   std::list<int>& extra_ones_indices,
                   std::queue<string>& verilog){

  // m multiplier size,
  // n multiplicand size,

  verilog.push ( "wire logic const1;");

  verilog.push ( "assign const1 = 1'b1;");

  if (signed_mul) {
    verilog.push ("wire [" + to_string(m-1) + ":0] mult = IN1;");
    verilog.push ("wire [" + to_string(n-1) + ":0] mcand = IN2;");
  } else {
    verilog.push ("wire [" + to_string(m) + ":0] mult = {1'b0, IN1};");
    verilog.push ("wire [" + to_string(n) + ":0] mcand = {1'b0, IN2};");
    m++;
    n++;
  }

  int pp_msb = n+1;

  
  pp_dim1 = (m+2)/3+2; // number of rows
  int size_to_fit_all_pp = pp_msb+1 + 3*((m+2)/3-1); 
  pp_dim2 = max(n+m, size_to_fit_all_pp); // number of columns

  pp_matrix = new string*[pp_dim1];
  // make sure it starts empty.  
  for (int i = 0; i < pp_dim1; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < pp_dim2; j++)
      pp_matrix[i][j] = ""; 			 
  }
  

  /////
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_1x;");
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_2x;");
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_3x;");
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_4x;");
  verilog.push ("assign mcand_1x = {{"+to_string(pp_msb - n + 1)+"{mcand["+ to_string(n-1) +"]}},  mcand};");
  verilog.push ("assign mcand_2x = {{"+to_string(pp_msb - n)+"{mcand["+ to_string(n-1) +"]}},  mcand, 1'b0};");
  verilog.push ("assign mcand_3x = mcand_1x + mcand_2x;");
  verilog.push ("assign mcand_4x = {{"+to_string(pp_msb - n - 1)+"{mcand["+ to_string(n-1) +"]}},  mcand, 2'b0};");
  /////

  for (int i = 0; i < m; i+=3){
    verilog.push ("");
    string m0 = (i == 0 ? "1'b0" : "mult[" + to_string(i-1) + "]");
    string m1 = "mult[" + to_string(i) + "]";
    string m2 = (i >= m-1 ? "mult[" + to_string(m-1) + "]" : "mult[" + to_string(i+1) + "]");
    string m3 = (i >= m-2 ? "mult[" + to_string(m-1) + "]" : "mult[" + to_string(i+2) + "]");

    verilog.push ("// Signed Booth Radix-8 Partial Products. Multiplier selectors: " + m3 + " " + m2 + " " + m1 + " " + m0);

    string select_e = "select_e_" + to_string(i/3);
    string select_ne = "select_ne_" + to_string(i/3);
    string select_2x = "select_2x_" + to_string(i/3);
    string select_n2x = "select_n2x_" + to_string(i/3);
    string select_3x = "select_3x_" + to_string(i/3);
    string select_4x = "select_4x_" + to_string(i/3);
    string select_n3x = "select_n3x_" + to_string(i/3);
    string select_n4x = "select_n4x_" + to_string(i/3);
    string cmp = "tcomp" + to_string(i/3);

    verilog.push ( "wire logic " + select_e
                   + ", " + select_2x
                   + ", " + select_3x
                   + ", " + select_4x
                   + ", " + cmp
                   + ", " + select_ne
                   + ", " + select_n2x
                   + ", " + select_n3x
                   + ", " + select_n4x + ";");

    verilog.push ("assign " + select_e +
                  " = ((~ " + m3 + ") & (~ " + m2 + ") & (" + m1 + " ^ " + m0 + "));");// +
    //" | ((~ " + m2 + ") & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_ne +
                  " = " + m3 + " & " + m2 + " &  (" + m1 + " ^ " + m0 + ");");// +
    //" | (" + m2 + " & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_2x +
                  " = (~ " + m3 + ") " + "& (" + m2 + " ^ " + m1 + ")" + "& (" + m2 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_n2x +
                  " = " + m3 + " " + "& (" + m2 + " ^ " + m1 + ")" + "& (" + m2 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_3x +
                  " = ((~ " + m3 + ") & " + m2 + " & (" + m1 + " ^ " + m0 + "));");

    verilog.push ("assign " + select_n3x +
                  " = (" + m3 + " & ~" + m2 + " & (" + m1 + " ^ " + m0 + "));");

    verilog.push ("assign " + select_4x +
                  " = ~" + m3 + " & " + m2 + " & " + m1 + " & " + m0+ ";");
    verilog.push ("assign " + select_n4x +
                  " = " + m3 + " & ~" + m2 + " & ~" + m1 + " & ~" + m0+ ";");

    //////
    string cur_pp = "pp_" + to_string(i/3);
    verilog.push ("wire ["+ to_string(pp_msb) +":0] " + cur_pp + ";");
    verilog.push ("assign " + cur_pp + " = (1<<" + to_string(pp_msb) + ") ^ // flip the MSB ");
    verilog.push ("               ((" +  select_e   + " ? mcand_1x : 0) | ");
    verilog.push ("                (" +  select_2x  + " ? mcand_2x : 0) | " );
    verilog.push ("                (" +  select_3x  + " ? mcand_3x : 0) | " );
    verilog.push ("                (" +  select_4x  + " ? mcand_4x : 0) | " );
    verilog.push ("                (" +  select_n4x + " ? (~ mcand_4x) : 0) | " );
    verilog.push ("                (" +  select_n3x + " ? (~ mcand_3x) : 0) | " );
    verilog.push ("                (" +  select_n2x + " ? (~ mcand_2x) : 0) | " );
    verilog.push ("                (" +  select_ne  + " ? (~ mcand_1x) : 0)); " );

    verilog.push ("assign " + cmp + " = " + select_ne + " | " + select_n4x +" | " + select_n3x +" | " + select_n2x + ";");

  }

  for (int i = 0; i < pp_dim1-2; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < i*3; j++){
      pp_matrix[i][j] = "";
    }
    for (int j = 0; j <= pp_msb && j+i*3 < pp_dim2 ; j++){
      string cur;
      cur = "pp_" + to_string(i) + "[" + to_string(j) + "]";
      pp_matrix[i][j+i*3] = cur;
    }
  }

  // add tcomps for the extra 1 needed for twos complemented partial products
  pp_matrix[pp_dim1-2] = new string[pp_dim2];
  for (int j = 0; j < m; j+=3){
    pp_matrix[pp_dim1-2][j] = "tcomp"  + to_string(j/3);
  }

  // // add the const1's for sign extention trick
  // for (int j = pp_msb+1, i = 0; j < pp_dim2 && i < pp_dim1-2; i++, j+=3){
  //   pp_matrix[i][j] = "const1";
  //   if (j + 1 < pp_dim2)
  //     pp_matrix[i][j+1] = "const1";
  // }

  // // for sign extension trick, add const1 to msb of the first pp
  // pp_matrix[pp_dim1-1] = new string[pp_dim2];
  // pp_matrix[pp_dim1-1][pp_msb] = "const1";

  // print_pp(pp_matrix, pp_dim1, pp_dim2, verilog, true);

  for (int j = pp_msb; j < size_to_fit_all_pp; j += 3){
    extra_ones_indices.push_back(j);
  }
}

void create_br16pp (int m, int n, bool signed_mul,
                    string**& pp_matrix,
                    int& pp_dim1,
                    int& pp_dim2,
                    std::list<int>& extra_ones_indices,
                    std::queue<string>& verilog){

  // m multiplier size, may need to round up to 3x+2, e.g., 5 8 11
  // n multiplicand size, will be padded.

  // m = 6;
  // n = 6;

  // verilog.push ( "wire logic const1;");

  // verilog.push ( "assign const1 = 1'b1;");

  if (signed_mul) {
    verilog.push ("wire [" + to_string(m-1) + ":0] mult = IN1;");
    verilog.push ("wire [" + to_string(n-1) + ":0] mcand = IN2;");
  } else {
    verilog.push ("wire [" + to_string(m) + ":0] mult = {1'b0, IN1};");
    verilog.push ("wire [" + to_string(n) + ":0] mcand = {1'b0, IN2};");
    m++;
    n++;
  }

  int pp_msb = n+2;

  pp_dim1 = (m+3)/4+2; // number of rows
  //pp_dim2 = (n+m); // number of columns
  int size_to_fit_all_pp = pp_msb+1 + 4*((m+3)/4 -1);
  pp_dim2 = max(n+m, pp_msb+1 + 4*((m+3)/4 -1)); // give enough number of bits to cover all pps (might be more than outsize).
  // make sure it starts empty. 
  pp_matrix = new string*[pp_dim1];
  for (int i = 0; i < pp_dim1; i++){
    pp_matrix[i] = new string[pp_dim2];
    for (int j = 0; j < pp_dim2; j++)
      pp_matrix[i][j] = "";  			 
  }

  /////
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_1x;");
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_2x;");
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_3x;");
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_4x;");
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_5x;");
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_6x;");
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_7x;");
  verilog.push ("wire [" + to_string(pp_msb) + ":0] mcand_8x;");
  verilog.push ("assign mcand_1x = {{"+to_string(pp_msb - n + 1)+"{mcand["+ to_string(n-1) +"]}},  mcand};");
  verilog.push ("assign mcand_2x = {{"+to_string(pp_msb - n)+"{mcand["+ to_string(n-1) +"]}},  mcand, 1'b0};");
  verilog.push ("assign mcand_3x = mcand_1x + mcand_2x;");
  verilog.push ("assign mcand_4x = {{"+to_string(pp_msb - n - 1)+"{mcand["+ to_string(n-1) +"]}},  mcand, 2'b0};");
  verilog.push ("assign mcand_5x = mcand_1x + mcand_4x;");
  verilog.push ("assign mcand_6x = {mcand_3x[" + to_string(pp_msb-1) + ":0], 1'b0};");
  verilog.push ("assign mcand_7x = mcand_8x - mcand_1x;");
  verilog.push ("assign mcand_8x =  {{"+to_string(pp_msb - n - 2)+"{mcand["+ to_string(n-1) +"]}},  mcand, 3'b0};");

  /////

  for (int i = 0; i < m; i+=4){
    verilog.push ("");
    string m0 = (i == 0 ? "1'b0" : "mult[" + to_string(i-1) + "]");
    string m1 = "mult[" + to_string(i) + "]";
    string m2 = (i >= m-1 ? "mult[" + to_string(m-1) + "]" : "mult[" + to_string(i+1) + "]");
    string m3 = (i >= m-2 ? "mult[" + to_string(m-1) + "]" : "mult[" + to_string(i+2) + "]");
    string m4 = (i >= m-3 ? "mult[" + to_string(m-1) + "]" : "mult[" + to_string(i+3) + "]");

    verilog.push ("// Signed Booth Radix-16 Partial Products. Multiplier selectors: " + m4 + " " + m3 + " " + m2 + " " + m1 + " " + m0);

    string select_e = "select_e_" + to_string(i/4);
    string select_ne = "select_ne_" + to_string(i/4);
    string select_2x = "select_2x_" + to_string(i/4);
    string select_n2x = "select_n2x_" + to_string(i/4);
    string select_3x = "select_3x_" + to_string(i/4);
    string select_n3x = "select_n3x_" + to_string(i/4);
    string select_4x = "select_4x_" + to_string(i/4);
    string select_n4x = "select_n4x_" + to_string(i/4);
    string select_5x = "select_5x_" + to_string(i/4);
    string select_n5x = "select_n5x_" + to_string(i/4);
    string select_6x = "select_6x_" + to_string(i/4);
    string select_n6x = "select_n6x_" + to_string(i/4);
    string select_7x = "select_7x_" + to_string(i/4);
    string select_n7x = "select_n7x_" + to_string(i/4);
    string select_8x = "select_8x_" + to_string(i/4);
    string select_n8x = "select_n8x_" + to_string(i/4);
    string cmp = "tcomp" + to_string(i/4);

    verilog.push ( "wire logic " + select_e
                   + ", " + select_2x
                   + ", " + select_3x
                   + ", " + select_4x
                   + ", " + select_5x
                   + ", " + select_6x
                   + ", " + select_7x
                   + ", " + select_8x
                   + ", " + cmp
                   + ", " + select_ne
                   + ", " + select_n2x
                   + ", " + select_n3x
                   + ", " + select_n4x +
                   + ", " + select_n5x +
                   + ", " + select_n6x +
                   + ", " + select_n7x +
                   + ", " + select_n8x +
                   ";");

    verilog.push ("assign " + select_e +
                  " = ((~ " + m4 + ") & (~ " + m3 + ") & (~ " + m2 + ") & (" + m1 + " ^ " + m0 + "));");// +
    //" | ((~ " + m2 + ") & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_2x +
                  " = (~ " + m4 + ") & (~ " + m3 + ") " + "& (" + m2 + " ^ " + m1 + ")" + "& (" + m2 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_3x +
                  " = (~ " + m4 + ") & (~ " + m3 + ") & " + m2 + " & (" + m1 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_4x +
                  " = (~" + m4 + ") &  (" + m3 + " ^ " + m2 + ") " + "& (" + m3 + " ^ " + m1 + ")" + "& (" + m3 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_5x +
                  " =  (~" + m4 + ") &  " + m3 + " & (~" + m2 + ") & (" + m1 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_6x +
                  " = (~" + m4 + ") & " + m3 + " " + "& (" + m2 + " ^ " + m1 + ")" + "& (" + m2 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_7x +
                  " =  (~" + m4 + ") &  " + m3 + " & " + m2 + " & (" + m1 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_8x +
                  " =  (~" + m4 + ") &  " + m3 + " & " + m2 + " & " + m1 + " & " + m0 + ";");
    verilog.push ("assign " + select_n8x +
                  " =  " + m4 + " &  (~" + m3 + ") & (~" + m2 + ") & (~" + m1 + ") & (~" + m0 + ");");

    verilog.push ("assign " + select_n7x +
                  " = (( " + m4 + ") & (~ " + m3 + ") & (~ " + m2 + ") & (" + m1 + " ^ " + m0 + "));");// +
    //" | ((~ " + m2 + ") & (~ " + m0 + ") & " + m1 + ");");

    verilog.push ("assign " + select_n6x +
                  " = (" + m4 + ") & (~ " + m3 + ") " + "& (" + m2 + " ^ " + m1 + ")" + "& (" + m2 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_n5x +
                  " = (" + m4 + ") & (~ " + m3 + ") & " + m2 + " & (" + m1 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_n4x +
                  " = (" + m4 + ") &  (" + m3 + " ^ " + m2 + ") " + "& (" + m3 + " ^ " + m1 + ")" + "& (" + m3 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_n3x +
                  " =  (" + m4 + ") &  " + m3 + " & (~" + m2 + ") & (" + m1 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_n2x +
                  " = (" + m4 + ") & " + m3 + " " + "& (" + m2 + " ^ " + m1 + ")" + "& (" + m2 + " ^ " + m0 + ");");

    verilog.push ("assign " + select_ne +
                  " =  (" + m4 + ") &  " + m3 + " & " + m2 + " & (" + m1 + " ^ " + m0 + ");");

    //
    string cur_pp = "pp_" + to_string(i/4);
    verilog.push ("wire ["+ to_string(pp_msb) +":0] " + cur_pp + ";");
    verilog.push ("assign " + cur_pp + " = (1<<" + to_string(pp_msb) + ") ^ // flip the MSB ");
    verilog.push ("               ((" +  select_e   + " ? mcand_1x : 0) | ");
    verilog.push ("                (" +  select_2x  + " ? mcand_2x : 0) | " );
    verilog.push ("                (" +  select_3x  + " ? mcand_3x : 0) | " );
    verilog.push ("                (" +  select_4x  + " ? mcand_4x : 0) | " );
    verilog.push ("                (" +  select_5x  + " ? mcand_5x : 0) | " );
    verilog.push ("                (" +  select_6x  + " ? mcand_6x : 0) | " );
    verilog.push ("                (" +  select_7x  + " ? mcand_7x : 0) | " );
    verilog.push ("                (" +  select_8x  + " ? mcand_8x : 0) | " );
    verilog.push ("                (" +  select_n8x + " ? (~ mcand_8x) : 0) | " );
    verilog.push ("                (" +  select_n7x + " ? (~ mcand_7x) : 0) | " );
    verilog.push ("                (" +  select_n6x + " ? (~ mcand_6x) : 0) | " );
    verilog.push ("                (" +  select_n5x + " ? (~ mcand_5x) : 0) | " );
    verilog.push ("                (" +  select_n4x + " ? (~ mcand_4x) : 0) | " );
    verilog.push ("                (" +  select_n3x + " ? (~ mcand_3x) : 0) | " );
    verilog.push ("                (" +  select_n2x + " ? (~ mcand_2x) : 0) | " );
    verilog.push ("                (" +  select_ne  + " ? (~ mcand_1x) : 0)); " );

    verilog.push ("assign " + cmp + " = " + select_ne + " | " + select_n8x +" | " + select_n7x +" | " + select_n6x +" | " + select_n5x + " | " + select_n4x +" | " + select_n3x +" | " + select_n2x + ";");

  }

  for (int i = 0; i < pp_dim1-2; i++){
    for (int j = 0; j <= pp_msb && j+i*4 < pp_dim2 ; j++){

      string cur;
      cur = "pp_" + to_string(i) + "[" + to_string(j) + "]";
      pp_matrix[i][j+i*4] = cur;
    }
  }

  // add tcomps for the extra 1 needed for twos complemented partial products
  for (int j = 0; j < m; j+=4){
    pp_matrix[pp_dim1-2][j] = "tcomp"  + to_string(j/4);
  }

  for (int j = pp_msb; j < size_to_fit_all_pp; j += 4){
    extra_ones_indices.push_back(j);
  }

}
