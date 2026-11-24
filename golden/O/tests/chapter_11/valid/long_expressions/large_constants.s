SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def add_large params= locals=__local_add_large__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def multiply_by_large params= locals=__local_multiply_by_large__0 indirect=false in_cycle=false
; def subtract_large params= locals=__local_subtract_large__0 indirect=false in_cycle=false
; call main -> add_large
; call main -> multiply_by_large
; call main -> subtract_large
; item add_large section=.text local=false zero=false
; item subtract_large section=.text local=false zero=false
; item multiply_by_large section=.text local=false zero=false
; item main section=.text local=false zero=false
; item x section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

add_large:
   SUBROUTINE

   LDA   x
   CLC
   ADC   #$F8
   STA   x
   LDA   x+1
   ADC   #$FF
   STA   x+1
   LDA   x+2
   ADC   #$FF
   STA   x+2
   LDA   x+3
   ADC   #$7F
   STA   x+3
   BCC   .carry_inc@0
   INC   x+4
   BNE   .carry_inc@0
   INC   x+5
   BNE   .carry_inc@0
   INC   x+6
   BNE   .carry_inc@0
   INC   x+7
.carry_inc@0:
   LDA   x+7
   BNE   .thread@0
   LDA   x+6
   BNE   .thread@0
   LDA   x+5
   BNE   .thread@0
   LDA   x+4
   BNE   .thread@0
   LDA   x+3
   CMP   #$7F
   BNE   .thread@0
   LDA   x+2
   CMP   #$FF
   BNE   .thread@0
   LDA   x+1
   CMP   #$FF
   BNE   .thread@0
   LDA   x
   CMP   #$FD
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

subtract_large:
   SUBROUTINE

   LDA   x
   SEC
   SBC   #$F8
   STA   x
   LDA   x+1
   SBC   #$FF
   STA   x+1
   LDA   x+2
   SBC   #$FF
   STA   x+2
   LDA   x+3
   SBC   #$7F
   STA   x+3
   LDA   x+4
   SBC   #$00
   STA   x+4
   LDA   x+5
   SBC   #$00
   STA   x+5
   LDA   x+6
   SBC   #$00
   STA   x+6
   BCS   .carry_inc@1
   DEC   x+7
.carry_inc@1:
   LDA   x+7
   BNE   .thread@1
   LDA   x+6
   BNE   .thread@1
   LDA   x+5
   BNE   .thread@1
   LDA   x+4
   BNE   .thread@1
   LDA   x+3
   BNE   .thread@1
   LDA   x+2
   BNE   .thread@1
   LDA   x+1
   BNE   .thread@1
   LDA   x
   CMP   #$05
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

multiply_by_large:
   SUBROUTINE

   LDA   x
   STA   HARGS
   LDA   x+1
   STA   HARGS+1
   LDA   x+2
   STA   HARGS+2
   LDA   x+3
   STA   HARGS+3
   LDA   x+4
   STA   HARGS+4
   LDA   x+5
   STA   HARGS+5
   LDA   x+6
   STA   HARGS+6
   LDA   x+7
   STA   HARGS+7
   LDA   #$C2
   STA   HARGS+9
   LDA   #$EB
   STA   HARGS+10
   LDA   #$0B
   STA   HARGS+11
   LDA   #$00
   STA   HARGS+8
   STA   HARGS+12
   LDY   #$00
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   STA   x
   LDA   HARGS+17
   STA   x+1
   LDA   HARGS+18
   STA   x+2
   LDA   HARGS+19
   STA   x+3
   LDA   HARGS+20
   STA   x+4
   LDA   HARGS+21
   STA   x+5
   LDA   HARGS+22
   STA   x+6
   LDA   HARGS+23
   STA   x+7
   BNE   .thread@2
   LDA   x+6
   BNE   .thread@2
   LDA   x+5
   BNE   .thread@2
   LDA   x+4
   BNE   .thread@2
   LDA   x+3
   CMP   #$3B
   BNE   .thread@2
   LDA   x+2
   CMP   #$9A
   BNE   .thread@2
   LDA   x+1
   CMP   #$CA
   BNE   .thread@2
   LDA   x
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   JSR   add_large
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   subtract_large
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   multiply_by_large
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

x:
   DC.L  $00000005
   DC.L  $00000000
