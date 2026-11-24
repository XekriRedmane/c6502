SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_malloc__size_0	EQU	$80
__zpabi_memcpy__s2_0	EQU	$80
__zpabi_malloc__size_1	EQU	$81
__zpabi_memcpy__s2_1	EQU	$81
__zpabi_malloc__size_2	EQU	$82
__zpabi_memcpy__n_0	EQU	$82
__zpabi_malloc__size_3	EQU	$83
__zpabi_memcpy__n_1	EQU	$83
__zpabi_memcpy__n_2	EQU	$84
__local_main__1	EQU	$85
__zpabi_memcpy__n_3	EQU	$85
__local_main__2	EQU	$86
__local_main__3	EQU	$87
__local_main__4	EQU	$88
__local_main__5	EQU	$89
__local_main__6	EQU	$8A
__local_main__7	EQU	$8B
__local_main__8	EQU	$8C
__local_main__9	EQU	$8D

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9 indirect=false in_cycle=false
; ext free params=__zpabi_free__ptr_0,__zpabi_free__ptr_1 param_regs=A,X
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext memcpy params=__zpabi_memcpy__s1_0,__zpabi_memcpy__s1_1,__zpabi_memcpy__s2_0,__zpabi_memcpy__s2_1,__zpabi_memcpy__n_0,__zpabi_memcpy__n_1,__zpabi_memcpy__n_2,__zpabi_memcpy__n_3 param_regs=A,X,,,,,,
; call main -> free
; call main -> malloc
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$20
   STA   __zpabi_malloc__size_0
   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JSR   malloc
   STA   __local_main__8
   STX   __local_main__9
   STA   DPTR
   STX   DPTR+1
   LDA   #$00
   LDY   #$10
   STA   (__local_main__8),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$24
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   CPX   __local_main__9
   BNE   .jcmp_skip@0
   LDA   __local_main__8
   CMP   __local_main__8
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDY   #$10
   LDA   (__local_main__8),Y
   STA   __local_main__7
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__6
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__5
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__4
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__8),Y
   CMP   #$40
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$24
   BNE   .jcmp_skip@1
   LDA   __local_main__2
   BNE   .jcmp_skip@1
   LDA   __local_main__3
   BNE   .jcmp_skip@1
   LDA   __local_main__4
   BNE   .jcmp_skip@1
   LDA   __local_main__5
   BNE   .jcmp_skip@1
   LDA   __local_main__6
   BNE   .jcmp_skip@1
   LDA   __local_main__7
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_main__8
   AND   #$07
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__8
   JSR   free
   LDA   #$00
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$04
   RTS
.if_end@4:
   LDX   #$00
   RTS
