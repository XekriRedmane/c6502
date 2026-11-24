SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_aligned_alloc__alignment_0	EQU	$80
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_malloc__size_0	EQU	$80
__zpabi_realloc__size_0	EQU	$80
__zpabi_aligned_alloc__alignment_1	EQU	$81
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_malloc__size_1	EQU	$81
__zpabi_realloc__size_1	EQU	$81
__zpabi_aligned_alloc__alignment_2	EQU	$82
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_malloc__size_2	EQU	$82
__zpabi_realloc__size_2	EQU	$82
__zpabi_aligned_alloc__alignment_3	EQU	$83
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_malloc__size_3	EQU	$83
__zpabi_realloc__size_3	EQU	$83
__zpabi_aligned_alloc__size_0	EQU	$84
__zpabi_calloc__size_0	EQU	$84
__zpabi_aligned_alloc__size_1	EQU	$85
__zpabi_calloc__size_1	EQU	$85
__zpabi_aligned_alloc__size_2	EQU	$86
__zpabi_calloc__size_2	EQU	$86
__zpabi_aligned_alloc__size_3	EQU	$87
__zpabi_calloc__size_3	EQU	$87
__local_main__0	EQU	$88
__local_main__1	EQU	$89
__local_main__i_1	EQU	$8A
__local_main__i_0	EQU	$8B
__local_main__2	EQU	$8C
__local_main__3	EQU	$8D
__local_main__4	EQU	$8E
__local_main__6	EQU	$90
__local_main__7	EQU	$91
__local_main__8	EQU	$92
__local_main__9	EQU	$93
__local_main__10	EQU	$94

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__i_1,__local_main__i_0,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; ext aligned_alloc params=__zpabi_aligned_alloc__alignment_0,__zpabi_aligned_alloc__alignment_1,__zpabi_aligned_alloc__alignment_2,__zpabi_aligned_alloc__alignment_3,__zpabi_aligned_alloc__size_0,__zpabi_aligned_alloc__size_1,__zpabi_aligned_alloc__size_2,__zpabi_aligned_alloc__size_3
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext free params=__zpabi_free__ptr_0,__zpabi_free__ptr_1 param_regs=A,X
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext realloc params=__zpabi_realloc__ptr_0,__zpabi_realloc__ptr_1,__zpabi_realloc__size_0,__zpabi_realloc__size_1,__zpabi_realloc__size_2,__zpabi_realloc__size_3 param_regs=A,X,,,,
; call main -> aligned_alloc
; call main -> calloc
; call main -> free
; call main -> malloc
; call main -> realloc
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$32
   STA   __zpabi_malloc__size_0
   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JSR   malloc
   STA   __local_main__2
   LDA   #$00
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$32
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_main__2
   CLC
   ADC   __local_main__i_0
   STA   __local_main__0
   TXA
   ADC   __local_main__i_1
   STA   __local_main__1
   LDA   __local_main__i_0
   LDY   #$00
   STA   (__local_main__0),Y
   INC   __local_main__i_0
   BNE   .loop@0_start
   INC   __local_main__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$64
   STA   __zpabi_realloc__size_0
   LDA   #$00
   STA   __zpabi_realloc__size_1
   STA   __zpabi_realloc__size_2
   STA   __zpabi_realloc__size_3
   LDA   __local_main__2
   JSR   realloc
   STA   __local_main__2
   STX   __local_main__3
   LDA   #$0B
   LDY   #$4B
   STA   (__local_main__2),Y
   LDA   #$00
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@1_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$32
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .loop@1_break
   LDA   __local_main__2
   CLC
   ADC   __local_main__i_0
   STA   __local_main__0
   TXA
   ADC   __local_main__i_1
   STA   __local_main__1
   LDY   #$00
   LDA   (__local_main__0),Y
   STA   __local_main__0
   LDA   #$00
   CMP   __local_main__i_1
   BNE   .jcmp_skip@2
   LDA   __local_main__0
   CMP   __local_main__i_0
   BEQ   .if_end@0
.jcmp_skip@2:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   INC   __local_main__i_0
   BNE   .loop@1_start
   INC   __local_main__i_1
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$4B
   LDA   (__local_main__2),Y
   CMP   #$0B
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_main__2
   JSR   free
   LDA   #$0A
   STA   __zpabi_calloc__nmemb_0
   LDA   #$08
   STA   __zpabi_calloc__size_0
   LDA   #$00
   STA   __zpabi_calloc__nmemb_1
   STA   __zpabi_calloc__nmemb_2
   STA   __zpabi_calloc__nmemb_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   JSR   calloc
   STA   __local_main__10
   LDA   #$00
   STA   __local_main__9
   LDY   #$00
   STY   __local_main__8
.loop@2_start:
   LDA   __local_main__9
   SEC
   SBC   #$0A
   LDA   __local_main__8
   SBC   #$00
   BVC   .jcmp_novf@3
   EOR   #$80
.jcmp_novf@3:
   BMI   .lb_skip@1
   JMP   .loop@2_break
.lb_skip@1:
   LDA   __local_main__9
   ASL
   STA   __local_main__0
   LDA   __local_main__8
   ROL
   ASL   __local_main__0
   ROL
   ASL   __local_main__0
   ROL
   STA   __local_main__1
   LDA   __local_main__10
   CLC
   ADC   __local_main__0
   STA   __local_main__6
   TXA
   ADC   __local_main__1
   STA   __local_main__7
   LDY   #$01
   LDA   (__local_main__6),Y
   STA   __local_main__4
   INY
   LDA   (__local_main__6),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__6),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__6),Y
   STA   __local_main__i_0
   INY
   LDA   (__local_main__6),Y
   STA   __local_main__i_1
   INY
   LDA   (__local_main__6),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__6),Y
   STA   __local_main__0
   LDY   #$00
   LDA   (__local_main__6),Y
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__i_0
   ORA   __local_main__i_1
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   INC   __local_main__9
   BEQ   .lb_skip@0
   JMP   .loop@2_start
.lb_skip@0:
   INC   __local_main__8
   JMP   .loop@2_start
.loop@2_break:
   LDA   __local_main__10
   JSR   free
   LDA   #$01
   STA   __zpabi_aligned_alloc__alignment_1
   STA   __zpabi_aligned_alloc__size_1
   LDA   #$00
   STA   __zpabi_aligned_alloc__alignment_0
   STA   __zpabi_aligned_alloc__alignment_2
   STA   __zpabi_aligned_alloc__alignment_3
   STA   __zpabi_aligned_alloc__size_0
   STA   __zpabi_aligned_alloc__size_2
   STA   __zpabi_aligned_alloc__size_3
   JSR   aligned_alloc
   STA   __local_main__0
   AND   #$FF
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __local_main__0
   JSR   free
   LDX   #$00
   TXA
   RTS
