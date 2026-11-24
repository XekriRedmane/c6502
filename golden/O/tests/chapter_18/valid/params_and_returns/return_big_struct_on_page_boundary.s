SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__addr_%15	EQU	$82
__local_main__x	EQU	$94

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__addr_%15,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__x,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37 indirect=false in_cycle=false
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

   LDA   #$0C
   STA   on_page_boundary+17
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
   LDA   on_page_boundary+10
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+11
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+12
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+13
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+14
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+15
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+16
   INY
   STA   (DPTR),Y
   LDA   on_page_boundary+17
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
   JSR   return_struct
   LDA   __local_main__addr_%15
   STA   __local_main__x
   LDA   __local_main__addr_%15+1
   STA   __local_main__x+1
   LDA   __local_main__addr_%15+2
   STA   __local_main__x+2
   LDA   __local_main__addr_%15+3
   STA   __local_main__x+3
   LDA   __local_main__addr_%15+4
   STA   __local_main__x+4
   LDA   __local_main__addr_%15+5
   STA   __local_main__x+5
   LDA   __local_main__addr_%15+6
   STA   __local_main__x+6
   LDA   __local_main__addr_%15+7
   STA   __local_main__x+7
   LDA   __local_main__addr_%15+8
   STA   __local_main__x+8
   LDA   __local_main__addr_%15+9
   STA   __local_main__x+9
   LDA   __local_main__addr_%15+10
   STA   __local_main__x+10
   LDA   __local_main__addr_%15+11
   STA   __local_main__x+11
   LDA   __local_main__addr_%15+12
   STA   __local_main__x+12
   LDA   __local_main__addr_%15+13
   STA   __local_main__x+13
   LDA   __local_main__addr_%15+14
   STA   __local_main__x+14
   LDA   __local_main__addr_%15+15
   STA   __local_main__x+15
   LDA   __local_main__addr_%15+16
   STA   __local_main__x+16
   LDA   __local_main__addr_%15+17
   STA   __local_main__x+17
   LDY   #$00
   LDX   #$00
.loop@0_start:
   TYA
   SEC
   SBC   #$12
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_main__x,Y
   STA   __local_main__0
   CPX   #$00
   BNE   .if_else@1
   CPY   #$07
   BNE   .if_else@1
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   __local_main__0
   CMP   #$FD
   BEQ   .if_end@0
.jcmp_skip@1:
   LDX   #$00
   LDA   #$01
   RTS
.if_else@1:
   CPX   #$00
   BNE   .if_else@4
   CPY   #$08
   BNE   .if_else@4
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_main__0
   CMP   #$FE
   BEQ   .if_end@3
.jcmp_skip@2:
   LDX   #$00
   LDA   #$02
   RTS
.if_else@4:
   CPX   #$00
   BNE   .if_else@7
   CPY   #$09
   BNE   .if_else@7
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   __local_main__0
   CMP   #$FF
   BEQ   .if_end@6
.jcmp_skip@3:
   LDX   #$00
   LDA   #$03
   RTS
.if_else@7:
   CPX   #$00
   BNE   .if_else@10
   CPY   #$11
   BNE   .if_else@10
   CMP   #$0C
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$04
   RTS
.if_else@10:
   ORA   #$00
   BEQ   .if_end@12
   LDX   #$00
   LDA   #$05
   RTS
.if_end@12:
.if_end@9:
.if_end@6:
.if_end@3:
.if_end@0:
   INY
   BNE   .loop@0_start
   INX
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$00
   TXA
   RTS
