SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def print_A params= locals= indirect=false in_cycle=false
; def print_B params= locals= indirect=false in_cycle=false
; ext putchar params=__zpabi_putchar__c_0,__zpabi_putchar__c_1 param_regs=A,X
; call main -> print_A
; call main -> print_B
; call print_A -> putchar
; call print_B -> putchar
; item print_A section=.text local=false zero=false
; item print_A@tail0 section=.text local=true zero=false
; item print_B section=.text local=false zero=false
; item main section=.text local=false zero=false
; item i section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

print_A:
   SUBROUTINE

   LDX   #$00
   LDA   #$41
   JMP   print_A@tail0

print_A@tail0:
   SUBROUTINE

   JSR   putchar
   LDX   #>i
   LDA   #<i
   RTS

print_B:
   SUBROUTINE

   LDX   #$00
   LDA   #$42
   JMP   print_A@tail0

main:
   SUBROUTINE

   JSR   print_A
   STA   __local_main__2
   STX   __local_main__3
   LDY   #$01
   LDA   (__local_main__2),Y
   STA   __local_main__1
   DEY
   LDA   (__local_main__2),Y
   CLC
   ADC   #$05
   BCC   .carry_inc@0
   INC   __local_main__1
.carry_inc@0:
   LDY   #$00
   STA   (__local_main__2),Y
   LDA   __local_main__1
   INY
   STA   (__local_main__2),Y
   LDA   i+1
   BNE   .jcmp_skip@0
   LDA   i
   CMP   #$05
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   print_B
   STA   __local_main__2
   STX   __local_main__3
   LDY   #$01
   LDA   (__local_main__2),Y
   STA   __local_main__1
   DEY
   LDA   (__local_main__2),Y
   CLC
   ADC   #$05
   BCC   .carry_inc@1
   INC   __local_main__1
.carry_inc@1:
   LDY   #$00
   STA   (__local_main__2),Y
   LDA   __local_main__1
   INY
   STA   (__local_main__2),Y
   LDA   i+1
   BNE   .jcmp_skip@1
   LDA   i
   CMP   #$0A
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

i:
   DS.B  2
