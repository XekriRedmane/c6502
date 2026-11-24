SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__result,__local_main__my_struct_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run4 section=.text local=true zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   BNE   .jcmp_skip@0
   LDA   #$01
   CMP   #$01
   BEQ   .case@1
.jcmp_skip@0:
   LDA   #$00
   BEQ   .lb_skip@0
   JMP   .default@3
.lb_skip@0:
   LDA   #$01
   CMP   #$02
   BEQ   .case@2
   JMP   .default@3
.case@1:
   JSR   main@run1
   STA   HARGS+5
   LDA   #$34
   JSR   main@run2
   LDA   #$3E
   JSR   main@run3
   RTS
.case@2:
   JSR   main@run1
   STA   HARGS+5
   LDA   #$26
   JSR   main@run2
   LDA   #$28
   JSR   main@run3
   RTS
.default@3:
   JSR   main@run1
   STY   HARGS+5
   STY   HARGS+6
   STY   HARGS+7
   STY   HARGS+8
   JSR   main@run4
   STY   HARGS+13
   STY   HARGS+14
   JSR   main@run0
   RTS

main@run4:
   SUBROUTINE

   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   RTS

main@run3:
   SUBROUTINE

   STA   HARGS+14
   LDA   #$40
   JMP   main@run0

main@run2:
   SUBROUTINE

   STA   HARGS+6
   LDA   #$40
   STA   HARGS+7
   TYA
   STA   HARGS+8
   JSR   main@run4
   STA   HARGS+13
   RTS

main@run1:
   SUBROUTINE

   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   RTS

main@run0:
   SUBROUTINE

   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   STA   HARGS
   LDA   HARGS+17
   STA   HARGS+1
   LDA   HARGS+18
   STA   HARGS+2
   LDA   HARGS+19
   STA   HARGS+3
   LDA   HARGS+20
   STA   HARGS+4
   LDA   HARGS+21
   STA   HARGS+5
   LDA   HARGS+22
   STA   HARGS+6
   LDA   HARGS+23
   STA   HARGS+7
   JSR   d2i
   LDA   HARGS+8
   LDX   HARGS+9
   RTS
