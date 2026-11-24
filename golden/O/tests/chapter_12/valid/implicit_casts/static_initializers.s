SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item u section=.data local=false zero=false
; item i section=.data local=false zero=false
; item l section=.data local=false zero=false
; item l2 section=.data local=false zero=false
; item ul section=.data local=false zero=false
; item ul2 section=.data local=false zero=false
; item i2 section=.data local=false zero=false
; item ui2 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   u+3
   BNE   .jcmp_skip@0
   LDA   u+2
   BNE   .jcmp_skip@0
   LDA   u+1
   CMP   #$80
   BNE   .jcmp_skip@0
   LDA   u
   CMP   #$0C
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   i+3
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   i+2
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   i+1
   CMP   #$80
   BNE   .jcmp_skip@1
   LDA   i
   CMP   #$02
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   l+7
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   l+6
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   l+5
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   l+4
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   l+3
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   l+2
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   l+1
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   l
   CMP   #$9C
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   l2+7
   BNE   .jcmp_skip@3
   LDA   l2+6
   BNE   .jcmp_skip@3
   LDA   l2+5
   BNE   .jcmp_skip@3
   LDA   l2+4
   BNE   .jcmp_skip@3
   LDA   l2+3
   BNE   .jcmp_skip@3
   LDA   l2+2
   BNE   .jcmp_skip@3
   LDA   l2+1
   CMP   #$80
   BNE   .jcmp_skip@3
   LDA   l2
   CMP   #$02
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   ul+7
   BNE   .jcmp_skip@4
   LDA   ul+6
   BNE   .jcmp_skip@4
   LDA   ul+5
   BNE   .jcmp_skip@4
   LDA   ul+4
   BNE   .jcmp_skip@4
   LDA   ul+3
   BNE   .jcmp_skip@4
   LDA   ul+2
   BNE   .jcmp_skip@4
   LDA   ul+1
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   ul
   CMP   #$FE
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   ul2+7
   BNE   .jcmp_skip@5
   LDA   ul2+6
   BNE   .jcmp_skip@5
   LDA   ul2+5
   BNE   .jcmp_skip@5
   LDA   ul2+4
   BNE   .jcmp_skip@5
   LDA   ul2+3
   CMP   #$7F
   BNE   .jcmp_skip@5
   LDA   ul2+2
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   ul2+1
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   ul2
   CMP   #$F6
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   i2+3
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   i2+2
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   i2+1
   CMP   #$80
   BNE   .jcmp_skip@6
   LDA   i2
   CMP   #$96
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   ui2+3
   BNE   .jcmp_skip@7
   LDA   ui2+2
   BNE   .jcmp_skip@7
   LDA   ui2+1
   CMP   #$80
   BNE   .jcmp_skip@7
   LDA   ui2
   CMP   #$96
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS

u:
   DC.L  $0200800C

i:
   DC.L  $00008002

l:
   DC.L  $FFFFFF9C
   DC.L  $00000000

l2:
   DC.L  $00008002
   DC.L  $00000000

ul:
   DC.L  $0000FFFE
   DC.L  $00000000

ul2:
   DC.L  $7FFFFFF6
   DC.L  $00000000

i2:
   DC.L  $80008096

ui2:
   DC.L  $80008096
