SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target params= locals=__local_target__loc_15,__local_target__loc_14,__local_target__loc_13,__local_target__loc_12,__local_target__loc_11,__local_target__loc_10,__local_target__loc_9,__local_target__loc_8,__local_target__loc_7,__local_target__loc_6,__local_target__loc_5,__local_target__loc_4,__local_target__loc_3,__local_target__loc_2,__local_target__loc_1,__local_target__loc_0 indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item glob section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   #$00
   LDY   #$64
   STY   glob
   LDY   #$00
   STY   glob+1
   STY   glob+2
   STY   glob+3
   STY   glob+4
   STY   glob+5
   STY   glob+6
   STA   glob+7
   LDY   #$C8
   STY   glob+8
   LDY   #$00
   STY   glob+9
   STY   glob+10
   STY   glob+11
   STY   glob+12
   STY   glob+13
   STY   glob+14
   STA   glob+15
   LDA   glob+8
   STA   HARGS+16
   TYA
   STA   HARGS+17
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   JSR   target
   LDA   HARGS+23
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   BNE   .jcmp_skip@0
   LDA   HARGS+21
   BNE   .jcmp_skip@0
   LDA   HARGS+20
   BNE   .jcmp_skip@0
   LDA   HARGS+19
   BNE   .jcmp_skip@0
   LDA   HARGS+18
   BNE   .jcmp_skip@0
   LDA   HARGS+17
   BNE   .jcmp_skip@0
   LDA   HARGS+16
   CMP   #$C8
   BEQ   .if_end@0
.jcmp_skip@0:
   LDA   #$01
   JSR   main@run0
   RTS
.if_end@0:
   LDA   glob+7
   BNE   .jcmp_skip@1
   LDA   glob+6
   BNE   .jcmp_skip@1
   LDA   glob+5
   BNE   .jcmp_skip@1
   LDA   glob+4
   BNE   .jcmp_skip@1
   LDA   glob+3
   BNE   .jcmp_skip@1
   LDA   glob+2
   BNE   .jcmp_skip@1
   LDA   glob+1
   BNE   .jcmp_skip@1
   LDA   glob
   CMP   #$64
   BEQ   .if_end@1
.jcmp_skip@1:
   LDA   #$02
   JSR   main@run0
   RTS
.if_end@1:
   LDA   glob+15
   BNE   .jcmp_skip@2
   LDA   glob+14
   BNE   .jcmp_skip@2
   LDA   glob+13
   BNE   .jcmp_skip@2
   LDA   glob+12
   BNE   .jcmp_skip@2
   LDA   glob+11
   BNE   .jcmp_skip@2
   LDA   glob+10
   BNE   .jcmp_skip@2
   LDA   glob+9
   BNE   .jcmp_skip@2
   LDA   glob+8
   CMP   #$C8
   BEQ   .if_end@2
.jcmp_skip@2:
   LDA   #$03
   JSR   main@run0
   RTS
.if_end@2:
   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   JSR   main@run1
   RTS

main@run1:
   SUBROUTINE

   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

main@run0:
   SUBROUTINE

   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   JSR   main@run1
   RTS

glob:
   DS.B  16
