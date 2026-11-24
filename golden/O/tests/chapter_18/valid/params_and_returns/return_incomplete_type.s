SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__val_3	EQU	$80
__local_main__val_2	EQU	$81
__local_main__val_1	EQU	$82
__local_main__val_0	EQU	$83
__local_main__arg_1	EQU	$84
__local_main__arg_0	EQU	$85
__local_main__addr_%2	EQU	$86
__local_increment_struct__0	EQU	$8A
__local_increment_struct__1	EQU	$8B
__local_increment_struct__2	EQU	$8C
__local_increment_struct__3	EQU	$8D

; @zp-link-meta-begin
; def increment_struct params= locals=__local_increment_struct__0,__local_increment_struct__1,__local_increment_struct__2,__local_increment_struct__3 indirect=false in_cycle=false
; def main params= locals=__local_main__val_3,__local_main__val_2,__local_main__val_1,__local_main__val_0,__local_main__arg_1,__local_main__arg_0,__local_main__addr_%2,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; call main -> increment_struct
; item main section=.text local=false zero=false
; item increment_struct section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__arg_0
   LDA   #$00
   STA   __local_main__arg_1
   LDY   #$02
   STY   __local_main__val_0
   STA   __local_main__val_1
   LDA   #<__local_main__addr_%2
   STA   __local_main__val_3
   LDA   #>__local_main__addr_%2
   STA   __local_main__val_2
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   __local_main__val_3
   DEY
   STA   (SSP),Y
   LDA   __local_main__val_2
   INY
   STA   (SSP),Y
   LDA   __local_main__arg_0
   INY
   STA   (SSP),Y
   LDA   __local_main__arg_1
   INY
   STA   (SSP),Y
   LDA   __local_main__val_0
   INY
   STA   (SSP),Y
   LDA   __local_main__val_1
   INY
   STA   (SSP),Y
   JSR   increment_struct
   LDA   __local_main__addr_%2
   STA   __local_main__val_0
   LDA   __local_main__addr_%2+2
   STA   __local_main__val_2
   LDA   __local_main__addr_%2+3
   STA   __local_main__val_3
   LDA   __local_main__addr_%2+1
   BNE   .main@jthread@0
   LDA   __local_main__val_0
   CMP   #$02
   BNE   .main@jthread@0
   LDA   __local_main__val_3
   BNE   .main@jthread@0
   LDA   __local_main__val_2
   CMP   #$03
   BEQ   .if_end@2
.main@jthread@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

increment_struct:
   SUBROUTINE

   ; prologue: 6 arg bytes, 0 local bytes
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
   STA   __local_increment_struct__1
   LDA   FP+1
   ADC   #$00
   STA   __local_increment_struct__1+1
   DEY
   LDA   (__local_increment_struct__1),Y
   STA   __local_increment_struct__3
   DEY
   LDA   (__local_increment_struct__1),Y
   CLC
   ADC   #$01
   STA   __local_increment_struct__2
   LDA   __local_increment_struct__3
   ADC   #$00
   STA   __local_increment_struct__3
   CLC
   LDA   FP
   ADC   #$05
   STA   __local_increment_struct__0
   LDA   FP+1
   ADC   #$00
   STA   __local_increment_struct__0+1
   LDA   __local_increment_struct__2
   STA   (__local_increment_struct__0),Y
   LDA   __local_increment_struct__3
   INY
   STA   (__local_increment_struct__0),Y
   CLC
   LDA   FP
   ADC   #$05
   STA   __local_increment_struct__1
   LDA   FP+1
   ADC   #$00
   STA   __local_increment_struct__1+1
   LDY   #$03
   LDA   (__local_increment_struct__1),Y
   STA   __local_increment_struct__3
   DEY
   LDA   (__local_increment_struct__1),Y
   CLC
   ADC   #$01
   STA   __local_increment_struct__2
   LDA   __local_increment_struct__3
   ADC   #$00
   STA   __local_increment_struct__3
   CLC
   LDA   FP
   ADC   #$05
   STA   __local_increment_struct__0
   LDA   FP+1
   ADC   #$00
   STA   __local_increment_struct__0+1
   LDA   __local_increment_struct__0
   STA   DPTR
   LDA   __local_increment_struct__1
   STA   DPTR+1
   LDA   __local_increment_struct__2
   STA   (__local_increment_struct__0),Y
   LDA   __local_increment_struct__3
   INY
   STA   (__local_increment_struct__0),Y
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   INY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$08
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
