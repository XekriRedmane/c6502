SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   str@0+2
   CMP   #$63
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   str@1+10
   CMP   #$73
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   str@1+16
   CMP   #$21
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   str@1+17
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #<str@2
   ORA   #>str@2
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $61, $62, $63, $64, $65, $66, $67, $00

str@1:
   DC.B  $54, $68, $69, $73, $20, $69, $73, $20, $61, $20, $73, $74, $72, $69, $6E, $67
   DC.B  $21, $00

str@2:
   DC.B  $4E, $6F, $74, $20, $61, $20, $6E, $75, $6C, $6C, $20, $70, $6F, $69, $6E, $74
   DC.B  $65, $72, $21, $00
