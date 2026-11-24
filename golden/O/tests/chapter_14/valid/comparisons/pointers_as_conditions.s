SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i	EQU	$80
__local_main__0	EQU	$81
__local_main__1	EQU	$82
__local_main__ptr_0	EQU	$83
__local_main__ptr_1	EQU	$84
__local_main__x	EQU	$85

; @zp-link-meta-begin
; def get_null_pointer params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__i,__local_main__0,__local_main__1,__local_main__ptr_0,__local_main__ptr_1,__local_main__x,__local_main__3,__local_main__4,__local_main__5 indirect=false in_cycle=false
; call main -> get_null_pointer
; item get_null_pointer section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

get_null_pointer:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #<__local_main__x
   STA   __local_main__ptr_0
   LDA   #>__local_main__x
   STA   __local_main__ptr_1
   JSR   get_null_pointer
   STA   __local_main__1
   STX   __local_main__i
   ORA   __local_main__i
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDA   #<__local_main__x
   ORA   #>__local_main__x
   BNE   .if_end@6
   LDX   #$00
   LDA   #$03
   RTS
.if_end@6:
   LDA   #<__local_main__x
   ORA   #>__local_main__x
   BNE   .if_end@7
   LDX   #$00
   LDA   #$04
   RTS
.if_end@7:
   LDA   #<__local_main__x
   ORA   #>__local_main__x
   BEQ   .cond_else@8
   LDA   #$01
   STA   __local_main__0
   JMP   .cond_end@9
.cond_else@8:
   LDA   #$02
   STA   __local_main__0
.cond_end@9:
   LDA   __local_main__1
   ORA   __local_main__i
   BEQ   .cond_else@10
   LDA   #$03
   STA   __local_main__i
   JMP   .cond_end@11
.cond_else@10:
   LDA   #$04
   STA   __local_main__i
.cond_end@11:
   LDA   __local_main__0
   CMP   #$01
   BEQ   .if_end@12
   LDX   #$00
   LDA   #$05
   RTS
.if_end@12:
   LDA   __local_main__i
   CMP   #$04
   BEQ   .if_end@13
   LDX   #$00
   LDA   #$06
   RTS
.if_end@13:
   LDA   #$00
   STA   __local_main__i
   LDX   #$00
   BEQ   .loop@0_continue
.main@rot@0:
   LDA   __local_main__i
   SEC
   SBC   #$0A
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .if_end@14
   LDA   #$00
   STA   __local_main__ptr_0
   STA   __local_main__ptr_1
   JMP   .loop@0_continue
.if_end@14:
   INC   __local_main__i
   BNE   .loop@0_continue
   INX
.loop@0_continue:
   LDA   __local_main__ptr_0
   ORA   __local_main__ptr_1
   BNE   .main@rot@0
   CPX   #$00
   BNE   .jcmp_skip@1
   LDA   __local_main__i
   CMP   #$0A
   BEQ   .if_end@15
.jcmp_skip@1:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@15:
   LDX   #$00
   TXA
   RTS
