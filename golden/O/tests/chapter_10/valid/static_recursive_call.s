SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; def print_alphabet params= locals= indirect=false in_cycle=true
; ext putchar params=__zpabi_putchar__ch_0,__zpabi_putchar__ch_1 param_regs=A,X
; call main -> print_alphabet
; call print_alphabet -> print_alphabet
; call print_alphabet -> putchar
; item print_alphabet section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @1.count section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

print_alphabet:
   SUBROUTINE

   LDA   @1.count
   CLC
   ADC   #$41
   STA   $80
   LDA   @1.count+1
   ADC   #$00
   TAX
   LDA   $80
   JSR   putchar
   INC   @1.count
   BNE   .inc_done@0
   INC   @1.count+1
.inc_done@0:
   LDA   @1.count
   SEC
   SBC   #$1A
   LDA   @1.count+1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .if_end@0
   JSR   print_alphabet
.if_end@0:
   LDX   @1.count+1
   LDA   @1.count
   RTS

main:
   SUBROUTINE

   JSR   print_alphabet
   LDX   #$00
   TXA
   RTS

@1.count:
   DS.B  2
