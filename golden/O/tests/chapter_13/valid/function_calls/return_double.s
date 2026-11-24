SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def d params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; call main -> d
; item d section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

d:
   SUBROUTINE

   LDA   #$2F
   STA   HARGS+16
   LDA   #$F7
   STA   HARGS+17
   LDA   #$76
   STA   HARGS+18
   LDA   #$7F
   STA   HARGS+19
   LDA   #$66
   STA   HARGS+20
   LDA   #$50
   STA   HARGS+21
   LDY   #$25
   STY   HARGS+22
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   JSR   d
   LDA   HARGS+23
   CMP   #$50
   BNE   .thread@0
   LDA   HARGS+22
   CMP   #$25
   BNE   .thread@0
   LDA   HARGS+21
   CMP   #$50
   BNE   .thread@0
   LDA   HARGS+20
   CMP   #$66
   BNE   .thread@0
   LDA   HARGS+19
   CMP   #$7F
   BNE   .thread@0
   LDA   HARGS+18
   CMP   #$76
   BNE   .thread@0
   LDA   HARGS+17
   CMP   #$F7
   BNE   .thread@0
   LDA   HARGS+16
   CMP   #$2F
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS
