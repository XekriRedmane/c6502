SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__i,__local_main__7,__local_main__8,__local_main__9,__local_main__l,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__d,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDY   #$99
   LDA   #$00
   BNE   .jcmp_skip@0
   LDA   #$0A
   CMP   #$0A
   BEQ   .vthread@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.vthread@1:
   CPY   #$99
   BNE   .jcmp_skip@2
   LDA   #$9A
   CMP   #$9A
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS
