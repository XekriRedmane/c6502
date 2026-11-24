SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__addr_%3	EQU	$86
__local_main__addr_%9	EQU	$8C
__local_main__addr_%15	EQU	$92
__local_f__retval_4	EQU	$99
__local_f__retval_2	EQU	$9B

; @zp-link-meta-begin
; def f params= locals=__local_f__retval_5,__local_f__retval_4,__local_f__retval_3,__local_f__retval_2,__local_f__retval_1,__local_f__retval_0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__addr_%3,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__addr_%9,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__addr_%15,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23 indirect=false in_cycle=false
; call main -> f
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
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

   STY   __local_f__retval_2
   INY
   STY   __local_f__retval_4
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
   LDA   __local_f__retval_2
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   __local_f__retval_4
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

main:
   SUBROUTINE

   LDA   #<__local_main__addr_%3
   STA   __local_main__0
   LDA   #>__local_main__addr_%3
   STA   __local_main__1
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   __local_main__0
   LDY   #$01
   STA   (SSP),Y
   LDA   __local_main__1
   INY
   STA   (SSP),Y
   JSR   f
   LDA   __local_main__addr_%3
   STA   __local_main__5
   LDA   __local_main__addr_%3+1
   STA   __local_main__4
   LDA   #<__local_main__addr_%9
   STA   __local_main__0
   LDA   #>__local_main__addr_%9
   STA   __local_main__1
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   __local_main__0
   LDY   #$01
   STA   (SSP),Y
   LDA   __local_main__1
   INY
   STA   (SSP),Y
   JSR   f
   LDA   __local_main__addr_%9+2
   STA   __local_main__3
   LDA   __local_main__addr_%9+3
   STA   __local_main__2
   LDA   #<__local_main__addr_%15
   STA   __local_main__0
   LDA   #>__local_main__addr_%15
   STA   __local_main__1
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   __local_main__0
   LDY   #$01
   STA   (SSP),Y
   LDA   __local_main__1
   INY
   STA   (SSP),Y
   JSR   f
   LDA   __local_main__addr_%15+4
   STA   __local_main__1
   LDA   __local_main__addr_%15+5
   STA   __local_main__0
   LDA   __local_main__4
   BNE   .jcmp_skip@0
   LDA   __local_main__5
   CMP   #$01
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_main__2
   BNE   .jcmp_skip@1
   LDA   __local_main__3
   CMP   #$02
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_main__0
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$03
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS
