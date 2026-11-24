SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item @0.chars section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   @0.chars
   STA   __local_main__0
   CLC
   ADC   #$01
   STA   @0.chars
   LDA   __local_main__0
   CMP   #$7B
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   @0.chars+1
   STA   __local_main__0
   SEC
   SBC   #$01
   STA   @0.chars+1
   LDA   __local_main__0
   CMP   #$7C
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   INC   @0.chars+2
   LDA   @0.chars+2
   CMP   #$7E
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   DEC   @0.chars+3
   LDA   @0.chars+3
   CMP   #$7D
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   @0.chars+4
   CLC
   ADC   #$01
   STA   __local_main__0
   STA   @0.chars+4
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__0
   CMP   #$80
   BEQ   .if_end@4
.jcmp_skip@0:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   @0.chars
   CMP   #$7C
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   @0.chars+1
   CMP   #$7B
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   @0.chars+2
   CMP   #$7E
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   @0.chars+3
   CMP   #$7D
   BEQ   .jcmp_skip@1
   LDX   #$00
   LDA   #$09
   RTS
.jcmp_skip@1:
   LDX   #$00
   LDA   #$0A
   RTS

@0.chars:
   DC.B  $7B, $7C, $7D, $7E, $7F
