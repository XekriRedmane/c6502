SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__4	EQU	$84
__local_main__5	EQU	$85

; @zp-link-meta-begin
; def get_ui_ptr params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; call main -> get_ui_ptr
; item get_ui_ptr section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; item ui section=.data local=false zero=false
; item shiftcount section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

get_ui_ptr:
   SUBROUTINE

   LDX   #>ui
   LDA   #<ui
   RTS

main:
   SUBROUTINE

   JSR   get_ui_ptr
   STA   __local_main__4
   STX   __local_main__5
   LDY   #$00
   LDA   (__local_main__4),Y
   STA   __local_main__0
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__4),Y
   ASL   __local_main__0
   ROL   __local_main__1
   ROL   __local_main__2
   ROL
   ASL   __local_main__0
   ROL   __local_main__1
   ROL   __local_main__2
   ROL
   BNE   .jcmp_skip@0
   LDA   __local_main__2
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_main__0
   CMP   #$FC
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   get_ui_ptr
   STA   __local_main__4
   STX   __local_main__5
   LDY   #$00
   LDA   (__local_main__4),Y
   STA   __local_main__0
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__4),Y
   LSR
   ROR   __local_main__2
   ROR   __local_main__1
   ROR   __local_main__0
   LSR
   ROR   __local_main__2
   ROR   __local_main__1
   ROR   __local_main__0
   CMP   #$00
   BNE   .jcmp_skip@1
   LDA   __local_main__2
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$3F
   BNE   .jcmp_skip@1
   LDA   __local_main__0
   CMP   #$FF
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   main@run0
   LDA   shiftcount
   STA   __local_main__4
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+8
   BNE   .jcmp_skip@2
   LDA   HARGS+7
   BNE   .jcmp_skip@2
   LDA   HARGS+6
   BNE   .jcmp_skip@2
   LDA   HARGS+5
   CMP   #$1F
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   main@run0
   LDA   __local_main__4
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+8
   BNE   .jcmp_skip@3
   LDA   HARGS+7
   BNE   .jcmp_skip@3
   LDA   HARGS+6
   CMP   #$7D
   BNE   .jcmp_skip@3
   LDA   HARGS+5
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   LDA   #$E8
   STA   HARGS
   LDA   #$03
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   RTS

ui:
   DC.L  $0000FFFF

shiftcount:
   DC.W  $0005
