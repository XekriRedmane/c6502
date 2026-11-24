SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__local_main__strings_0	EQU	$82
__local_main__strings_7	EQU	$84
__local_main__strings_6	EQU	$85
__local_main__strings_5	EQU	$86
__local_main__strings_4	EQU	$87
__local_main__strings_3	EQU	$88
__local_main__strings_2	EQU	$89

; @zp-link-meta-begin
; def main params= locals=__local_main__strings_0,__local_main__0,__local_main__strings_7,__local_main__strings_6,__local_main__strings_5,__local_main__strings_4,__local_main__strings_3,__local_main__strings_2 indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call main -> strcmp
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item str@3 section=.rodata local=true zero=false
; item str@4 section=.rodata local=true zero=false
; item str@5 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDX   #>str@0
   LDA   #<str@1
   STA   __local_main__strings_2
   LDA   #>str@1
   STA   __local_main__strings_3
   LDA   #<str@2
   STA   __local_main__strings_4
   LDA   #>str@2
   STA   __local_main__strings_5
   LDA   #$00
   STA   __local_main__strings_6
   STA   __local_main__strings_7
   LDA   #<str@3
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@3
   STA   __zpabi_strcmp__s2_1
   LDA   #<str@0
   JSR   strcmp
   STX   __local_main__strings_0
   ORA   __local_main__strings_0
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #<str@4
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@4
   STA   __zpabi_strcmp__s2_1
   LDX   __local_main__strings_3
   LDA   __local_main__strings_2
   JSR   strcmp
   STX   __local_main__strings_0
   ORA   __local_main__strings_0
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #<str@5
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@5
   STA   __zpabi_strcmp__s2_1
   LDX   __local_main__strings_5
   LDA   __local_main__strings_4
   JSR   strcmp
   STX   __local_main__strings_0
   ORA   __local_main__strings_0
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__strings_6
   ORA   __local_main__strings_7
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $79, $65, $73, $00

str@1:
   DC.B  $6E, $6F, $00

str@2:
   DC.B  $6D, $61, $79, $62, $65, $00

str@3:
   DC.B  $79, $65, $73, $00

str@4:
   DC.B  $6E, $6F, $00

str@5:
   DC.B  $6D, $61, $79, $62, $65, $00
