SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__i,__local_main__15,__local_main__ul,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__d,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__i2,__local_main__33,__local_main__ul2,__local_main__35,__local_main__36,__local_main__37,__local_main__38,__local_main__39,__local_main__40,__local_main__41,__local_main__d2,__local_main__43,__local_main__44,__local_main__45,__local_main__46,__local_main__47,__local_main__48,__local_main__49 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   #$9C
   CMP   #$9C
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDY   #$00
   BNE   .jcmp_skip@1
   LDA   #$C0
   CMP   #$C0
   BNE   .jcmp_skip@1
   TYA
   BNE   .jcmp_skip@1
   BEQ   .if_end@7
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@7:
   LDY   #$66
   LDA   #$C0
   CMP   #$C0
   BNE   .jcmp_skip@8
   LDA   #$40
   CMP   #$40
   BNE   .jcmp_skip@8
   LDA   #$A6
   CMP   #$A6
   BNE   .jcmp_skip@8
   CPY   #$66
   BNE   .jcmp_skip@8
   BEQ   .if_end@8
.jcmp_skip@8:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDX   #$00
   TXA
   RTS
