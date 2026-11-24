SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def foo params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call main -> foo
; item foo section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

foo:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

main:
   SUBROUTINE

   JSR   foo
   STA   __local_main__1
   TXA
   BNE   .thread@0
   LDA   __local_main__1
   CMP   #$01
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   EOR   #$01
   LDX   #$00
   RTS
