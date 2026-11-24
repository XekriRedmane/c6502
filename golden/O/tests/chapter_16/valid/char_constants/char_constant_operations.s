SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__array_39,__local_main__array_38,__local_main__array_37,__local_main__array_36,__local_main__array_35,__local_main__array_34,__local_main__array_33,__local_main__array_32,__local_main__array_31,__local_main__array_30,__local_main__array_29,__local_main__array_28,__local_main__array_15,__local_main__array_14,__local_main__array_13,__local_main__array_12,__local_main__array_11,__local_main__array_10,__local_main__array_9,__local_main__array_8 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item d section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   d+7
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   d+6
   CMP   #$57
   BNE   .jcmp_skip@0
   LDA   d+5
   BNE   .jcmp_skip@0
   LDA   d+4
   BNE   .jcmp_skip@0
   LDA   d+3
   BNE   .jcmp_skip@0
   LDA   d+2
   BNE   .jcmp_skip@0
   LDA   d+1
   BNE   .jcmp_skip@0
   LDA   d
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDY   #$00
   BNE   .jcmp_skip@1
   TYA
   BNE   .jcmp_skip@1
   LDA   #$61
   CMP   #$61
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   TYA
   BNE   .jcmp_skip@2
   LDA   #$08
   CMP   #$08
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   TYA
   BNE   .jcmp_skip@3
   LDA   #$21
   CMP   #$21
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   TYA
   BNE   .jcmp_skip@4
   LDA   #$25
   CMP   #$25
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   TYA
   BNE   .jcmp_skip@5
   LDA   #$7E
   CMP   #$7E
   BEQ   .if_end@6
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@6:
   TYA
   BNE   .jcmp_skip@6
   LDA   #$21
   CMP   #$21
   BEQ   .if_end@7
.jcmp_skip@6:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS

d:
   DC.L  $00000000
   DC.L  $40570000
