SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__local_main__0	EQU	$82
__local_main__2	EQU	$84
__local_main__multi_string	EQU	$85
__local_main__nested_multi_string	EQU	$8B

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__multi_string,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__nested_multi_string,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14 indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call main -> strcmp
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$79
   STA   __local_main__multi_string
   LDA   #$65
   STA   __local_main__multi_string+1
   LDA   #$73
   STA   __local_main__multi_string+2
   LDA   #$6E
   STA   __local_main__multi_string+3
   LDA   #$6F
   STA   __local_main__multi_string+4
   LDA   #$00
   STA   __local_main__multi_string+5
   LDA   #$61
   STA   __local_main__nested_multi_string
   LDA   #$62
   STA   __local_main__nested_multi_string+1
   LDA   #$00
   STA   __local_main__nested_multi_string+2
   LDA   #$63
   STA   __local_main__nested_multi_string+3
   LDA   #$64
   STA   __local_main__nested_multi_string+4
   LDA   #$00
   STA   __local_main__nested_multi_string+5
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDX   #>__local_main__multi_string
   LDA   #<__local_main__multi_string
   JSR   strcmp
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #>__local_main__nested_multi_string
   STA   __local_main__2
   CLC
   BCC   .carry_inc@0
   INC   __local_main__2
.carry_inc@0:
   LDA   #<str@1
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@1
   STA   __zpabi_strcmp__s2_1
   LDX   __local_main__2
   LDA   #<__local_main__nested_multi_string
   JSR   strcmp
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #>__local_main__nested_multi_string
   STA   __local_main__2
   LDA   #<__local_main__nested_multi_string
   CLC
   ADC   #$03
   STA   __local_main__0
   BCC   .carry_inc@1
   INC   __local_main__2
.carry_inc@1:
   LDA   #<str@2
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@2
   STA   __zpabi_strcmp__s2_1
   LDX   __local_main__2
   LDA   __local_main__0
   JSR   strcmp
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $79, $65, $73, $6E, $6F, $00

str@1:
   DC.B  $61, $62, $00

str@2:
   DC.B  $63, $64, $00
