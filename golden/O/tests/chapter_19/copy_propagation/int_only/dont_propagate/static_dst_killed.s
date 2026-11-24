SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params= locals= indirect=false in_cycle=false
; def update_x params= locals= indirect=false in_cycle=false
; call main -> target
; call target -> update_x
; item update_x section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item x section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

update_x:
   SUBROUTINE

   LDA   #$04
   STA   x
   LDA   #$00
   STA   x+1
   LDX   #$00
   RTS

target:
   SUBROUTINE

   LDA   #$03
   STA   x
   LDA   #$00
   STA   x+1
   JSR   update_x
   LDX   x+1
   LDA   x
   RTS

main:
   SUBROUTINE

   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$04
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   x+1
   BNE   .jcmp_skip@1
   LDA   x
   CMP   #$04
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

x:
   DS.B  2
