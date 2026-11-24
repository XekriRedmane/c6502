SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_multiply_by_large_num__d_0	EQU	$80
__zpabi_non_zero__d_0	EQU	$80
__zpabi_multiply_by_large_num__d_1	EQU	$81
__zpabi_non_zero__d_1	EQU	$81
__zpabi_multiply_by_large_num__d_2	EQU	$82
__zpabi_non_zero__d_2	EQU	$82
__zpabi_multiply_by_large_num__d_3	EQU	$83
__zpabi_non_zero__d_3	EQU	$83
__zpabi_multiply_by_large_num__d_4	EQU	$84
__zpabi_non_zero__d_4	EQU	$84
__zpabi_multiply_by_large_num__d_5	EQU	$85
__zpabi_non_zero__d_5	EQU	$85
__zpabi_multiply_by_large_num__d_6	EQU	$86
__zpabi_non_zero__d_6	EQU	$86
__zpabi_multiply_by_large_num__d_7	EQU	$87
__zpabi_non_zero__d_7	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def multiply_by_large_num params=__zpabi_multiply_by_large_num__d_0,__zpabi_multiply_by_large_num__d_1,__zpabi_multiply_by_large_num__d_2,__zpabi_multiply_by_large_num__d_3,__zpabi_multiply_by_large_num__d_4,__zpabi_multiply_by_large_num__d_5,__zpabi_multiply_by_large_num__d_6,__zpabi_multiply_by_large_num__d_7 locals= indirect=false in_cycle=false
; def non_zero params=__zpabi_non_zero__d_0,__zpabi_non_zero__d_1,__zpabi_non_zero__d_2,__zpabi_non_zero__d_3,__zpabi_non_zero__d_4,__zpabi_non_zero__d_5,__zpabi_non_zero__d_6,__zpabi_non_zero__d_7 locals=__local_non_zero__0 indirect=false in_cycle=false
; call main -> multiply_by_large_num
; call main -> non_zero
; item non_zero section=.text local=false zero=false
; item multiply_by_large_num section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

non_zero:
   SUBROUTINE

   LDA   __zpabi_non_zero__d_0
   ORA   __zpabi_non_zero__d_1
   ORA   __zpabi_non_zero__d_2
   ORA   __zpabi_non_zero__d_3
   ORA   __zpabi_non_zero__d_4
   ORA   __zpabi_non_zero__d_5
   ORA   __zpabi_non_zero__d_6
   ORA   __zpabi_non_zero__d_7
   BEQ   .lnot_true@0
   LDA   #$00
   BEQ   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   LDX   #$00
   RTS

multiply_by_large_num:
   SUBROUTINE

   LDA   __zpabi_multiply_by_large_num__d_0
   STA   HARGS
   LDA   __zpabi_multiply_by_large_num__d_1
   STA   HARGS+1
   LDA   __zpabi_multiply_by_large_num__d_2
   STA   HARGS+2
   LDA   __zpabi_multiply_by_large_num__d_3
   STA   HARGS+3
   LDA   __zpabi_multiply_by_large_num__d_4
   STA   HARGS+4
   LDA   __zpabi_multiply_by_large_num__d_5
   STA   HARGS+5
   LDA   __zpabi_multiply_by_large_num__d_6
   STA   HARGS+6
   LDA   __zpabi_multiply_by_large_num__d_7
   STA   HARGS+7
   LDA   #$40
   STA   HARGS+8
   LDA   #$8C
   STA   HARGS+9
   LDA   #$B5
   STA   HARGS+10
   LDA   #$78
   STA   HARGS+11
   LDA   #$1D
   STA   HARGS+12
   LDA   #$AF
   STA   HARGS+13
   LDA   #$25
   STA   HARGS+14
   LDA   #$44
   STA   HARGS+15
   JMP   dmul

main:
   SUBROUTINE

   LDA   #$C4
   STA   __zpabi_multiply_by_large_num__d_0
   LDA   #$13
   STA   __zpabi_multiply_by_large_num__d_1
   LDA   #$00
   STA   __zpabi_multiply_by_large_num__d_2
   STA   __zpabi_multiply_by_large_num__d_3
   STA   __zpabi_multiply_by_large_num__d_4
   STA   __zpabi_multiply_by_large_num__d_5
   STA   __zpabi_multiply_by_large_num__d_6
   STA   __zpabi_multiply_by_large_num__d_7
   JSR   multiply_by_large_num
   LDA   HARGS+23
   CMP   #$01
   BNE   .jcmp_skip@2
   LDA   HARGS+22
   CMP   #$CA
   BNE   .jcmp_skip@2
   LDA   HARGS+21
   CMP   #$C9
   BNE   .jcmp_skip@2
   LDA   HARGS+20
   CMP   #$94
   BNE   .jcmp_skip@2
   LDA   HARGS+19
   CMP   #$28
   BNE   .jcmp_skip@2
   LDA   HARGS+18
   CMP   #$5E
   BNE   .jcmp_skip@2
   LDA   HARGS+17
   CMP   #$46
   BNE   .jcmp_skip@2
   LDA   HARGS+16
   CMP   #$82
   BEQ   .if_end@0
.jcmp_skip@2:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@0:
   LDA   #$C4
   STA   __zpabi_non_zero__d_0
   LDA   #$13
   STA   __zpabi_non_zero__d_1
   LDA   #$00
   STA   __zpabi_non_zero__d_2
   STA   __zpabi_non_zero__d_3
   STA   __zpabi_non_zero__d_4
   STA   __zpabi_non_zero__d_5
   STA   __zpabi_non_zero__d_6
   STA   __zpabi_non_zero__d_7
   JMP   non_zero
