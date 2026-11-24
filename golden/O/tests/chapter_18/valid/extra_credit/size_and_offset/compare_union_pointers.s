SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item my_union section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #>my_union
   CMP   #>my_union
   BNE   .jcmp_skip@0
   LDA   #<my_union
   CMP   #<my_union
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #>my_union
   CMP   #>my_union
   BNE   .jcmp_skip@1
   LDA   #<my_union
   CMP   #<my_union
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #>my_union
   CMP   #>my_union
   BNE   .jcmp_skip@2
   LDA   #<my_union
   CMP   #<my_union
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #>my_union
   CMP   #>my_union
   BNE   .jcmp_skip@3
   LDA   #<my_union
   CMP   #<my_union
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #<my_union
   SEC
   SBC   #<my_union
   LDA   #>my_union
   SBC   #>my_union
   BCS   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #<my_union
   SBC   #<my_union
   LDA   #>my_union
   SBC   #>my_union
   BCS   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS

my_union:
   DS.B  8
