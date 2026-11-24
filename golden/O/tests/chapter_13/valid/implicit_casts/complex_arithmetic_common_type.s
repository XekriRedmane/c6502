SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item ul section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   ul
   CLC
   ADC   #$CE
   STA   HARGS
   LDA   ul+1
   ADC   #$FF
   STA   HARGS+1
   LDA   ul+2
   ADC   #$FF
   STA   HARGS+2
   LDA   ul+3
   ADC   #$FF
   STA   HARGS+3
   JSR   ul2d
   LDA   HARGS+4
   STA   HARGS
   LDA   HARGS+5
   STA   HARGS+1
   LDA   HARGS+6
   STA   HARGS+2
   LDA   HARGS+7
   STA   HARGS+3
   LDA   HARGS+8
   STA   HARGS+4
   LDA   HARGS+9
   STA   HARGS+5
   LDA   HARGS+10
   STA   HARGS+6
   LDA   HARGS+11
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$09
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dmul
   LDA   HARGS+23
   CMP   #$40
   BNE   .thread@0
   LDA   HARGS+22
   CMP   #$DE
   BNE   .thread@0
   LDA   HARGS+21
   CMP   #$5D
   BNE   .thread@0
   LDA   HARGS+20
   CMP   #$70
   BNE   .thread@0
   LDA   HARGS+19
   BNE   .thread@0
   LDA   HARGS+18
   BNE   .thread@0
   LDA   HARGS+17
   BNE   .thread@0
   LDA   HARGS+16
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

ul:
   DC.L  $00002710
