SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item d1 section=.data local=false zero=false
; item d2 section=.data local=false zero=false
; item d3 section=.data local=false zero=false
; item d4 section=.data local=false zero=false
; item d5 section=.data local=false zero=false
; item d6 section=.data local=false zero=false
; item d7 section=.data local=false zero=false
; item uninitialized section=.bss local=false zero=true
; item i section=.data local=true zero=false
; item u section=.data local=false zero=false
; item l section=.data local=false zero=false
; item ul section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   d1+7
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   d1+6
   CMP   #$DF
   BNE   .jcmp_skip@0
   LDA   d1+5
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   d1+4
   CMP   #$C0
   BNE   .jcmp_skip@0
   LDA   d1+3
   BNE   .jcmp_skip@0
   LDA   d1+2
   BNE   .jcmp_skip@0
   LDA   d1+1
   BNE   .jcmp_skip@0
   LDA   d1
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   d2+7
   CMP   #$40
   BNE   .jcmp_skip@1
   LDA   d2+6
   CMP   #$EF
   BNE   .jcmp_skip@1
   LDA   d2+5
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   d2+4
   CMP   #$E0
   BNE   .jcmp_skip@1
   LDA   d2+3
   BNE   .jcmp_skip@1
   LDA   d2+2
   BNE   .jcmp_skip@1
   LDA   d2+1
   BNE   .jcmp_skip@1
   LDA   d2
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   d3+7
   CMP   #$41
   BNE   .jcmp_skip@2
   LDA   d3+6
   CMP   #$CD
   BNE   .jcmp_skip@2
   LDA   d3+5
   CMP   #$CD
   BNE   .jcmp_skip@2
   LDA   d3+4
   CMP   #$65
   BNE   .jcmp_skip@2
   LDA   d3+3
   BNE   .jcmp_skip@2
   LDA   d3+2
   BNE   .jcmp_skip@2
   LDA   d3+1
   BNE   .jcmp_skip@2
   LDA   d3
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   d4+7
   CMP   d3+7
   BNE   .jcmp_skip@3
   LDA   d4+6
   CMP   d3+6
   BNE   .jcmp_skip@3
   LDA   d4+5
   CMP   d3+5
   BNE   .jcmp_skip@3
   LDA   d4+4
   CMP   d3+4
   BNE   .jcmp_skip@3
   LDA   d4+3
   CMP   d3+3
   BNE   .jcmp_skip@3
   LDA   d4+2
   CMP   d3+2
   BNE   .jcmp_skip@3
   LDA   d4+1
   CMP   d3+1
   BNE   .jcmp_skip@3
   LDA   d4
   CMP   d3
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   d5+7
   CMP   #$41
   BNE   .jcmp_skip@4
   LDA   d5+6
   CMP   #$ED
   BNE   .jcmp_skip@4
   LDA   d5+5
   CMP   #$CD
   BNE   .jcmp_skip@4
   LDA   d5+4
   CMP   #$65
   BNE   .jcmp_skip@4
   LDA   d5+3
   BNE   .jcmp_skip@4
   LDA   d5+2
   BNE   .jcmp_skip@4
   LDA   d5+1
   BNE   .jcmp_skip@4
   LDA   d5
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   d6+7
   CMP   d3+7
   BNE   .jcmp_skip@5
   LDA   d6+6
   CMP   d3+6
   BNE   .jcmp_skip@5
   LDA   d6+5
   CMP   d3+5
   BNE   .jcmp_skip@5
   LDA   d6+4
   CMP   d3+4
   BNE   .jcmp_skip@5
   LDA   d6+3
   CMP   d3+3
   BNE   .jcmp_skip@5
   LDA   d6+2
   CMP   d3+2
   BNE   .jcmp_skip@5
   LDA   d6+1
   CMP   d3+1
   BNE   .jcmp_skip@5
   LDA   d6
   CMP   d3
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   d7+7
   CMP   d5+7
   BNE   .jcmp_skip@6
   LDA   d7+6
   CMP   d5+6
   BNE   .jcmp_skip@6
   LDA   d7+5
   CMP   d5+5
   BNE   .jcmp_skip@6
   LDA   d7+4
   CMP   d5+4
   BNE   .jcmp_skip@6
   LDA   d7+3
   CMP   d5+3
   BNE   .jcmp_skip@6
   LDA   d7+2
   CMP   d5+2
   BNE   .jcmp_skip@6
   LDA   d7+1
   CMP   d5+1
   BNE   .jcmp_skip@6
   LDA   d7
   CMP   d5
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   uninitialized
   ORA   uninitialized+1
   ORA   uninitialized+2
   ORA   uninitialized+3
   ORA   uninitialized+4
   ORA   uninitialized+5
   ORA   uninitialized+6
   ORA   uninitialized+7
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   i+1
   BNE   .jcmp_skip@7
   LDA   i
   CMP   #$04
   BEQ   .if_end@8
.jcmp_skip@7:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDA   u+3
   BNE   .jcmp_skip@8
   LDA   u+2
   BNE   .jcmp_skip@8
   LDA   u+1
   CMP   #$5B
   BNE   .jcmp_skip@8
   LDA   u
   CMP   #$E0
   BEQ   .if_end@9
.jcmp_skip@8:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   LDA   l+7
   BNE   .jcmp_skip@9
   LDA   l+6
   BNE   .jcmp_skip@9
   LDA   l+5
   BNE   .jcmp_skip@9
   LDA   l+4
   BNE   .jcmp_skip@9
   LDA   l+3
   CMP   #$3B
   BNE   .jcmp_skip@9
   LDA   l+2
   CMP   #$9A
   BNE   .jcmp_skip@9
   LDA   l+1
   CMP   #$CA
   BNE   .jcmp_skip@9
   LDA   l
   BEQ   .if_end@10
.jcmp_skip@9:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   LDA   ul+7
   BNE   .jcmp_skip@10
   LDA   ul+6
   BNE   .jcmp_skip@10
   LDA   ul+5
   BNE   .jcmp_skip@10
   LDA   ul+4
   BNE   .jcmp_skip@10
   LDA   ul+3
   CMP   #$EE
   BNE   .jcmp_skip@10
   LDA   ul+2
   CMP   #$6B
   BNE   .jcmp_skip@10
   LDA   ul+1
   CMP   #$28
   BNE   .jcmp_skip@10
   LDA   ul
   BEQ   .if_end@11
.jcmp_skip@10:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   LDX   #$00
   TXA
   RTS

d1:
   DC.L  $00000000
   DC.L  $40DFFFC0

d2:
   DC.L  $00000000
   DC.L  $40EFFFE0

d3:
   DC.L  $00000000
   DC.L  $41CDCD65

d4:
   DC.L  $00000000
   DC.L  $41CDCD65

d5:
   DC.L  $00000000
   DC.L  $41EDCD65

d6:
   DC.L  $00000000
   DC.L  $41CDCD65

d7:
   DC.L  $00000000
   DC.L  $41EDCD65

uninitialized:
   DS.B  8

i:
   DC.W  $0004

u:
   DC.L  $028F5BE0

l:
   DC.L  $3B9ACA00
   DC.L  $00000000

ul:
   DC.L  $EE6B2800
   DC.L  $00000000
