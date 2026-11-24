SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def check_null_ptr_cast params= locals=__local_check_null_ptr_cast__ptr_ptr_1,__local_check_null_ptr_cast__ptr_ptr_0,__local_check_null_ptr_cast__int_ptr_1,__local_check_null_ptr_cast__int_ptr_0,__local_check_null_ptr_cast__dbl_ptr_1,__local_check_null_ptr_cast__dbl_ptr_0 indirect=false in_cycle=false
; def check_round_trip params= locals=__local_check_round_trip__0,__local_check_round_trip__1,__local_check_round_trip__2,__local_check_round_trip__3,__local_check_round_trip__4,__local_check_round_trip__5,__local_check_round_trip__l,__local_check_round_trip__7,__local_check_round_trip__8,__local_check_round_trip__9 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call main -> check_null_ptr_cast
; call main -> check_round_trip
; item check_null_ptr_cast section=.text local=false zero=false
; item check_null_ptr_cast@run0 section=.text local=true zero=false
; item check_round_trip section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @0.long_ptr section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

check_null_ptr_cast:
   SUBROUTINE

   JSR   check_null_ptr_cast@run0
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   check_null_ptr_cast@run0
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   check_null_ptr_cast@run0
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   check_null_ptr_cast@run0
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

check_null_ptr_cast@run0:
   SUBROUTINE

   LDA   @0.long_ptr
   ORA   @0.long_ptr+1
   RTS

check_round_trip:
   SUBROUTINE

   LDA   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@0
   BEQ   .if_end@4
.jcmp_skip@0:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   JSR   check_null_ptr_cast
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@5
   LDA   __local_main__1
   RTS
.if_end@5:
   JMP   check_round_trip

@0.long_ptr:
   DS.B  2
