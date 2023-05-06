
; Multiplier verification script for ACL2

; Note: The license below is based on the template at:
; http://opensource.org/licenses/BSD-3-Clause

; Copyright (C) 2020 Regents of the University of Texas
; All rights reserved.

; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are
; met:

; o Redistributions of source code must retain the above copyright
;   notice, this list of conditions and the following disclaimer.

; o Redistributions in binary form must reproduce the above copyright
;   notice, this list of conditions and the following disclaimer in the
;   documentation and/or other materials provided with the distribution.

; o Neither the name of the copyright holders nor the names of its
;   contributors may be used to endorse or promote products derived
;   from this software without specific prior written permission.

; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
; "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
; LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
; A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
; HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
; SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
; LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
; DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
; THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
; OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

; Original Author(s):
; Mertcan Temel         <mert@utexas.edu>

(in-package "RP")

(include-book "projects/rp-rewriter/lib/mult3/top" :dir :system)

(defsection example1
  (parse-and-create-svtv :file "DT_UB16_KS_16x16_multgen.sv"
                         :topmodule "DT_UB16_KS_16x16_spec")
  (verify-svtv-of-mult :name DT_UB16_KS_16x16_spec
                       :concl (equal design_is_correct
                                     1)))

(defsection example2
  (parse-and-create-svtv :file "DOT_Product_DT_UB4_LF_4_8x8_plus_10_9to0_multgen.sv"
                         :topmodule "DOT_Product_DT_UB4_LF_4_8x8_plus_10_9to0_spec")
  (verify-svtv-of-mult :name DOT_Product_DT_UB4_LF_4_8x8_plus_10_9to0_spec
                       :concl (equal design_is_correct
                                     1)))

(defsection example3
  (parse-and-create-svtv :file "DT_SB4_HC_8x8_11to0_multgen.sv"
                         :topmodule "DT_SB4_HC_8x8_11to0_spec")
  (verify-svtv-of-mult :name DT_SB4_HC_8x8_11to0_spec
                       :concl (equal design_is_correct
                                     1)))

(defsection example4
  (parse-and-create-svtv :file "MAC_WT_SB8_LF_8x10_plus_6_18to0_multgen.sv"
                         :topmodule "MAC_WT_SB8_LF_8x10_plus_6_18to0_spec")
  (verify-svtv-of-mult :name MAC_WT_SB8_LF_8x10_plus_6_18to0_spec
                       :concl (equal design_is_correct
                                     1)))

(defsection example5
  (parse-and-create-svtv :file "Merged_DT_SSP_RP_16x16_multgen.sv"
                         :topmodule "Merged_DT_SSP_RP_16x16_spec")
  (verify-svtv-of-mult :name Merged_DT_SSP_RP_16x16_spec
                       :concl (equal design_is_correct
                                     1)))

(defsection example6
  (parse-and-create-svtv :file "Merged_DT_SB8_JSkCond_12x12_15to8_multgen.sv"
                         :topmodule "Merged_DT_SB8_JSkCond_12x12_15to8_spec")
  (verify-svtv-of-mult :name Merged_DT_SB8_JSkCond_12x12_15to8_spec
                       :concl (equal design_is_correct
                                     1)))

(defsection example7
  (parse-and-create-svtv :file "MAC_c42_SB4_JSkCond_10x15_plus_15_19to10_multgen.sv"
                         :topmodule "MAC_c42_SB4_JSkCond_10x15_plus_15_19to10_spec")
  (verify-svtv-of-mult :name MAC_c42_SB4_JSkCond_10x15_plus_15_19to10_spec
                       :concl (equal design_is_correct
                                     1)))
