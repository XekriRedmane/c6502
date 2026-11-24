SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item zero section=.bss local=false zero=true
; item non_zero section=.data local=false zero=false
; item one section=.data local=false zero=false
; item rounded_to_zero section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   JSR   main@run0
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   main@run2
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   main@run1
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   main@run1
   BNE   .if_end@5
   LDX   #$00
   LDA   #$05
   RTS
.if_end@5:
   JSR   main@run0
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$06
   RTS
.if_end@6:
   JSR   main@run2
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$07
   RTS
.if_end@7:
   JSR   main@run1
   BNE   .if_end@10
   LDX   #$00
   LDA   #$08
   RTS
.if_end@10:
   JSR   main@run0
   BEQ   .if_end@13
   LDX   #$00
   LDA   #$09
   RTS
.if_end@13:
   JSR   main@run2
   BEQ   .if_end@16
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@16:
   JSR   main@run0
   BEQ   .if_end@19
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@19:
   JSR   main@run1
   BNE   .if_end@22
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@22:
   JSR   main@run0
   BNE   .main@jthread@5
   JSR   main@run2
   BEQ   .if_end@28
.main@jthread@5:
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@28:
   JSR   main@run1
   BNE   .if_end@34
   LDX   #$00
   LDA   #$10
   RTS
.if_end@34:
   LDX   #$00
   TXA
   RTS

main@run2:
   SUBROUTINE

   LDA   rounded_to_zero
   ORA   rounded_to_zero+1
   ORA   rounded_to_zero+2
   ORA   rounded_to_zero+3
   ORA   rounded_to_zero+4
   ORA   rounded_to_zero+5
   ORA   rounded_to_zero+6
   ORA   rounded_to_zero+7
   RTS

main@run1:
   SUBROUTINE

   LDA   non_zero
   ORA   non_zero+1
   ORA   non_zero+2
   ORA   non_zero+3
   ORA   non_zero+4
   ORA   non_zero+5
   ORA   non_zero+6
   ORA   non_zero+7
   RTS

main@run0:
   SUBROUTINE

   LDA   zero
   ORA   zero+1
   ORA   zero+2
   ORA   zero+3
   ORA   zero+4
   ORA   zero+5
   ORA   zero+6
   ORA   zero+7
   RTS

zero:
   DS.B  8

non_zero:
   DC.L  $0C924223
   DC.L  $3BC79CA1

one:
   DC.L  $00000000
   DC.L  $3FF00000

rounded_to_zero:
   DS.B  8
