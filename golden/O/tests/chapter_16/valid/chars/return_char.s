SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$83
__local_main__1	EQU	$87
__local_main__2	EQU	$8B

; @zp-link-meta-begin
; def main params= locals=__local_main__char_array_0,__local_main__char_array4_1,__local_main__char_array4_0,__local_main__0,__local_main__char_array3_2,__local_main__char_array3_1,__local_main__char_array3_0,__local_main__1,__local_main__char_array2_2,__local_main__char_array2_1,__local_main__char_array2_0,__local_main__2,__local_main__char_array_2,__local_main__char_array_1,__local_main__3 indirect=false in_cycle=false
; def return_char params= locals= indirect=false in_cycle=false
; def return_schar params= locals= indirect=false in_cycle=false
; def return_uchar params= locals= indirect=false in_cycle=false
; call main -> return_char
; call main -> return_schar
; call main -> return_uchar
; item return_char section=.text local=false zero=false
; item return_schar section=.text local=false zero=false
; item return_uchar section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

return_char:
   SUBROUTINE

   LDA   #$F6
   RTS

return_schar:
   SUBROUTINE

   LDA   #$F6
   RTS

return_uchar:
   SUBROUTINE

   LDA   #$F6
   RTS

main:
   SUBROUTINE

   JSR   return_char
   STA   __local_main__2
   JSR   return_schar
   STA   __local_main__1
   JSR   return_uchar
   STA   __local_main__0
   LDA   #$79
   CMP   #$79
   BNE   .main@jthread@1
   LDA   #$86
   BMI   .if_end@4
.main@jthread@1:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@4:
   LDA   __local_main__2
   BPL   .jcmp_skip@6
   CMP   #$F6
   BEQ   .if_end@10
.jcmp_skip@6:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@10:
   LDA   __local_main__1
   BPL   .jcmp_skip@13
   CMP   #$F6
   BEQ   .if_end@16
.jcmp_skip@13:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@16:
   LDA   __local_main__0
   CMP   #$F6
   BEQ   .if_end@20
   LDX   #$00
   LDA   #$06
   RTS
.if_end@20:
   LDX   #$00
   TXA
   RTS
