SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__2	EQU	$82
__local_main__multi_dim	EQU	$86

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__multi_dim,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDY   #$05
   LDA   #>__local_main__multi_dim
   CMP   #>__local_main__multi_dim
   BNE   .jcmp_skip@0
   LDA   #<__local_main__multi_dim
   CMP   #<__local_main__multi_dim
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #>(__local_main__multi_dim+6)
   STA   __local_main__2
   LDA   #$00
   BNE   .jcmp_skip@1
   LDA   #$04
   CMP   #$04
   BEQ   .vthread@0
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.vthread@0:
   CPY   #$05
   BEQ   .if_end@3
   LDX   #$00
   RTS
.if_end@3:
   LDA   #<(__local_main__multi_dim+6)
   SEC
   SBC   #$06
   STA   __local_main__0
   BCS   .carry_inc@0
   DEC   __local_main__2
.carry_inc@0:
   LDA   __local_main__2
   CMP   #>__local_main__multi_dim
   BNE   .jcmp_skip@4
   LDA   __local_main__0
   CMP   #<__local_main__multi_dim
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   TYA
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS
