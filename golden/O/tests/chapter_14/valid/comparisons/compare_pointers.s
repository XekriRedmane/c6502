SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__a	EQU	$86
__local_main__b	EQU	$88

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__a,__local_main__7,__local_main__b,__local_main__9 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #>__local_main__a
   CMP   #>__local_main__b
   BNE   .if_end@0
   LDA   #<__local_main__a
   CMP   #<__local_main__b
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #>__local_main__a
   CMP   #>__local_main__a
   BNE   .jcmp_skip@0
   LDA   #<__local_main__a
   CMP   #<__local_main__a
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #>__local_main__a
   CMP   #>__local_main__a
   BNE   .jcmp_skip@1
   LDA   #<__local_main__a
   CMP   #<__local_main__a
   BEQ   .if_end@2
.jcmp_skip@1:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #>__local_main__a
   CMP   #>__local_main__b
   BNE   .if_end@3
   LDA   #<__local_main__a
   CMP   #<__local_main__b
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #>__local_main__a
   CMP   #>__local_main__b
   BNE   .if_end@4
   LDA   #<__local_main__a
   CMP   #<__local_main__b
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #>__local_main__a
   CMP   #>__local_main__a
   BNE   .jcmp_skip@2
   LDA   #<__local_main__a
   CMP   #<__local_main__a
   BEQ   .if_end@5
.jcmp_skip@2:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS
