SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i_1	EQU	$80
__local_main__i_0	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__i_1,__local_main__i_0 indirect=false in_cycle=false
; def print_letters params= locals= indirect=false in_cycle=false
; ext putchar params=__zpabi_putchar__ch_0,__zpabi_putchar__ch_1 param_regs=A,X
; call main -> print_letters
; call print_letters -> putchar
; item print_letters section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @1.i section=.data local=true zero=false
; item @2.i section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

print_letters:
   SUBROUTINE

   LDX   @1.i+1
   LDA   @1.i
   JSR   putchar
   INC   @1.i
   BNE   .inc_done@0
   INC   @1.i+1
.inc_done@0:
   LDX   @2.i+1
   LDA   @2.i
   JSR   putchar
   INC   @2.i
   BNE   .inc_done@1
   INC   @2.i+1
.inc_done@1:
   LDX   #$00
   LDA   #$0A
   JSR   putchar
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$1A
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   JSR   print_letters
   INC   __local_main__i_0
   BNE   .loop@0_start
   INC   __local_main__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$00
   TXA
   RTS

@1.i:
   DC.W  $0041

@2.i:
   DC.W  $0061
