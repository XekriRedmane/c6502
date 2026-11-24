SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__auto2_1	EQU	$84

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__auto3_1,__local_main__auto3_0,__local_main__auto2_2,__local_main__auto2_1,__local_main__auto2_0,__local_main__auto1_4,__local_main__auto1_3,__local_main__auto1_2,__local_main__auto1_1,__local_main__auto1_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item static1 section=.data local=false zero=false
; item static2 section=.data local=false zero=false
; item static3 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   static1
   CMP   #$01
   BNE   .main@jthread@2
   LDA   static1+1
   CMP   #$02
   BNE   .main@jthread@2
   LDA   static1+2
   BNE   .main@jthread@2
   LDA   static1+3
   BEQ   .if_end@6
.main@jthread@2:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@6:
   LDA   static2
   CMP   #$03
   BNE   .main@jthread@5
   LDA   static2+1
   CMP   #$04
   BNE   .main@jthread@5
   LDA   static2+2
   BNE   .main@jthread@5
   LDA   static2+3
   BEQ   .if_end@13
.main@jthread@5:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@13:
   LDA   static3
   CMP   #$05
   BNE   .main@jthread@7
   LDA   static3+1
   BNE   .main@jthread@7
   LDA   static3+2
   BEQ   .if_end@18
.main@jthread@7:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@18:
   SEC
   LDA   #$00
   SBC   static1
   STA   __local_main__auto2_1
   LDY   #$61
   LDA   #$FC
   BMI   .if_end@27
   LDX   #$00
   LDA   #$04
   RTS
.if_end@27:
   LDA   static1+2
   BNE   .main@jthread@13
   LDA   __local_main__auto2_1
   BPL   .main@jthread@13
   CMP   #$FF
   BNE   .main@jthread@13
   LDA   #$00
   BEQ   .if_end@32
.main@jthread@13:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@32:
   CPY   #$61
   BNE   .main@jthread@14
   ORA   #$00
   BEQ   .if_end@35
.main@jthread@14:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@35:
   LDX   #$00
   RTS

static1:
   DC.B  $01, $02, $00, $00

static2:
   DC.B  $03, $04, $00, $00

static3:
   DC.B  $05, $00, $00
