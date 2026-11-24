SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__x_23,__local_main__x_22,__local_main__x_21,__local_main__x_20,__local_main__x_19,__local_main__x_18,__local_main__x_17,__local_main__x_16,__local_main__x_15,__local_main__x_14,__local_main__x_13,__local_main__x_12,__local_main__x_11,__local_main__x_10,__local_main__x_9,__local_main__x_8,__local_main__x_7,__local_main__x_6,__local_main__x_5,__local_main__x_4,__local_main__x_3,__local_main__x_2,__local_main__x_1,__local_main__x_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   LDY   #$00
   CMP   #$00
   BNE   .jcmp_skip@0
   LDA   #$01
   CMP   #$01
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   TYA
   BNE   .jcmp_skip@1
   LDA   #$02
   CMP   #$02
   BEQ   .vthread@0
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.vthread@0:
   TYA
   BNE   .jcmp_skip@2
   BEQ   .if_end@4
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@4:
   LDX   #$00
   RTS
