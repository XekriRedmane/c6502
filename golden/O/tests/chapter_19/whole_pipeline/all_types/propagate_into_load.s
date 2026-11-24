SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_target__0	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params= locals=__local_target__0 indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item glob section=.bss local=false zero=true
; item i section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   #<i
   STA   glob
   LDY   #>i
   STY   glob+1
   STA   DPTR
   STY   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_target__0
   INY
   LDA   (DPTR),Y
   TAX
   LDA   __local_target__0
   RTS

main:
   SUBROUTINE

   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   glob
   STA   DPTR
   LDA   glob+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_main__1
   INY
   LDA   (DPTR),Y
   BNE   .jcmp_skip@1
   LDA   __local_main__1
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

glob:
   DS.B  2

i:
   DC.W  $000A
