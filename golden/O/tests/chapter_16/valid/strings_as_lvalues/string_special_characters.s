SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__local_main__0	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; ext puts params=__zpabi_puts__s_0,__zpabi_puts__s_1 param_regs=A,X
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call main -> puts
; call main -> strcmp
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item str@3 section=.rodata local=true zero=false
; item str@4 section=.rodata local=true zero=false
; item str@5 section=.rodata local=true zero=false
; item str@6 section=.rodata local=true zero=false
; item str@7 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   str@0
   CMP   #$07
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   str@0+1
   CMP   #$08
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   str@0+2
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   str@1+5
   CMP   #$22
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #>str@1
   LDA   #<str@1
   JSR   puts
   LDA   str@2+5
   CMP   #$5C
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #>str@2
   LDA   #<str@2
   JSR   puts
   LDA   str@3+4
   CMP   #$0A
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDX   #>str@3
   LDA   #<str@3
   JSR   puts
   LDA   #<str@5
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@5
   STA   __zpabi_strcmp__s2_1
   LDX   #>str@4
   LDA   #<str@4
   JSR   strcmp
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDX   #>str@6
   LDA   #<str@6
   JSR   puts
   LDX   #>str@7
   LDA   #<str@7
   JSR   puts
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $07, $08, $00

str@1:
   DC.B  $48, $65, $6C, $6C, $6F, $22, $77, $6F, $72, $6C, $64, $00

str@2:
   DC.B  $48, $65, $6C, $6C, $6F, $5C, $57, $6F, $72, $6C, $64, $00

str@3:
   DC.B  $4C, $69, $6E, $65, $0A, $62, $72, $65, $61, $6B, $21, $00

str@4:
   DC.B  $09, $00

str@5:
   DC.B  $09, $00

str@6:
   DC.B  $54, $65, $73, $74, $69, $6E, $67, $2C, $20, $31, $32, $33, $2E, $00

str@7:
   DC.B  $5E, $40, $31, $20, $5F, $5C, $5D, $00
