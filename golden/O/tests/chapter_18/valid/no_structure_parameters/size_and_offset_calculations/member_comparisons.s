SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_calloc__nmem_0	EQU	$80
__zpabi_calloc__nmem_1	EQU	$81
__zpabi_calloc__nmem_2	EQU	$82
__zpabi_calloc__nmem_3	EQU	$83
__zpabi_calloc__size_0	EQU	$84
__zpabi_calloc__size_1	EQU	$85
__zpabi_calloc__size_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87
__local_main__0	EQU	$88
__local_main__1	EQU	$89
__local_main__2	EQU	$8A
__local_main__3	EQU	$8B
__local_main__4	EQU	$8C
__local_main__5	EQU	$8D

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmem_0,__zpabi_calloc__nmem_1,__zpabi_calloc__nmem_2,__zpabi_calloc__nmem_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; call main -> calloc
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_calloc__nmem_0
   LDA   #$06
   STA   __zpabi_calloc__size_0
   LDA   #$00
   STA   __zpabi_calloc__nmem_1
   STA   __zpabi_calloc__nmem_2
   STA   __zpabi_calloc__nmem_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   JSR   calloc
   STA   __local_main__5
   STX   __local_main__4
   CPX   __local_main__4
   BNE   .jcmp_skip@0
   CMP   __local_main__5
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   SEC
   SBC   __local_main__5
   TXA
   SBC   __local_main__4
   BCS   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_main__5
   CLC
   ADC   #$04
   STA   __local_main__3
   TXA
   ADC   #$00
   STA   __local_main__2
   LDA   __local_main__5
   SEC
   SBC   __local_main__3
   TXA
   SBC   __local_main__2
   BCC   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__5
   ADC   #$02
   STA   __local_main__1
   TXA
   ADC   #$00
   STA   __local_main__0
   LDA   __local_main__3
   SEC
   SBC   __local_main__1
   LDA   __local_main__2
   SBC   __local_main__0
   BCS   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __local_main__5
   SBC   __local_main__1
   TXA
   SBC   __local_main__0
   BCC   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS
