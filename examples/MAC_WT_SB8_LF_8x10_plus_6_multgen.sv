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

// DO NOT REMOVE:
// This file is generated by Temel's multiplier generator. Download from https://github.com/temelmertcan/multgen.


// Specification module to help understand what the design implements.
module MAC_WT_SB8_LF_8x10_plus_6_spec (
        input logic [7:0] IN1,
        input logic [9:0] IN2,
        input logic [5:0] IN3,
        output logic design_is_correct, // is set to 1 iff the output of MAC_WT_SB8_LF_8x10_plus_6  matches its spec.
        output logic [18:0] design_res,
        output logic [18:0] spec_res);
    
    assign spec_res = signed'(IN1) * signed'(IN2) + signed'(IN3);
    MAC_WT_SB8_LF_8x10_plus_6 mult(IN1, IN2, IN3, design_res);
    assign design_is_correct = ((spec_res == design_res) ? 1 : 0);
    
endmodule

module WT_SB8_8x10_19(
        input logic [7:0] IN1,
        input logic [9:0] IN2,
        output logic [18:0] result0,
        output logic [18:0] result1);
    
    
// Creating Partial Products 

    wire logic const1;
    assign const1 = 1'b1;
    wire [7:0] mult = IN1;
    wire [9:0] mcand = IN2;
    wire [11:0] mcand_1x;
    wire [11:0] mcand_2x;
    wire [11:0] mcand_3x;
    wire [11:0] mcand_4x;
    assign mcand_1x = {{2{mcand[9]}},  mcand};
    assign mcand_2x = {{1{mcand[9]}},  mcand, 1'b0};
    assign mcand_3x = mcand_1x + mcand_2x;
    assign mcand_4x = {{0{mcand[9]}},  mcand, 2'b0};
    
    // Signed Booth Radix-8 Partial Products. Multiplier selectors: mult[2] mult[1] mult[0] 1'b0
    wire logic select_e_0, select_2x_0, select_3x_0, select_4x_0, tcomp0, select_ne_0, select_n2x_0, select_n3x_0, select_n4x_0;
    assign select_e_0 = ((~ mult[2]) & (~ mult[1]) & (mult[0] ^ 1'b0));
    assign select_ne_0 = mult[2] & mult[1] &  (mult[0] ^ 1'b0);
    assign select_2x_0 = (~ mult[2]) & (mult[1] ^ mult[0])& (mult[1] ^ 1'b0);
    assign select_n2x_0 = mult[2] & (mult[1] ^ mult[0])& (mult[1] ^ 1'b0);
    assign select_3x_0 = ((~ mult[2]) & mult[1] & (mult[0] ^ 1'b0));
    assign select_n3x_0 = (mult[2] & ~mult[1] & (mult[0] ^ 1'b0));
    assign select_4x_0 = ~mult[2] & mult[1] & mult[0] & 1'b0;
    assign select_n4x_0 = mult[2] & ~mult[1] & ~mult[0] & ~1'b0;
    wire [11:0] pp_0;
    assign pp_0 = (1<<11) ^ // flip the MSB 
                   ((select_e_0 ? mcand_1x : 0) | 
                    (select_2x_0 ? mcand_2x : 0) | 
                    (select_3x_0 ? mcand_3x : 0) | 
                    (select_4x_0 ? mcand_4x : 0) | 
                    (select_n4x_0 ? (~ mcand_4x) : 0) | 
                    (select_n3x_0 ? (~ mcand_3x) : 0) | 
                    (select_n2x_0 ? (~ mcand_2x) : 0) | 
                    (select_ne_0 ? (~ mcand_1x) : 0)); 
    assign tcomp0 = select_ne_0 | select_n4x_0 | select_n3x_0 | select_n2x_0;
    
    // Signed Booth Radix-8 Partial Products. Multiplier selectors: mult[5] mult[4] mult[3] mult[2]
    wire logic select_e_1, select_2x_1, select_3x_1, select_4x_1, tcomp1, select_ne_1, select_n2x_1, select_n3x_1, select_n4x_1;
    assign select_e_1 = ((~ mult[5]) & (~ mult[4]) & (mult[3] ^ mult[2]));
    assign select_ne_1 = mult[5] & mult[4] &  (mult[3] ^ mult[2]);
    assign select_2x_1 = (~ mult[5]) & (mult[4] ^ mult[3])& (mult[4] ^ mult[2]);
    assign select_n2x_1 = mult[5] & (mult[4] ^ mult[3])& (mult[4] ^ mult[2]);
    assign select_3x_1 = ((~ mult[5]) & mult[4] & (mult[3] ^ mult[2]));
    assign select_n3x_1 = (mult[5] & ~mult[4] & (mult[3] ^ mult[2]));
    assign select_4x_1 = ~mult[5] & mult[4] & mult[3] & mult[2];
    assign select_n4x_1 = mult[5] & ~mult[4] & ~mult[3] & ~mult[2];
    wire [11:0] pp_1;
    assign pp_1 = (1<<11) ^ // flip the MSB 
                   ((select_e_1 ? mcand_1x : 0) | 
                    (select_2x_1 ? mcand_2x : 0) | 
                    (select_3x_1 ? mcand_3x : 0) | 
                    (select_4x_1 ? mcand_4x : 0) | 
                    (select_n4x_1 ? (~ mcand_4x) : 0) | 
                    (select_n3x_1 ? (~ mcand_3x) : 0) | 
                    (select_n2x_1 ? (~ mcand_2x) : 0) | 
                    (select_ne_1 ? (~ mcand_1x) : 0)); 
    assign tcomp1 = select_ne_1 | select_n4x_1 | select_n3x_1 | select_n2x_1;
    
    // Signed Booth Radix-8 Partial Products. Multiplier selectors: mult[7] mult[7] mult[6] mult[5]
    wire logic select_e_2, select_2x_2, select_3x_2, select_4x_2, tcomp2, select_ne_2, select_n2x_2, select_n3x_2, select_n4x_2;
    assign select_e_2 = ((~ mult[7]) & (~ mult[7]) & (mult[6] ^ mult[5]));
    assign select_ne_2 = mult[7] & mult[7] &  (mult[6] ^ mult[5]);
    assign select_2x_2 = (~ mult[7]) & (mult[7] ^ mult[6])& (mult[7] ^ mult[5]);
    assign select_n2x_2 = mult[7] & (mult[7] ^ mult[6])& (mult[7] ^ mult[5]);
    assign select_3x_2 = ((~ mult[7]) & mult[7] & (mult[6] ^ mult[5]));
    assign select_n3x_2 = (mult[7] & ~mult[7] & (mult[6] ^ mult[5]));
    assign select_4x_2 = ~mult[7] & mult[7] & mult[6] & mult[5];
    assign select_n4x_2 = mult[7] & ~mult[7] & ~mult[6] & ~mult[5];
    wire [11:0] pp_2;
    assign pp_2 = (1<<11) ^ // flip the MSB 
                   ((select_e_2 ? mcand_1x : 0) | 
                    (select_2x_2 ? mcand_2x : 0) | 
                    (select_3x_2 ? mcand_3x : 0) | 
                    (select_4x_2 ? mcand_4x : 0) | 
                    (select_n4x_2 ? (~ mcand_4x) : 0) | 
                    (select_n3x_2 ? (~ mcand_3x) : 0) | 
                    (select_n2x_2 ? (~ mcand_2x) : 0) | 
                    (select_ne_2 ? (~ mcand_1x) : 0)); 
    assign tcomp2 = select_ne_2 | select_n4x_2 | select_n3x_2 | select_n2x_2;
    
    // The values to be summed in the summation tree, from LSB (left) to MSB:
     // pp_0[0]  pp_0[1]  pp_0[2]  pp_0[3]  pp_0[4]  pp_0[5]  pp_0[6]  pp_0[7]  pp_0[8]  pp_0[9]  pp_0[10] pp_0[11]   --       --       --       --       --       --     
     //   --       --       --     pp_1[0]  pp_1[1]  pp_1[2]  pp_1[3]  pp_1[4]  pp_1[5]  pp_1[6]  pp_1[7]  pp_1[8]  pp_1[9]  pp_1[10] pp_1[11]   --       --       --     
     //   --       --       --       --       --       --     pp_2[0]  pp_2[1]  pp_2[2]  pp_2[3]  pp_2[4]  pp_2[5]  pp_2[6]  pp_2[7]  pp_2[8]  pp_2[9]  pp_2[10] pp_2[11] 
     // tcomp0     --       --     tcomp1     --       --     tcomp2     --       --       --       --       --       --       --       --       --       --       --     
     //   --       --       --       --       --       --       --       --       --       --       --       --       --       --       --       --       --       --     
    
// Creating Summation Tree 

    
    // Wallace Summation Stage 1
    logic s0 ,c0;
    ha ha0 (pp_0[0], tcomp0, s0, c0);
    logic s1 ,c1; 
    fa fa1 (pp_0[3], pp_1[0], tcomp1, s1, c1);
    logic s2 ,c2;
    ha ha2 (pp_0[4], pp_1[1], s2, c2);
    logic s3 ,c3;
    ha ha3 (pp_0[5], pp_1[2], s3, c3);
    logic s4 ,c4; 
    fa fa4 (pp_0[6], pp_1[3], pp_2[0], s4, c4);
    logic s5 ,c5; 
    fa fa5 (pp_0[7], pp_1[4], pp_2[1], s5, c5);
    logic s6 ,c6; 
    fa fa6 (pp_0[8], pp_1[5], pp_2[2], s6, c6);
    logic s7 ,c7; 
    fa fa7 (pp_0[9], pp_1[6], pp_2[3], s7, c7);
    logic s8 ,c8; 
    fa fa8 (pp_0[10], pp_1[7], pp_2[4], s8, c8);
    logic s9 ,c9; 
    fa fa9 (pp_0[11], pp_1[8], pp_2[5], s9, c9);
    logic s10 ,c10;
    ha ha10 (pp_1[9], pp_2[6], s10, c10);
    logic s11 ,c11;
    ha ha11 (pp_1[10], pp_2[7], s11, c11);
    logic s12 ,c12;
    ha ha12 (pp_1[11], pp_2[8], s12, c12);
    
    // Wallace Summation Stage 2
    logic s13 ,c13;
    ha ha13 (pp_0[1], c0, s13, c13);
    logic s14 ,c14;
    ha ha14 (c1, s2, s14, c14);
    logic s15 ,c15;
    ha ha15 (c2, s3, s15, c15);
    logic s16 ,c16; 
    fa fa16 (tcomp2, c3, s4, s16, c16);
    logic s17 ,c17;
    ha ha17 (c4, s5, s17, c17);
    logic s18 ,c18;
    ha ha18 (c5, s6, s18, c18);
    logic s19 ,c19;
    ha ha19 (c6, s7, s19, c19);
    logic s20 ,c20;
    ha ha20 (c7, s8, s20, c20);
    logic s21 ,c21;
    ha ha21 (c8, s9, s21, c21);
    logic s22 ,c22;
    ha ha22 (c9, s10, s22, c22);
    logic s23 ,c23;
    ha ha23 (c10, s11, s23, c23);
    logic s24 ,c24;
    ha ha24 (c11, s12, s24, c24);
    logic s25 ,c25;
    ha ha25 (pp_2[9], c12, s25, c25);
    
    assign result0[0] = s0;
    assign result1[0] = 1'b0;
    assign result0[1] = s13;
    assign result1[1] = 1'b0;
    
    assign result0[17:2] = {pp_2[11], pp_2[10], c24, c23, c22, c21, c20, c19, c18, c17, c16, c15, c14, s14, s1, pp_0[2] };
    assign result1[17:2] = {1'b0, c25, s25, s24, s23, s22, s21, s20, s19, s18, s17, s16, s15, 1'b0, 1'b0, c13 };
    assign result0[18] = 1'b0;
    assign result1[18] = 1'b0;
endmodule



module MAC_WT_SB8_LF_8x10_plus_6(
        input logic [7:0] IN1,
        input logic [9:0] IN2,
        input logic [5:0] IN3,
        output logic [18:0] result);
    wire logic [18:0] m1_0;
    wire logic [18:0] m1_1;
    WT_SB8_8x10_19 m1 (IN1[7:0], IN2[9:0], m1_0, m1_1);
    
    
    // The values to be summed in the summation tree, from LSB (left) to MSB:
     // m1_0[0]  m1_0[1]  m1_0[2]  m1_0[3]  m1_0[4]  m1_0[5]  m1_0[6]  m1_0[7]  m1_0[8]  m1_0[9]  m1_0[10] m1_0[11] m1_0[12] m1_0[13] m1_0[14] m1_0[15] m1_0[16] m1_0[17]   --     
     //   --       --     m1_1[2]    --       --     m1_1[5]  m1_1[6]  m1_1[7]  m1_1[8]  m1_1[9]  m1_1[10] m1_1[11] m1_1[12] m1_1[13] m1_1[14] m1_1[15] m1_1[16]   --       --     
     // IN3[0]   IN3[1]   IN3[2]   IN3[3]   IN3[4]   ~IN3[5]    --       --       --       --       --       --       --       --       --       --       --       --       --     
     //   --       --       --       --       --     1'b1     1'b1     1'b1     1'b1     1'b1     1'b1       --     1'b1     1'b1       --     1'b1     1'b1       --     1'b1     
    
    // Wallace Summation Stage 1
    logic s0 ,c0;
    ha ha0 (m1_0[0], IN3[0], s0, c0);
    logic s1 ,c1;
    ha ha1 (m1_0[1], IN3[1], s1, c1);
    logic s2 ,c2; 
    fa fa2 (m1_0[2], m1_1[2], IN3[2], s2, c2);
    logic s3 ,c3;
    ha ha3 (m1_0[3], IN3[3], s3, c3);
    logic s4 ,c4;
    ha ha4 (m1_0[4], IN3[4], s4, c4);
    logic s5 ,c5; 
    fa fa5 (m1_0[5], m1_1[5], ~IN3[5], s5, c5);
    logic s6 ,c6; 
    fa fa6 (m1_0[6], m1_1[6], 1'b1, s6, c6);
    logic s7 ,c7; 
    fa fa7 (m1_0[7], m1_1[7], 1'b1, s7, c7);
    logic s8 ,c8; 
    fa fa8 (m1_0[8], m1_1[8], 1'b1, s8, c8);
    logic s9 ,c9; 
    fa fa9 (m1_0[9], m1_1[9], 1'b1, s9, c9);
    logic s10 ,c10; 
    fa fa10 (m1_0[10], m1_1[10], 1'b1, s10, c10);
    logic s11 ,c11;
    ha ha11 (m1_0[11], m1_1[11], s11, c11);
    logic s12 ,c12; 
    fa fa12 (m1_0[12], m1_1[12], 1'b1, s12, c12);
    logic s13 ,c13; 
    fa fa13 (m1_0[13], m1_1[13], 1'b1, s13, c13);
    logic s14 ,c14;
    ha ha14 (m1_0[14], m1_1[14], s14, c14);
    logic s15 ,c15; 
    fa fa15 (m1_0[15], m1_1[15], 1'b1, s15, c15);
    logic s16 ,c16; 
    fa fa16 (m1_0[16], m1_1[16], 1'b1, s16, c16);
    
    // Wallace Summation Stage 2
    logic s17 ,c17;
    ha ha17 (c0, s1, s17, c17);
    logic s18 ,c18;
    ha ha18 (c1, s2, s18, c18);
    logic s19 ,c19;
    ha ha19 (c2, s3, s19, c19);
    logic s20 ,c20;
    ha ha20 (c3, s4, s20, c20);
    logic s21 ,c21; 
    fa fa21 (1'b1, c4, s5, s21, c21);
    logic s22 ,c22;
    ha ha22 (c5, s6, s22, c22);
    logic s23 ,c23;
    ha ha23 (c6, s7, s23, c23);
    logic s24 ,c24;
    ha ha24 (c7, s8, s24, c24);
    logic s25 ,c25;
    ha ha25 (c8, s9, s25, c25);
    logic s26 ,c26;
    ha ha26 (c9, s10, s26, c26);
    logic s27 ,c27;
    ha ha27 (c10, s11, s27, c27);
    logic s28 ,c28;
    ha ha28 (c11, s12, s28, c28);
    logic s29 ,c29;
    ha ha29 (c12, s13, s29, c29);
    logic s30 ,c30;
    ha ha30 (c13, s14, s30, c30);
    logic s31 ,c31;
    ha ha31 (c14, s15, s31, c31);
    logic s32 ,c32;
    ha ha32 (c15, s16, s32, c32);
    logic s33 ,c33;
    ha ha33 (m1_0[17], c16, s33, c33);
    
    assign result[0] = s0;
    assign result[1] = s17;
    logic [17:0] adder_result;
    LF_17 final_adder ({1'b1, c32, c31, c30, c29, c28, c27, c26, c25, c24, c23, c22, c21, c20, c19, c18, c17 }, {c33, s33, s32, s31, s30, s29, s28, s27, s26, s25, s24, s23, s22, s21, s20, s19, s18 }, adder_result );
    assign result[18:2] = adder_result[16:0];

endmodule


module LF_17 ( 
        input logic [16:0] IN1,
        input logic [16:0] IN2,
        output logic [17:0] OUT);
    
    wire logic [16:0] p_0;
    wire logic [16:0] g_0;
    assign g_0 = IN1 & IN2;
    assign p_0 = IN1 ^ IN2;
    
    // LF stage 1
    wire logic p_1_1;
    wire logic g_1_1;
    assign p_1_1 = p_0[1] & p_0[0];
    assign g_1_1 = (p_0[1] & g_0[0]) | g_0[1];
    wire logic p_1_3;
    wire logic g_1_3;
    assign p_1_3 = p_0[3] & p_0[2];
    assign g_1_3 = (p_0[3] & g_0[2]) | g_0[3];
    wire logic p_1_5;
    wire logic g_1_5;
    assign p_1_5 = p_0[5] & p_0[4];
    assign g_1_5 = (p_0[5] & g_0[4]) | g_0[5];
    wire logic p_1_7;
    wire logic g_1_7;
    assign p_1_7 = p_0[7] & p_0[6];
    assign g_1_7 = (p_0[7] & g_0[6]) | g_0[7];
    wire logic p_1_9;
    wire logic g_1_9;
    assign p_1_9 = p_0[9] & p_0[8];
    assign g_1_9 = (p_0[9] & g_0[8]) | g_0[9];
    wire logic p_1_11;
    wire logic g_1_11;
    assign p_1_11 = p_0[11] & p_0[10];
    assign g_1_11 = (p_0[11] & g_0[10]) | g_0[11];
    wire logic p_1_13;
    wire logic g_1_13;
    assign p_1_13 = p_0[13] & p_0[12];
    assign g_1_13 = (p_0[13] & g_0[12]) | g_0[13];
    wire logic p_1_15;
    wire logic g_1_15;
    assign p_1_15 = p_0[15] & p_0[14];
    assign g_1_15 = (p_0[15] & g_0[14]) | g_0[15];
    
    // LF stage 2
    wire logic p_2_2;
    wire logic g_2_2;
    assign p_2_2 = p_0[2] & p_1_1;
    assign g_2_2 = (p_0[2] & g_1_1) | g_0[2];
    wire logic p_2_3;
    wire logic g_2_3;
    assign p_2_3 = p_1_3 & p_1_1;
    assign g_2_3 = (p_1_3 & g_1_1) | g_1_3;
    wire logic p_2_6;
    wire logic g_2_6;
    assign p_2_6 = p_0[6] & p_1_5;
    assign g_2_6 = (p_0[6] & g_1_5) | g_0[6];
    wire logic p_2_7;
    wire logic g_2_7;
    assign p_2_7 = p_1_7 & p_1_5;
    assign g_2_7 = (p_1_7 & g_1_5) | g_1_7;
    wire logic p_2_10;
    wire logic g_2_10;
    assign p_2_10 = p_0[10] & p_1_9;
    assign g_2_10 = (p_0[10] & g_1_9) | g_0[10];
    wire logic p_2_11;
    wire logic g_2_11;
    assign p_2_11 = p_1_11 & p_1_9;
    assign g_2_11 = (p_1_11 & g_1_9) | g_1_11;
    wire logic p_2_14;
    wire logic g_2_14;
    assign p_2_14 = p_0[14] & p_1_13;
    assign g_2_14 = (p_0[14] & g_1_13) | g_0[14];
    wire logic p_2_15;
    wire logic g_2_15;
    assign p_2_15 = p_1_15 & p_1_13;
    assign g_2_15 = (p_1_15 & g_1_13) | g_1_15;
    
    // LF stage 3
    wire logic p_3_4;
    wire logic g_3_4;
    assign p_3_4 = p_0[4] & p_2_3;
    assign g_3_4 = (p_0[4] & g_2_3) | g_0[4];
    wire logic p_3_5;
    wire logic g_3_5;
    assign p_3_5 = p_1_5 & p_2_3;
    assign g_3_5 = (p_1_5 & g_2_3) | g_1_5;
    wire logic p_3_6;
    wire logic g_3_6;
    assign p_3_6 = p_2_6 & p_2_3;
    assign g_3_6 = (p_2_6 & g_2_3) | g_2_6;
    wire logic p_3_7;
    wire logic g_3_7;
    assign p_3_7 = p_2_7 & p_2_3;
    assign g_3_7 = (p_2_7 & g_2_3) | g_2_7;
    wire logic p_3_12;
    wire logic g_3_12;
    assign p_3_12 = p_0[12] & p_2_11;
    assign g_3_12 = (p_0[12] & g_2_11) | g_0[12];
    wire logic p_3_13;
    wire logic g_3_13;
    assign p_3_13 = p_1_13 & p_2_11;
    assign g_3_13 = (p_1_13 & g_2_11) | g_1_13;
    wire logic p_3_14;
    wire logic g_3_14;
    assign p_3_14 = p_2_14 & p_2_11;
    assign g_3_14 = (p_2_14 & g_2_11) | g_2_14;
    wire logic p_3_15;
    wire logic g_3_15;
    assign p_3_15 = p_2_15 & p_2_11;
    assign g_3_15 = (p_2_15 & g_2_11) | g_2_15;
    
    // LF stage 4
    wire logic p_4_8;
    wire logic g_4_8;
    assign p_4_8 = p_0[8] & p_3_7;
    assign g_4_8 = (p_0[8] & g_3_7) | g_0[8];
    wire logic p_4_9;
    wire logic g_4_9;
    assign p_4_9 = p_1_9 & p_3_7;
    assign g_4_9 = (p_1_9 & g_3_7) | g_1_9;
    wire logic p_4_10;
    wire logic g_4_10;
    assign p_4_10 = p_2_10 & p_3_7;
    assign g_4_10 = (p_2_10 & g_3_7) | g_2_10;
    wire logic p_4_11;
    wire logic g_4_11;
    assign p_4_11 = p_2_11 & p_3_7;
    assign g_4_11 = (p_2_11 & g_3_7) | g_2_11;
    wire logic p_4_12;
    wire logic g_4_12;
    assign p_4_12 = p_3_12 & p_3_7;
    assign g_4_12 = (p_3_12 & g_3_7) | g_3_12;
    wire logic p_4_13;
    wire logic g_4_13;
    assign p_4_13 = p_3_13 & p_3_7;
    assign g_4_13 = (p_3_13 & g_3_7) | g_3_13;
    wire logic p_4_14;
    wire logic g_4_14;
    assign p_4_14 = p_3_14 & p_3_7;
    assign g_4_14 = (p_3_14 & g_3_7) | g_3_14;
    wire logic p_4_15;
    wire logic g_4_15;
    assign p_4_15 = p_3_15 & p_3_7;
    assign g_4_15 = (p_3_15 & g_3_7) | g_3_15;
    
    // LF stage 5
    wire logic p_5_16;
    wire logic g_5_16;
    assign p_5_16 = p_0[16] & p_4_15;
    assign g_5_16 = (p_0[16] & g_4_15) | g_0[16];
    
    // LF postprocess 
    assign OUT[0] = p_0[0];
    assign OUT[1] = p_0[1] ^ g_0[0];
    assign OUT[2] = p_0[2] ^ g_1_1;
    assign OUT[3] = p_0[3] ^ g_2_2;
    assign OUT[4] = p_0[4] ^ g_2_3;
    assign OUT[5] = p_0[5] ^ g_3_4;
    assign OUT[6] = p_0[6] ^ g_3_5;
    assign OUT[7] = p_0[7] ^ g_3_6;
    assign OUT[8] = p_0[8] ^ g_3_7;
    assign OUT[9] = p_0[9] ^ g_4_8;
    assign OUT[10] = p_0[10] ^ g_4_9;
    assign OUT[11] = p_0[11] ^ g_4_10;
    assign OUT[12] = p_0[12] ^ g_4_11;
    assign OUT[13] = p_0[13] ^ g_4_12;
    assign OUT[14] = p_0[14] ^ g_4_13;
    assign OUT[15] = p_0[15] ^ g_4_14;
    assign OUT[16] = p_0[16] ^ g_4_15;
    assign OUT[17] = g_5_16;
endmodule

module LF_17_spec (
        input logic [16:0] IN1,
        input logic [16:0] IN2,
        output logic adder_correct,
        output logic [17:0] spec_res);
    
    assign spec_res = IN1 + IN2;
    wire [17:0] adder_res;
    LF_17 adder(IN1, IN2, adder_res);
    assign adder_correct = ((spec_res == adder_res) ? 1 : 0);
    
endmodule



module ha (
        input logic a,
        input logic b,
        output logic s,
        output logic c);
    
    assign s = a ^ b;
    assign c = a & b;
endmodule



module fa (
        input logic x,
        input logic y,
        input logic z,
        output logic s,
        output logic c);
    
    assign s = x ^ y ^ z;
    assign c = (x & y) | (x & z) | (y & z);
endmodule


