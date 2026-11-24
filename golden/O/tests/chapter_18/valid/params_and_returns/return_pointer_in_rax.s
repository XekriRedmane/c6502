SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_return_in_mem__result_9	EQU	$82
__local_return_in_mem__result_8	EQU	$83
__local_return_in_mem__result_5	EQU	$86
__local_return_in_mem__result_4	EQU	$87
__local_return_in_mem__result_2	EQU	$89

; @zp-link-meta-begin
; def return_in_mem params= locals=__local_return_in_mem__result_11,__local_return_in_mem__result_10,__local_return_in_mem__result_9,__local_return_in_mem__result_8,__local_return_in_mem__result_7,__local_return_in_mem__result_6,__local_return_in_mem__result_5,__local_return_in_mem__result_4,__local_return_in_mem__result_3,__local_return_in_mem__result_2,__local_return_in_mem__result_1,__local_return_in_mem__result_0 indirect=false in_cycle=false
; item return_in_mem section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

return_in_mem:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
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

   LDY   #$00
   STY   __local_return_in_mem__result_2
   LDY   #$02
   STY   __local_return_in_mem__result_4
   LDY   #$00
   STY   __local_return_in_mem__result_5
   LDY   #$03
   STY   __local_return_in_mem__result_8
   LDY   #$00
   STY   __local_return_in_mem__result_9
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   __local_return_in_mem__result_2
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   __local_return_in_mem__result_4
   INY
   STA   (DPTR),Y
   LDA   __local_return_in_mem__result_5
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   __local_return_in_mem__result_8
   INY
   STA   (DPTR),Y
   LDA   __local_return_in_mem__result_9
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
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
