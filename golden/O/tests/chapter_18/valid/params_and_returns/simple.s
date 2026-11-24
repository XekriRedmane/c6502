SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__result_11	EQU	$80
__local_main__result_10	EQU	$81
__local_main__result_9	EQU	$82
__local_main__result_8	EQU	$83
__local_main__result_7	EQU	$84
__local_main__result_6	EQU	$85
__local_main__result_5	EQU	$86
__local_main__result_4	EQU	$87
__local_main__result_3	EQU	$88
__local_main__result_2	EQU	$89
__local_main__result_1	EQU	$8A
__local_main__result_0	EQU	$8B
__local_main__addr_%15	EQU	$8C
__local_double_members__retval_11	EQU	$98
__local_double_members__retval_10	EQU	$99
__local_double_members__retval_9	EQU	$9A
__local_double_members__retval_8	EQU	$9B
__local_double_members__retval_7	EQU	$9C
__local_double_members__retval_6	EQU	$9D
__local_double_members__retval_5	EQU	$9E
__local_double_members__retval_4	EQU	$9F
__local_double_members__retval_3	EQU	$A0
__local_double_members__retval_2	EQU	$A1
__local_double_members__retval_1	EQU	$A2
__local_double_members__retval_0	EQU	$A3

; @zp-link-meta-begin
; def double_members params= locals=__local_double_members__retval_11,__local_double_members__retval_10,__local_double_members__retval_9,__local_double_members__retval_8,__local_double_members__retval_7,__local_double_members__retval_6,__local_double_members__retval_5,__local_double_members__retval_4,__local_double_members__retval_3,__local_double_members__retval_2,__local_double_members__retval_1,__local_double_members__retval_0 indirect=false in_cycle=false
; def main params= locals=__local_main__result_11,__local_main__result_10,__local_main__result_9,__local_main__result_8,__local_main__result_7,__local_main__result_6,__local_main__result_5,__local_main__result_4,__local_main__result_3,__local_main__result_2,__local_main__result_1,__local_main__result_0,__local_main__addr_%15,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; call main -> double_members
; item double_members section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

double_members:
   SUBROUTINE

   ; prologue: 5 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   CLC
   LDA   FP
   ADC   #$05
   STA   __local_double_members__retval_9
   LDA   FP+1
   ADC   #$00
   STA   __local_double_members__retval_9+1
   LDY   #$00
   LDA   (__local_double_members__retval_9),Y
   STA   __local_double_members__retval_11
   INY
   LDA   (__local_double_members__retval_9),Y
   ASL   __local_double_members__retval_11
   ROL
   STA   __local_double_members__retval_10
   STA   HARGS+1
   LDA   __local_double_members__retval_11
   STA   HARGS
   JSR   i2d
   LDA   HARGS+2
   STA   __local_double_members__retval_0
   LDA   HARGS+3
   STA   __local_double_members__retval_1
   LDA   HARGS+4
   STA   __local_double_members__retval_2
   LDA   HARGS+5
   STA   __local_double_members__retval_3
   LDA   HARGS+6
   STA   __local_double_members__retval_4
   LDA   HARGS+7
   STA   __local_double_members__retval_5
   LDA   HARGS+8
   STA   __local_double_members__retval_6
   LDA   HARGS+9
   STA   __local_double_members__retval_7
   CLC
   LDA   FP
   ADC   #$05
   STA   __local_double_members__retval_11
   LDA   FP+1
   ADC   #$00
   STA   __local_double_members__retval_11+1
   LDA   __local_double_members__retval_11
   STA   DPTR
   LDA   __local_double_members__retval_10
   STA   DPTR+1
   LDY   #$02
   LDA   (__local_double_members__retval_11),Y
   ASL
   STA   __local_double_members__retval_8
   LDA   #$00
   ROL
   STA   __local_double_members__retval_9
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_double_members__retval_10
   STA   __local_double_members__retval_11
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_double_members__retval_0
   LDY   #$00
   STA   (DPTR),Y
   LDA   __local_double_members__retval_1
   INY
   STA   (DPTR),Y
   LDA   __local_double_members__retval_2
   INY
   STA   (DPTR),Y
   LDA   __local_double_members__retval_3
   INY
   STA   (DPTR),Y
   LDA   __local_double_members__retval_4
   INY
   STA   (DPTR),Y
   LDA   __local_double_members__retval_5
   INY
   STA   (DPTR),Y
   LDA   __local_double_members__retval_6
   INY
   STA   (DPTR),Y
   LDA   __local_double_members__retval_7
   INY
   STA   (DPTR),Y
   LDA   __local_double_members__retval_8
   INY
   STA   (DPTR),Y
   LDA   __local_double_members__retval_9
   INY
   STA   (DPTR),Y
   LDA   __local_double_members__retval_10
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$07
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__result_7
   LDA   #$00
   STA   __local_main__result_8
   LDA   #$04
   STA   __local_main__result_9
   LDA   #<__local_main__addr_%15
   STA   __local_main__result_11
   LDA   #>__local_main__addr_%15
   STA   __local_main__result_10
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   __local_main__result_11
   LDY   #$01
   STA   (SSP),Y
   LDA   __local_main__result_10
   INY
   STA   (SSP),Y
   LDA   __local_main__result_7
   INY
   STA   (SSP),Y
   LDA   __local_main__result_8
   INY
   STA   (SSP),Y
   LDA   __local_main__result_9
   INY
   STA   (SSP),Y
   JSR   double_members
   LDA   __local_main__addr_%15
   STA   __local_main__result_0
   LDA   __local_main__addr_%15+1
   STA   __local_main__result_1
   LDA   __local_main__addr_%15+2
   STA   __local_main__result_2
   LDA   __local_main__addr_%15+3
   STA   __local_main__result_3
   LDA   __local_main__addr_%15+4
   STA   __local_main__result_4
   LDA   __local_main__addr_%15+5
   STA   __local_main__result_5
   LDA   __local_main__addr_%15+6
   STA   __local_main__result_6
   LDA   __local_main__addr_%15+8
   STA   __local_main__result_8
   LDA   __local_main__addr_%15+9
   STA   __local_main__result_9
   LDA   __local_main__addr_%15+10
   STA   __local_main__result_10
   LDA   __local_main__addr_%15+11
   STA   __local_main__result_11
   LDA   __local_main__addr_%15+7
   CMP   #$40
   BNE   .main@jthread@0
   LDA   __local_main__result_6
   BNE   .main@jthread@0
   LDA   __local_main__result_5
   BNE   .main@jthread@0
   LDA   __local_main__result_4
   BNE   .main@jthread@0
   LDA   __local_main__result_3
   BNE   .main@jthread@0
   LDA   __local_main__result_2
   BNE   .main@jthread@0
   LDA   __local_main__result_1
   BNE   .main@jthread@0
   LDA   __local_main__result_0
   BNE   .main@jthread@0
   LDA   __local_main__result_11
   BNE   .main@jthread@0
   LDA   __local_main__result_10
   BNE   .main@jthread@0
   LDA   __local_main__result_9
   BNE   .main@jthread@0
   LDA   __local_main__result_8
   CMP   #$08
   BEQ   .if_end@2
.main@jthread@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS
