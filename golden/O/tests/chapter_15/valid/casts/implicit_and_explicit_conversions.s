SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__arr	EQU	$88

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__arr,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37,__local_main__38,__local_main__39 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDY   #$FF
   LDA   #>__local_main__arr
   CMP   #>__local_main__arr
   BNE   .jcmp_skip@0
   LDA   #<__local_main__arr
   CMP   #<__local_main__arr
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #>__local_main__arr
   CMP   #>__local_main__arr
   BNE   .jcmp_skip@1
   LDA   #<__local_main__arr
   CMP   #<__local_main__arr
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   TYA
   BNE   .jcmp_skip@2
   CMP   #$FF
   BNE   .jcmp_skip@2
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   TYA
   BNE   .jcmp_skip@3
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   #$FC
   CMP   #$FC
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
