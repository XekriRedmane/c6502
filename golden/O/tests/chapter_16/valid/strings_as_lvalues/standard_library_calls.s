SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__local_main__0	EQU	$82
__local_main__1	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; ext atoi params=__zpabi_atoi__s_0,__zpabi_atoi__s_1 param_regs=A,X
; ext puts params=__zpabi_puts__s_0,__zpabi_puts__s_1 param_regs=A,X
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; ext strlen params=__zpabi_strlen__s_0,__zpabi_strlen__s_1 param_regs=A,X
; call main -> atoi
; call main -> puts
; call main -> strcmp
; call main -> strlen
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item str@3 section=.rodata local=true zero=false
; item str@4 section=.rodata local=true zero=false
; item str@5 section=.rodata local=true zero=true
; item str@6 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #<str@1
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@1
   STA   __zpabi_strcmp__s2_1
   LDX   #>str@0
   LDA   #<str@0
   JSR   strcmp
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #<str@3
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@3
   STA   __zpabi_strcmp__s2_1
   LDX   #>str@2
   LDA   #<str@2
   JSR   strcmp
   TXA
   BMI   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #>str@4
   LDA   #<str@4
   JSR   puts
   LDX   #>str@5
   LDA   #<str@5
   JSR   strlen
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #>str@6
   LDA   #<str@6
   JSR   atoi
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@3
.jcmp_skip@0:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $61, $62, $63, $00

str@1:
   DC.B  $61, $62, $63, $00

str@2:
   DC.B  $61, $62, $00

str@3:
   DC.B  $78, $79, $00

str@4:
   DC.B  $48, $65, $6C, $6C, $6F, $2C, $20, $57, $6F, $72, $6C, $64, $21, $00

str@5:
   DS.B  1

str@6:
   DC.B  $31, $30, $00
