SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2 indirect=false in_cycle=false
; ext puts params=__zpabi_puts__s_0,__zpabi_puts__s_1 param_regs=A,X
; call main -> puts
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDX   #>str@0
   LDA   #<str@0
   JSR   puts
   LDA   #>(str@0+16)
   STA   __local_main__1
   LDA   #<(str@0+16)
   SEC
   SBC   #$01
   STA   __local_main__0
   BCS   .carry_inc@0
   DEC   __local_main__1
.carry_inc@0:
   LDY   #$00
   LDA   (__local_main__0),Y
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $53, $61, $6D, $70, $6C, $65, $09, $73, $74, $72, $69, $6E, $67, $21, $0A, $00
