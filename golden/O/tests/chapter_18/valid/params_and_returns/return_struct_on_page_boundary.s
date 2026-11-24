SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__addr_%12	EQU	$82
__local_main__x	EQU	$8C

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__addr_%12,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__x,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21 indirect=false in_cycle=false
; def return_struct params= locals= indirect=false in_cycle=false
; call main -> return_struct
; item return_struct section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

return_struct:
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

   LDA   #$FF
   STA   on_page_boundary+9
   LDA   #$FE
   STA   on_page_boundary+8
   LDA   #$FD
   STA   on_page_boundary+7
   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   on_page_boundary
   LDY   #$00
   STA   (DPTR),Y
   LDA   on_page_boundary+1
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+2
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+3
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+4
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+5
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+6
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+7
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+8
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+9
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

   LDA   #<__local_main__addr_%12
   STA   __local_main__0
   LDA   #>__local_main__addr_%12
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
   JSR   return_struct
   LDA   __local_main__addr_%12
   STA   __local_main__x
   LDA   __local_main__addr_%12+1
   STA   __local_main__x+1
   LDA   __local_main__addr_%12+2
   STA   __local_main__x+2
   LDA   __local_main__addr_%12+3
   STA   __local_main__x+3
   LDA   __local_main__addr_%12+4
   STA   __local_main__x+4
   LDA   __local_main__addr_%12+5
   STA   __local_main__x+5
   LDA   __local_main__addr_%12+6
   STA   __local_main__x+6
   LDA   __local_main__addr_%12+7
   STA   __local_main__x+7
   LDA   __local_main__addr_%12+8
   STA   __local_main__x+8
   LDA   __local_main__addr_%12+9
   STA   __local_main__x+9
   LDY   #$00
   LDX   #$00
.loop@0_start:
   TYA
   SEC
   SBC   #$07
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_main__x,Y
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   INY
   BNE   .loop@0_start
   INX
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_main__x+7
   STA   __local_main__0
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_main__0
   CMP   #$FD
   BNE   .jcmp_skip@2
   LDA   __local_main__x+8
   STA   __local_main__0
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_main__0
   CMP   #$FE
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   LDA   __local_main__x+9
   STA   __local_main__0
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   __local_main__0
   CMP   #$FF
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
