SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_target__a	EQU	$82
__local_target__stat_ptr	EQU	$83
__local_target__0	EQU	$84

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params=__zpabi_target__stat_ptr_0,__zpabi_target__stat_ptr_1 locals=__local_target__a,__local_target__stat_ptr,__local_target__0 indirect=false in_cycle=false param_regs=A,X
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item stat section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   STA   __local_target__stat_ptr
   STX   __local_target__0
   LDA   stat
   STA   __local_target__a
   LDX   stat+1
   LDA   #$08
   LDY   #$00
   STA   (__local_target__stat_ptr),Y
   LDA   #$00
   INY
   STA   (__local_target__stat_ptr),Y
   LDA   __local_target__a
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   stat
   LDA   #$00
   STA   stat+1
   LDX   #>stat
   LDA   #<stat
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$05
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   stat+1
   BNE   .jcmp_skip@1
   LDA   stat
   CMP   #$08
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

stat:
   DS.B  2
