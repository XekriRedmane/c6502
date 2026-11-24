SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run4 section=.text local=true zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item fifty_fiveE5 section=.data local=false zero=false
; item fifty_fourE4 section=.data local=false zero=false
; item tiny section=.data local=false zero=false
; item four section=.data local=false zero=false
; item point_one section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=17
; @zp-link-meta-end

main:
   SUBROUTINE

   JSR   main@run3
   JSR   dle
   LDA   HARGS+16
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   four
   STA   HARGS
   LDA   four+1
   STA   HARGS+1
   LDA   four+2
   STA   HARGS+2
   LDA   four+3
   STA   HARGS+3
   LDA   four+4
   STA   HARGS+4
   LDA   four+5
   STA   HARGS+5
   LDA   four+6
   STA   HARGS+6
   LDA   four+7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$10
   STA   HARGS+14
   LDA   #$40
   JSR   main@run4
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   JSR   main@run1
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   main@run3
   JSR   dlt
   LDA   HARGS+16
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   tiny
   ORA   tiny+1
   ORA   tiny+2
   ORA   tiny+3
   ORA   tiny+4
   ORA   tiny+5
   ORA   tiny+6
   ORA   tiny+7
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   point_one+7
   CMP   point_one+7
   BNE   .jcmp_skip@0
   LDA   point_one+6
   CMP   point_one+6
   BNE   .jcmp_skip@0
   LDA   point_one+5
   CMP   point_one+5
   BNE   .jcmp_skip@0
   LDA   point_one+4
   CMP   point_one+4
   BNE   .jcmp_skip@0
   LDA   point_one+3
   CMP   point_one+3
   BNE   .jcmp_skip@0
   LDA   point_one+2
   CMP   point_one+2
   BNE   .jcmp_skip@0
   LDA   point_one+1
   CMP   point_one+1
   BNE   .jcmp_skip@0
   LDA   point_one
   CMP   point_one
   BEQ   .if_end@5
.jcmp_skip@0:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   JSR   main@run2
   LDA   #$D4
   STA   HARGS+6
   LDA   #$3E
   JSR   main@run1
   BNE   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   JSR   main@run2
   LDA   #$04
   STA   HARGS+6
   LDA   #$BF
   STA   HARGS+7
   LDA   four
   STA   HARGS+8
   LDA   four+1
   STA   HARGS+9
   LDA   four+2
   STA   HARGS+10
   LDA   four+3
   STA   HARGS+11
   LDA   four+4
   STA   HARGS+12
   LDA   four+5
   STA   HARGS+13
   LDA   four+6
   STA   HARGS+14
   LDA   four+7
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   BNE   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   tiny
   STA   HARGS
   STA   HARGS+8
   LDA   tiny+1
   STA   HARGS+1
   STA   HARGS+9
   LDA   tiny+2
   STA   HARGS+2
   STA   HARGS+10
   LDA   tiny+3
   STA   HARGS+3
   STA   HARGS+11
   LDA   tiny+4
   STA   HARGS+4
   STA   HARGS+12
   LDA   tiny+5
   STA   HARGS+5
   STA   HARGS+13
   LDA   tiny+6
   STA   HARGS+6
   STA   HARGS+14
   LDA   tiny+7
   STA   HARGS+7
   JSR   main@run4
   BNE   .if_end@8
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDA   fifty_fiveE5
   STA   HARGS
   STA   HARGS+8
   LDA   fifty_fiveE5+1
   STA   HARGS+1
   STA   HARGS+9
   LDA   fifty_fiveE5+2
   STA   HARGS+2
   STA   HARGS+10
   LDA   fifty_fiveE5+3
   STA   HARGS+3
   STA   HARGS+11
   LDA   fifty_fiveE5+4
   STA   HARGS+4
   STA   HARGS+12
   LDA   fifty_fiveE5+5
   STA   HARGS+5
   STA   HARGS+13
   LDA   fifty_fiveE5+6
   STA   HARGS+6
   STA   HARGS+14
   LDA   fifty_fiveE5+7
   STA   HARGS+7
   JSR   main@run4
   BNE   .if_end@9
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   LDA   #$3F
   CMP   point_one+7
   BNE   .jcmp_skip@1
   LDA   #$B9
   CMP   point_one+6
   BNE   .jcmp_skip@1
   LDA   #$99
   CMP   point_one+5
   BNE   .jcmp_skip@1
   CMP   point_one+4
   BNE   .jcmp_skip@1
   CMP   point_one+3
   BNE   .jcmp_skip@1
   CMP   point_one+2
   BNE   .jcmp_skip@1
   CMP   point_one+1
   BNE   .jcmp_skip@1
   LDA   #$9A
   CMP   point_one
   BEQ   .if_end@10
.jcmp_skip@1:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   LDA   tiny+7
   CMP   #$3E
   BNE   .if_end@11
   LDA   tiny+6
   CMP   #$FF
   BNE   .if_end@11
   LDA   tiny+5
   CMP   #$75
   BNE   .if_end@11
   LDA   tiny+4
   CMP   #$10
   BNE   .if_end@11
   LDA   tiny+3
   CMP   #$4D
   BNE   .if_end@11
   LDA   tiny+2
   CMP   #$55
   BNE   .if_end@11
   LDA   tiny+1
   CMP   #$1D
   BNE   .if_end@11
   LDA   tiny
   CMP   #$69
   BNE   .if_end@11
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   LDX   #$00
   TXA
   RTS

main@run4:
   SUBROUTINE

   STA   HARGS+15
   JSR   dle
   LDA   HARGS+16
   RTS

main@run3:
   SUBROUTINE

   JSR   main@run0
   LDA   fifty_fiveE5+4
   STA   HARGS+12
   LDA   fifty_fiveE5+5
   STA   HARGS+13
   LDA   fifty_fiveE5+6
   STA   HARGS+14
   LDA   fifty_fiveE5+7
   STA   HARGS+15
   RTS

main@run2:
   SUBROUTINE

   LDA   #$F1
   STA   HARGS
   LDA   #$68
   STA   HARGS+1
   LDA   #$E3
   STA   HARGS+2
   LDA   #$88
   STA   HARGS+3
   LDA   #$B5
   STA   HARGS+4
   LDA   #$F8
   STA   HARGS+5
   RTS

main@run1:
   SUBROUTINE

   STA   HARGS+7
   LDA   tiny
   STA   HARGS+8
   LDA   tiny+1
   STA   HARGS+9
   LDA   tiny+2
   STA   HARGS+10
   LDA   tiny+3
   STA   HARGS+11
   LDA   tiny+4
   STA   HARGS+12
   LDA   tiny+5
   STA   HARGS+13
   LDA   tiny+6
   STA   HARGS+14
   LDA   tiny+7
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   RTS

main@run0:
   SUBROUTINE

   LDA   fifty_fourE4
   STA   HARGS
   LDA   fifty_fourE4+1
   STA   HARGS+1
   LDA   fifty_fourE4+2
   STA   HARGS+2
   LDA   fifty_fourE4+3
   STA   HARGS+3
   LDA   fifty_fourE4+4
   STA   HARGS+4
   LDA   fifty_fourE4+5
   STA   HARGS+5
   LDA   fifty_fourE4+6
   STA   HARGS+6
   LDA   fifty_fourE4+7
   STA   HARGS+7
   LDA   fifty_fiveE5
   STA   HARGS+8
   LDA   fifty_fiveE5+1
   STA   HARGS+9
   LDA   fifty_fiveE5+2
   STA   HARGS+10
   LDA   fifty_fiveE5+3
   STA   HARGS+11
   RTS

fifty_fiveE5:
   DC.L  $00000000
   DC.L  $4154FB18

fifty_fourE4:
   DC.L  $00000000
   DC.L  $41207AC0

tiny:
   DC.L  $88E368F1
   DC.L  $3F04F8B5

four:
   DC.L  $00000000
   DC.L  $40100000

point_one:
   DC.L  $9999999A
   DC.L  $3FB99999
