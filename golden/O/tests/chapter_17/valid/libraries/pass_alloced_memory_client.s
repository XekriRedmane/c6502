SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_fill_100_bytes__byte_0	EQU	$80
__zpabi_fill_100_bytes__byte_1	EQU	$81
__local_main__0	EQU	$82
__local_main__1	EQU	$83
__local_main__i_1	EQU	$84
__local_main__i_0	EQU	$85
__local_main__2	EQU	$86
__local_main__3	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__i_1,__local_main__i_0,__local_main__2,__local_main__3 indirect=false in_cycle=false
; ext fill_100_bytes params=__zpabi_fill_100_bytes__pointer_0,__zpabi_fill_100_bytes__pointer_1,__zpabi_fill_100_bytes__byte_0,__zpabi_fill_100_bytes__byte_1 param_regs=A,X,,
; ext free_bytes params=__zpabi_free_bytes__ptr_0,__zpabi_free_bytes__ptr_1 param_regs=A,X
; ext get_100_zeroed_bytes params=
; call main -> fill_100_bytes
; call main -> free_bytes
; call main -> get_100_zeroed_bytes
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   JSR   get_100_zeroed_bytes
   STA   __local_main__3
   STX   __local_main__2
   LDA   #$00
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$64
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_main__3
   CLC
   ADC   __local_main__i_0
   STA   __local_main__0
   TXA
   ADC   __local_main__i_1
   STA   __local_main__1
   LDA   (__local_main__0),Y
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   INC   __local_main__i_0
   BNE   .loop@0_start
   INC   __local_main__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$63
   STA   __zpabi_fill_100_bytes__byte_0
   LDA   #$00
   STA   __zpabi_fill_100_bytes__byte_1
   LDA   __local_main__3
   JSR   fill_100_bytes
   LDA   #$00
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@1_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$64
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .loop@1_break
   LDA   __local_main__3
   CLC
   ADC   __local_main__i_0
   STA   __local_main__0
   LDA   __local_main__2
   ADC   __local_main__i_1
   STA   __local_main__1
   LDA   (__local_main__0),Y
   CMP   #$63
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   INC   __local_main__i_0
   BNE   .loop@1_start
   INC   __local_main__i_1
   JMP   .loop@1_start
.loop@1_break:
   LDX   __local_main__2
   LDA   __local_main__3
   JSR   free_bytes
   LDX   #$00
   TXA
   RTS
