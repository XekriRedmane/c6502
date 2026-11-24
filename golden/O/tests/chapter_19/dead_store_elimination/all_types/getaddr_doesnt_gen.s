SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_target__x	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals=__local_target__0,__local_target__1,__local_target__x,__local_target__3 indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   #>__local_target__x
   BNE   .thread@0
   LDA   #<__local_target__x
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   JMP   target
