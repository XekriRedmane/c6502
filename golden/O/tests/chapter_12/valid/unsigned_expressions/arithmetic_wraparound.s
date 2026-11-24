SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_addition__1	EQU	$83
__local_neg__1	EQU	$83
__local_subtraction__1	EQU	$83
__local_addition__2	EQU	$84
__local_neg__2	EQU	$84
__local_subtraction__2	EQU	$84
__local_addition__3	EQU	$85
__local_neg__3	EQU	$85
__local_subtraction__3	EQU	$85
__local_neg__4	EQU	$86
__local_subtraction__4	EQU	$86
__local_neg__5	EQU	$87
__local_subtraction__5	EQU	$87
__local_neg__6	EQU	$88
__local_subtraction__6	EQU	$88
__local_neg__7	EQU	$89
__local_subtraction__7	EQU	$89

; @zp-link-meta-begin
; def addition params= locals=__local_addition__0,__local_addition__1,__local_addition__2,__local_addition__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def neg params= locals=__local_neg__0,__local_neg__1,__local_neg__2,__local_neg__3,__local_neg__4,__local_neg__5,__local_neg__6,__local_neg__7 indirect=false in_cycle=false
; def subtraction params= locals=__local_subtraction__0,__local_subtraction__1,__local_subtraction__2,__local_subtraction__3,__local_subtraction__4,__local_subtraction__5,__local_subtraction__6,__local_subtraction__7 indirect=false in_cycle=false
; call main -> addition
; call main -> neg
; call main -> subtraction
; item addition section=.text local=false zero=false
; item subtraction section=.text local=false zero=false
; item neg section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; item ui_a section=.bss local=false zero=true
; item ui_b section=.bss local=false zero=true
; item ul_a section=.bss local=false zero=true
; item ul_b section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

addition:
   SUBROUTINE

   LDA   ui_a
   CLC
   ADC   ui_b
   STA   __local_addition__3
   LDA   ui_a+1
   ADC   ui_b+1
   STA   __local_addition__2
   LDA   ui_a+2
   ADC   ui_b+2
   STA   __local_addition__1
   LDA   ui_a+3
   ADC   ui_b+3
   BNE   .thread@0
   LDA   __local_addition__1
   BNE   .thread@0
   LDA   __local_addition__2
   BNE   .thread@0
   LDA   __local_addition__3
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

subtraction:
   SUBROUTINE

   LDA   ul_a
   SEC
   SBC   ul_b
   STA   __local_subtraction__7
   LDA   ul_a+1
   SBC   ul_b+1
   STA   __local_subtraction__6
   LDA   ul_a+2
   SBC   ul_b+2
   STA   __local_subtraction__5
   LDA   ul_a+3
   SBC   ul_b+3
   STA   __local_subtraction__4
   LDA   ul_a+4
   SBC   ul_b+4
   STA   __local_subtraction__3
   LDA   ul_a+5
   SBC   ul_b+5
   STA   __local_subtraction__2
   LDA   ul_a+6
   SBC   ul_b+6
   STA   __local_subtraction__1
   LDA   ul_a+7
   SBC   ul_b+7
   BNE   .thread@1
   LDA   __local_subtraction__1
   BNE   .thread@1
   LDA   __local_subtraction__2
   BNE   .thread@1
   LDA   __local_subtraction__3
   BNE   .thread@1
   LDA   __local_subtraction__4
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_subtraction__5
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_subtraction__6
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_subtraction__7
   CMP   #$F6
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

neg:
   SUBROUTINE

   SEC
   LDA   #$00
   SBC   ul_a
   STA   __local_neg__7
   LDA   #$00
   SBC   ul_a+1
   STA   __local_neg__6
   LDA   #$00
   SBC   ul_a+2
   STA   __local_neg__5
   LDA   #$00
   SBC   ul_a+3
   STA   __local_neg__4
   LDA   #$00
   SBC   ul_a+4
   STA   __local_neg__3
   LDA   #$00
   SBC   ul_a+5
   STA   __local_neg__2
   LDA   #$00
   SBC   ul_a+6
   STA   __local_neg__1
   LDA   #$00
   SBC   ul_a+7
   BNE   .thread@2
   LDA   __local_neg__1
   BNE   .thread@2
   LDA   __local_neg__2
   BNE   .thread@2
   LDA   __local_neg__3
   BNE   .thread@2
   LDA   __local_neg__4
   CMP   #$FF
   BNE   .thread@2
   LDA   __local_neg__5
   CMP   #$FF
   BNE   .thread@2
   LDA   __local_neg__6
   CMP   #$FF
   BNE   .thread@2
   LDA   __local_neg__7
   CMP   #$FF
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

   LDA   #$FD
   STA   ui_a
   LDA   #$FF
   STA   ui_a+1
   LDA   #$03
   STA   ui_b
   LDA   #$00
   STA   ui_a+2
   STA   ui_a+3
   STA   ui_b+1
   STA   ui_b+2
   STA   ui_b+3
   JSR   addition
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$0A
   STA   ul_a
   LDA   #$14
   STA   ul_b
   LDA   #$00
   STA   ul_a+1
   JSR   main@run0
   STA   ul_b+1
   STA   ul_b+2
   STA   ul_b+3
   STA   ul_b+4
   STA   ul_b+5
   STA   ul_b+6
   STA   ul_b+7
   JSR   subtraction
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$01
   STA   ul_a
   LDA   #$00
   STA   ul_a+1
   JSR   main@run0
   JSR   neg
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

main@run0:
   SUBROUTINE

   STA   ul_a+2
   STA   ul_a+3
   STA   ul_a+4
   STA   ul_a+5
   STA   ul_a+6
   STA   ul_a+7
   RTS

ui_a:
   DS.B  4

ui_b:
   DS.B  4

ul_a:
   DS.B  8

ul_b:
   DS.B  8
