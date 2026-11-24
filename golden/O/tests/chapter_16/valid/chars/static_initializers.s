SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item from_long section=.bss local=false zero=true
; item from_double section=.data local=false zero=false
; item from_uint section=.data local=false zero=false
; item from_ulong section=.data local=false zero=false
; item schar_from_long section=.data local=false zero=false
; item schar_from_uint section=.data local=false zero=false
; item schar_from_ulong section=.data local=false zero=false
; item schar_from_double section=.bss local=false zero=true
; item uchar_from_int section=.data local=false zero=false
; item uchar_from_uint section=.data local=false zero=false
; item uchar_from_long section=.data local=false zero=false
; item uchar_from_ulong section=.data local=false zero=false
; item uchar_from_double section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   from_long
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   from_double
   CMP   #$0F
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   from_uint
   BPL   .jcmp_skip@2
   CMP   #$81
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   from_ulong
   CMP   #$0A
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   schar_from_uint
   BPL   .jcmp_skip@5
   CMP   #$FA
   BEQ   .if_end@4
.jcmp_skip@5:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   schar_from_ulong
   CMP   #$0A
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   schar_from_double
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   ORA   schar_from_double
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   uchar_from_int
   CMP   #$D6
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   uchar_from_uint
   CMP   #$FA
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDA   uchar_from_ulong
   CMP   #$0A
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   LDA   uchar_from_double
   CMP   #$4D
   BEQ   .if_end@10
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   LDA   schar_from_long
   CMP   #$03
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   LDA   uchar_from_long
   CMP   #$FA
   BEQ   .if_end@12
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@12:
   LDX   #$00
   TXA
   RTS

from_long:
   DS.B  1

from_double:
   DC.B  $0F

from_uint:
   DC.B  $81

from_ulong:
   DC.B  $0A

schar_from_long:
   DC.B  $03

schar_from_uint:
   DC.B  $FA

schar_from_ulong:
   DC.B  $0A

schar_from_double:
   DS.B  1

uchar_from_int:
   DC.B  $D6

uchar_from_uint:
   DC.B  $FA

uchar_from_long:
   DC.B  $FA

uchar_from_ulong:
   DC.B  $0A

uchar_from_double:
   DC.B  $4D
