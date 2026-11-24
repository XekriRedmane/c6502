SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__y	EQU	$8A
__local_main__z	EQU	$8C
__local_main__arr	EQU	$8E
__local_main__arr2	EQU	$94

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__array_of_pointers_0,__local_main__array_of_pointers_1,__local_main__array_of_pointers_4,__local_main__array_of_pointers_5,__local_main__array_of_pointers_2,__local_main__array_of_pointers_3,__local_main__x,__local_main__3,__local_main__y,__local_main__5,__local_main__z,__local_main__7,__local_main__arr,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__arr2,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #>__local_main__arr
   CMP   #>__local_main__arr
   BNE   .jcmp_skip@0
   LDA   #<__local_main__arr
   CMP   #<__local_main__arr
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #>__local_main__arr2
   CMP   #>__local_main__arr2
   BNE   .jcmp_skip@1
   LDA   #<__local_main__arr2
   CMP   #<__local_main__arr2
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #>__local_main__arr
   CMP   #>__local_main__arr
   BNE   .jcmp_skip@2
   LDA   #<__local_main__arr
   CMP   #<__local_main__arr
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #>__local_main__z
   CMP   #>__local_main__z
   BNE   .jcmp_skip@3
   LDA   #<__local_main__z
   CMP   #<__local_main__z
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #>__local_main__y
   CMP   #>__local_main__y
   BNE   .jcmp_skip@4
   LDA   #<__local_main__y
   CMP   #<__local_main__y
   BEQ   .if_end@5
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS
