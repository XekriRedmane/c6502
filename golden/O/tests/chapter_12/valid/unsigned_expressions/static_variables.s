SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item x section=.data local=true zero=false
; item zero_long section=.bss local=false zero=true
; item zero_int section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   x+7
   BNE   .jcmp_skip@0
   LDA   x+6
   BNE   .jcmp_skip@0
   LDA   x+5
   BNE   .jcmp_skip@0
   LDA   x+4
   BNE   .jcmp_skip@0
   LDA   x+3
   CMP   #$7F
   BNE   .jcmp_skip@0
   LDA   x+2
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   x+1
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   x
   CMP   #$FB
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   TXA
   RTS
.if_end@0:
   CLC
   ADC   #$0A
   STA   x
   BCC   .carry_inc@0
   INC   x+1
   BNE   .carry_inc@0
   INC   x+2
   BNE   .carry_inc@0
   INC   x+3
   BNE   .carry_inc@0
   INC   x+4
   BNE   .carry_inc@0
   INC   x+5
   BNE   .carry_inc@0
   INC   x+6
   BNE   .carry_inc@0
   INC   x+7
.carry_inc@0:
   LDA   x+7
   BNE   .main@jthread@0
   LDA   x+6
   BNE   .main@jthread@0
   LDA   x+5
   BNE   .main@jthread@0
   LDA   x+4
   BNE   .main@jthread@0
   LDA   x+3
   CMP   #$80
   BNE   .main@jthread@0
   LDA   x+2
   BNE   .main@jthread@0
   LDA   x+1
   BNE   .main@jthread@0
   LDA   x
   CMP   #$05
   BNE   .main@jthread@0
   LDA   zero_long
   ORA   zero_long+1
   ORA   zero_long+2
   ORA   zero_long+3
   ORA   zero_long+4
   ORA   zero_long+5
   ORA   zero_long+6
   ORA   zero_long+7
   BNE   .main@jthread@0
   LDA   zero_int
   ORA   zero_int+1
   BEQ   .if_end@4
.main@jthread@0:
   LDX   #$00
   TXA
   RTS
.if_end@4:
   LDX   #$00
   LDA   #$01
   RTS

x:
   DC.L  $7FFFFFFB
   DC.L  $00000000

zero_long:
   DS.B  8

zero_int:
   DS.B  2
