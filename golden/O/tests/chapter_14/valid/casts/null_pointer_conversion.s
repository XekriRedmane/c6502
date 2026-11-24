SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_expect_null_param__val_0	EQU	$83

; @zp-link-meta-begin
; def expect_null_param params=__zpabi_expect_null_param__val_0,__zpabi_expect_null_param__val_1 locals=__local_expect_null_param__val_1,__local_expect_null_param__val_0 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def return_null_ptr params= locals= indirect=false in_cycle=false
; call main -> expect_null_param
; call main -> return_null_ptr
; item expect_null_param section=.text local=false zero=false
; item return_null_ptr section=.text local=false zero=false
; item main section=.text local=false zero=false
; item d section=.bss local=false zero=true
; item i section=.bss local=false zero=true
; item i2 section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

expect_null_param:
   SUBROUTINE

   STA   __local_expect_null_param__val_0
   TXA
   BNE   .thread@0
   LDA   __local_expect_null_param__val_0
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

return_null_ptr:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   d
   ORA   d+1
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   i
   ORA   i+1
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   i2
   ORA   i2+1
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$00
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   JSR   expect_null_param
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   JSR   return_null_ptr
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDX   #$00
   TXA
   RTS

d:
   DS.B  2

i:
   DS.B  2

i2:
   DS.B  2
