



#include <iostream>
#include <queue>          // std::queue
#include <fstream>
#include <algorithm>


using namespace std;

#include "pp_gen.h"

void print_pp (string** pp, int m, int n){

  for (int i=0; i<m; i++){
    for (int j=0; j<n; j++){
      if (pp[i][j] != "")
	cout << pp[i][j] << " ";
      else
	cout << "   -   ";
    }
  cout << endl;
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
 

  verilog.push ( "wire logic [" + to_string(n+1) + ":0] eIN2;");
  verilog.push ( "wire logic [" + to_string(n+1) + ":0] neIN2;");
  verilog.push ( "wire logic [" + to_string(n+1) + ":0] twoxIN2;");
  verilog.push ( "wire logic [" + to_string(n+1) + ":0] n2xIN2;");

  verilog.push ( "wire logic const1;");

  verilog.push ( "assign const1 = 1'b1;");


  // verilog.push ( "assign eIN2 = {IN2["
  // 		 + to_string(n-1) + "], IN2["
  // 		 + to_string(n-1)+ "], IN2[" + to_string(n-1) + ":0]};");

  // verilog.push ( "assign neIN2 = ~ eIN2;");
  // verilog.push ( "assign twoxIN2 = {eIN2[" + to_string(n) + ":0], 1'b0};");

  // verilog.push ( "assign n2xIN2 = ~ twoxIN2;");


  pp_dim1 = (m+1)/2+2; // number of rows
  pp_dim2 = (n+m); // number of columns

  pp_matrix = new string*[pp_dim1];
  
  // for (int i = 0; i< pp_dim1; i++)
  //   verilog.push ( "wire logic [" + to_string(n+1) + ":0] pp" + to_string(i) + ";");

  // cout << "here" << endl;

  string e_lower = "eIN2[" + to_string(n) + ":0]";
  // string ne_lower = "neIN2[" + to_string(n) + ":0]";
  // string twox_lower = "twoxIN2[" + to_string(n) + ":0]";
  // string n2x_lower = "n2xIN2[" + to_string(n) + ":0]";

  // string e_upper = "( eIN2[" + to_string(n+1) + "])";
  // string ne_upper = "( neIN2[" + to_string(n+1) + "])";
  // string twox_upper = "( twoxIN2[" + to_string(n+1) + "])";
  // string n2x_upper = "( n2xIN2[" + to_string(n+1) +  "])";

  for (int i = 0; i < (m+1)/2; i++){
    verilog.push ("");
    verilog.push ("//Booth Radix-4 Partial Products Row " + to_string(i+1));
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
    

    // string pp_lower_v = "assign pp" + to_string(i) + "[" +  to_string(n) + ":0] = ";

    // pp_lower_v += "(" + select_2x + " == 1) ? " + twox_lower + " : " +
    //   "(" + select_ne + " == 1) ? " + ne_lower + " : " +
    //   "(" + select_e + " == 1) ? " + e_lower + " : " +
    //   "(" + select_n2x + " == 1) ? " + n2x_lower + " : " +
    //   "0;";

    for (int j = 0; j < n+2; j++){
      string cur_pp = "pp_" + to_string(i) + "_" + to_string(j);
      
      string e_lower = "IN2[" + to_string(min(j,n-1)) + "]";
      string ne_lower = "(~ IN2[" + to_string(min(j,n-1)) + "])";
      string twox_lower = (j==0 ? "1'b0" : "IN2[" + to_string(min(j-1,n-1)) + "]");
      string n2x_lower = (j==0 ? "1'b1" : "(~ IN2[" + to_string(min(j-1,n-1)) + "])");
	//"n2xIN2[" + to_string(j) + "]";
  
      verilog.push ("wire logic " + cur_pp + ";");
      verilog.push ("assign " + cur_pp + " = " +
		    (j==n+1 ? "~ " : "") +
		    "(" +
		    select_e  + " & " + e_lower    + " | " + 
		    select_2x + " & " + twox_lower + " | " +
		    select_n2x+ " & " + n2x_lower  + " | " +
		    select_ne + " & " + ne_lower   + "  " +
		   
		    ");");
    }

    // string pp_upper_v = "assign pp" + to_string(i) + "[" +  to_string(n+1) + "] = ";

    // pp_upper_v += "~ ( (" + select_e + " == 1) ? " + e_upper + " : " +
    //   "(" + select_ne + " == 1) ? " + ne_upper + " : " +
    //   "(" + select_2x + " == 1) ? " + twox_upper + " : " +
    //   "(" + select_n2x + " == 1) ? " + n2x_upper + " : " +
    //   "1'b0);";

    // verilog.push (pp_lower_v);
    // verilog.push (pp_upper_v);

    verilog.push ("assign " + cmp + " = " + select_ne + " | " + select_n2x + ";");
    
  }


  for (int i = 0; i < pp_dim1-2; i++){
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


  
    
   

   //print_pp(pp_matrix, pp_dim1, pp_dim2);

   
}
