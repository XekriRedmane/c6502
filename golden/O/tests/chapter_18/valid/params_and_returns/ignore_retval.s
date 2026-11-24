SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__addr_%22	EQU	$88
__local_main__addr_%24	EQU	$8A
__local_main__addr_%29	EQU	$8C
__local_main__addr_%31	EQU	$9A
__local_return_in_mem__0	EQU	$A8
__local_return_in_mem__1	EQU	$A9
__local_return_in_mem__2	EQU	$AA
__local_return_in_mem__3	EQU	$AB

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__addr_%22,__local_main__9,__local_main__addr_%24,__local_main__11,__local_main__addr_%29,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__addr_%31,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37,__local_main__38,__local_main__39 indirect=false in_cycle=false
; def return_in_mem params= locals=__local_return_in_mem__0,__local_return_in_mem__1,__local_return_in_mem__2,__local_return_in_mem__3,__local_return_in_mem__4,__local_return_in_mem__5,__local_return_in_mem__6,__local_return_in_mem__7 indirect=false in_cycle=false
; def return_in_reg params= locals= indirect=false in_cycle=false
; call main -> return_in_mem
; call main -> return_in_reg
; item return_in_reg section=.text local=false zero=false
; item return_in_mem section=.text local=false zero=false
; item main section=.text local=false zero=false
; item globl section=.bss local=false zero=true
; item globl2 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

return_in_reg:
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

   INC   globl
   BNE   .inc_done@0
   INC   globl+1
.inc_done@0:
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globl
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globl+1
   LDY   #$01
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

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

   LDA   globl2
   STA   HARGS
   LDA   globl2+1
   STA   HARGS+1
   LDA   globl2+2
   STA   HARGS+2
   LDA   globl2+3
   STA   HARGS+3
   LDA   globl2+4
   STA   HARGS+4
   LDA   globl2+5
   STA   HARGS+5
   LDA   globl2+6
   STA   HARGS+6
   LDA   globl2+7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dmul
   LDA   HARGS+16
   STA   globl2
   LDA   HARGS+17
   STA   globl2+1
   LDA   HARGS+18
   STA   globl2+2
   LDA   HARGS+19
   STA   globl2+3
   LDA   HARGS+20
   STA   globl2+4
   LDA   HARGS+21
   STA   globl2+5
   LDA   HARGS+22
   STA   globl2+6
   LDA   HARGS+23
   STA   globl2+7
   LDA   globl2+8
   STA   __local_return_in_mem__3
   LDA   globl2+9
   STA   __local_return_in_mem__2
   LDA   __local_return_in_mem__3
   ASL
   STA   __local_return_in_mem__0
   LDA   __local_return_in_mem__2
   ROL
   STA   __local_return_in_mem__1
   LDA   __local_return_in_mem__0
   CLC
   ADC   __local_return_in_mem__3
   STA   globl2+8
   LDA   __local_return_in_mem__1
   ADC   __local_return_in_mem__2
   STA   globl2+9
   LDA   globl2+10
   STA   __local_return_in_mem__3
   LDA   globl2+11
   STA   __local_return_in_mem__2
   LDA   globl2+12
   STA   __local_return_in_mem__1
   LDA   globl2+13
   STA   __local_return_in_mem__0
   LDA   __local_return_in_mem__3
   ASL
   STA   globl2+10
   LDA   __local_return_in_mem__2
   ROL
   STA   globl2+11
   LDA   __local_return_in_mem__1
   ROL
   STA   globl2+12
   LDA   __local_return_in_mem__0
   ROL
   ASL   globl2+10
   ROL   globl2+11
   ROL   globl2+12
   ROL
   STA   globl2+13
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globl2
   LDY   #$00
   STA   (DPTR),Y
   LDA   globl2+1
   INY
   STA   (DPTR),Y
   LDA   globl2+2
   INY
   STA   (DPTR),Y
   LDA   globl2+3
   INY
   STA   (DPTR),Y
   LDA   globl2+4
   INY
   STA   (DPTR),Y
   LDA   globl2+5
   INY
   STA   (DPTR),Y
   LDA   globl2+6
   INY
   STA   (DPTR),Y
   LDA   globl2+7
   INY
   STA   (DPTR),Y
   LDA   globl2+8
   INY
   STA   (DPTR),Y
   LDA   globl2+9
   INY
   STA   (DPTR),Y
   LDA   globl2+10
   INY
   STA   (DPTR),Y
   LDA   globl2+11
   INY
   STA   (DPTR),Y
   LDA   globl2+12
   INY
   STA   (DPTR),Y
   LDA   globl2+13
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

   LDA   #<__local_main__addr_%22
   STA   __local_main__0
   LDA   #>__local_main__addr_%22
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
   JSR   return_in_reg
   LDA   #<__local_main__addr_%24
   STA   __local_main__0
   LDA   #>__local_main__addr_%24
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
   JSR   return_in_reg
   LDA   globl+1
   BNE   .jcmp_skip@0
   LDA   globl
   CMP   #$02
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #<__local_main__addr_%29
   STA   __local_main__0
   LDA   #>__local_main__addr_%29
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
   JSR   return_in_mem
   LDA   #<__local_main__addr_%31
   STA   __local_main__0
   LDA   #>__local_main__addr_%31
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
   JSR   return_in_mem
   LDA   globl2+7
   CMP   #$40
   BNE   .main@jthread@1
   LDA   globl2+6
   CMP   #$14
   BNE   .main@jthread@1
   LDA   globl2+5
   BNE   .main@jthread@1
   LDA   globl2+4
   BNE   .main@jthread@1
   LDA   globl2+3
   BNE   .main@jthread@1
   LDA   globl2+2
   BNE   .main@jthread@1
   LDA   globl2+1
   BNE   .main@jthread@1
   LDA   globl2
   BNE   .main@jthread@1
   LDA   globl2+9
   BNE   .main@jthread@1
   LDA   globl2+8
   CMP   #$12
   BNE   .main@jthread@1
   LDA   globl2+13
   BNE   .main@jthread@1
   LDA   globl2+12
   BNE   .main@jthread@1
   LDA   globl2+11
   CMP   #$12
   BNE   .main@jthread@1
   LDA   globl2+10
   CMP   #$C0
   BEQ   .if_end@5
.main@jthread@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS

globl:
   DS.B  2

globl2:
   DC.L  $00000000
   DC.L  $3FF40000
   DC.W  $0002
   DC.L  $0000012C
