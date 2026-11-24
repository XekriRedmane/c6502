SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__x	EQU	$82
__local_update_value__0	EQU	$84
__local_update_value__ptr	EQU	$85
__local_update_value__1	EQU	$86

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__x,__local_main__3 indirect=false in_cycle=false
; def update_value params=__zpabi_update_value__ptr_0,__zpabi_update_value__ptr_1 locals=__local_update_value__0,__local_update_value__ptr,__local_update_value__1 indirect=false in_cycle=false param_regs=A,X
; call main -> update_value
; item update_value section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

update_value:
   SUBROUTINE

   STA   __local_update_value__ptr
   STX   __local_update_value__1
   LDY   #$00
   LDA   (__local_update_value__ptr),Y
   STA   __local_update_value__0
   INY
   LDA   (__local_update_value__ptr),Y
   TAX
   LDA   #$0A
   DEY
   STA   (__local_update_value__ptr),Y
   LDA   #$00
   INY
   STA   (__local_update_value__ptr),Y
   LDA   __local_update_value__0
   RTS

main:
   SUBROUTINE

   LDA   #$14
   STA   __local_main__x
   LDA   #$00
   STA   __local_main__x+1
   LDX   #>__local_main__x
   LDA   #<__local_main__x
   JSR   update_value
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$14
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_main__x+1
   BNE   .jcmp_skip@1
   LDA   __local_main__x
   CMP   #$0A
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS
