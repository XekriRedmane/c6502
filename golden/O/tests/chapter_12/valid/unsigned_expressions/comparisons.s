SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run7 section=.text local=true zero=false
; item main@run6 section=.text local=true zero=false
; item main@run5 section=.text local=true zero=false
; item main@run4 section=.text local=true zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item one_hundred section=.data local=false zero=false
; item large_uint section=.data local=false zero=false
; item one_hundred_ulong section=.data local=false zero=false
; item large_ulong section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   JSR   main@run4
   BCS   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   one_hundred
   JSR   main@run3
   BCC   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   main@run5
   BCC   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   main@run4
   BCS   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   large_uint
   JSR   main@run2
   BCS   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   one_hundred
   JSR   main@run3
   BCC   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   JSR   main@run5
   BCC   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   JSR   main@run4
   BCS   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   large_ulong
   JSR   main@run0
   BCS   .if_end@8
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDA   one_hundred_ulong
   JSR   main@run1
   BCC   .if_end@9
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   JSR   main@run6
   BCC   .if_end@10
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   JSR   main@run7
   BCS   .if_end@11
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   LDA   large_ulong
   JSR   main@run0
   BCS   .if_end@12
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@12:
   LDA   one_hundred_ulong
   JSR   main@run1
   BCC   .if_end@13
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@13:
   JSR   main@run6
   BCC   .if_end@14
   LDX   #$00
   LDA   #$0F
   RTS
.if_end@14:
   JSR   main@run7
   BCS   .if_end@15
   LDX   #$00
   LDA   #$10
   RTS
.if_end@15:
   LDX   #$00
   TXA
   RTS

main@run7:
   SUBROUTINE

   LDA   large_ulong
   SEC
   JMP   main@run0

main@run6:
   SUBROUTINE

   LDA   one_hundred_ulong
   SEC
   JMP   main@run1

main@run5:
   SUBROUTINE

   LDA   one_hundred
   SEC
   JMP   main@run3

main@run4:
   SUBROUTINE

   LDA   large_uint
   SEC
   JMP   main@run2

main@run3:
   SUBROUTINE

   SBC   large_uint
   LDA   one_hundred+1
   SBC   large_uint+1
   LDA   one_hundred+2
   SBC   large_uint+2
   LDA   one_hundred+3
   SBC   large_uint+3
   RTS

main@run2:
   SUBROUTINE

   SBC   one_hundred
   LDA   large_uint+1
   SBC   one_hundred+1
   LDA   large_uint+2
   SBC   one_hundred+2
   LDA   large_uint+3
   SBC   one_hundred+3
   RTS

main@run1:
   SUBROUTINE

   SBC   large_ulong
   LDA   one_hundred_ulong+1
   SBC   large_ulong+1
   LDA   one_hundred_ulong+2
   SBC   large_ulong+2
   LDA   one_hundred_ulong+3
   SBC   large_ulong+3
   LDA   one_hundred_ulong+4
   SBC   large_ulong+4
   LDA   one_hundred_ulong+5
   SBC   large_ulong+5
   LDA   one_hundred_ulong+6
   SBC   large_ulong+6
   LDA   one_hundred_ulong+7
   SBC   large_ulong+7
   RTS

main@run0:
   SUBROUTINE

   SBC   one_hundred_ulong
   LDA   large_ulong+1
   SBC   one_hundred_ulong+1
   LDA   large_ulong+2
   SBC   one_hundred_ulong+2
   LDA   large_ulong+3
   SBC   one_hundred_ulong+3
   LDA   large_ulong+4
   SBC   one_hundred_ulong+4
   LDA   large_ulong+5
   SBC   one_hundred_ulong+5
   LDA   large_ulong+6
   SBC   one_hundred_ulong+6
   LDA   large_ulong+7
   SBC   one_hundred_ulong+7
   RTS

one_hundred:
   DC.L  $00000064

large_uint:
   DC.L  $0000FFFE

one_hundred_ulong:
   DC.L  $00000064
   DC.L  $00000000

large_ulong:
   DC.L  $FFFFFFFE
   DC.L  $00000000
