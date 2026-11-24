SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
x	EQU	$26
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def callee params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params= locals= indirect=false in_cycle=false
; call main -> target
; call target -> callee
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item target@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item y section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp x size=2 at=auto def=true init=$00,$00 weight=6
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   LDA   x
   ASL
   STA   y
   LDA   x+1
   ROL
   STA   y+1
   LDX   #$00
   LDA   #$05
   RTS

target:
   SUBROUTINE

   JSR   target@run0
   JSR   callee
   JSR   target@run0
   LDX   #$00
   LDA   #$02
   RTS

target@run0:
   SUBROUTINE

   LDA   #$02
   STA   x
   LDA   #$00
   STA   x+1
   RTS

main:
   SUBROUTINE

   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$02
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   y+1
   BNE   .jcmp_skip@1
   LDA   y
   CMP   #$04
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   x+1
   BNE   .jcmp_skip@2
   LDA   x
   CMP   #$02
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

y:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   x
   STA   x+1
   RTS
