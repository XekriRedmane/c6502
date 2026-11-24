SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; ext putchar params=__zpabi_putchar__c_0,__zpabi_putchar__c_1 param_regs=A,X
; call main -> putchar
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$48
   JSR   putchar
   LDX   #$00
   LDA   #$65
   JSR   putchar
   LDX   #$00
   LDA   #$6C
   JSR   putchar
   LDX   #$00
   LDA   #$6C
   JSR   putchar
   LDX   #$00
   LDA   #$6F
   JSR   putchar
   LDX   #$00
   LDA   #$2C
   JSR   putchar
   LDX   #$00
   LDA   #$20
   JSR   putchar
   LDX   #$00
   LDA   #$57
   JSR   putchar
   LDX   #$00
   LDA   #$6F
   JSR   putchar
   LDX   #$00
   LDA   #$72
   JSR   putchar
   LDX   #$00
   LDA   #$6C
   JSR   putchar
   LDX   #$00
   LDA   #$64
   JSR   putchar
   LDX   #$00
   LDA   #$21
   JSR   putchar
   LDX   #$00
   LDA   #$0A
   JSR   putchar
   LDX   #$00
   TXA
   RTS
