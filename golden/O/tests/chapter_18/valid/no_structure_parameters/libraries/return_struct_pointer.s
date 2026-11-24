SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_malloc__size_0	EQU	$80
__zpabi_malloc__size_1	EQU	$81
__zpabi_malloc__size_2	EQU	$82
__zpabi_malloc__size_3	EQU	$83
__local_make_struct_inner__0	EQU	$84
__local_make_struct_outermost__0	EQU	$84
__local_make_struct_inner__1	EQU	$85
__local_make_struct_outermost__1	EQU	$85
__local_make_struct_inner__2	EQU	$86
__local_make_struct_outermost__2	EQU	$86
__local_make_struct_inner__3	EQU	$87
__local_make_struct_outermost__3	EQU	$87
__local_make_struct_inner__4	EQU	$88
__local_make_struct_outermost__4	EQU	$88
__local_make_struct_inner__5	EQU	$89
__local_make_struct_outermost__5	EQU	$89
__local_make_struct_inner__6	EQU	$8A
__local_make_struct_outermost__6	EQU	$8A
__local_make_struct_inner__8	EQU	$8C
__local_make_struct_outermost__8	EQU	$8C
__local_make_struct_inner__9	EQU	$8D
__local_make_struct_outermost__9	EQU	$8D
__local_make_struct_inner__seed_1	EQU	$8E
__local_make_struct_outermost__10	EQU	$8E
__local_make_struct_inner__seed_0	EQU	$8F
__local_make_struct_outermost__11	EQU	$8F
__local_make_struct_outermost__seed_1	EQU	$90
__local_make_struct_outermost__seed_0	EQU	$91
__local_make_struct_outer__0	EQU	$92
__local_make_struct_outer__1	EQU	$93
__local_make_struct_outer__2	EQU	$94
__local_make_struct_outer__3	EQU	$95
__local_make_struct_outer__4	EQU	$96
__local_make_struct_outer__5	EQU	$97
__local_make_struct_outer__6	EQU	$98
__local_make_struct_outer__7	EQU	$99
__local_make_struct_outer__8	EQU	$9A
__local_make_struct_outer__10	EQU	$9C
__local_make_struct_outer__11	EQU	$9D
__local_make_struct_outer__seed_1	EQU	$9E
__local_make_struct_outer__seed_0	EQU	$9F

; @zp-link-meta-begin
; def make_struct_inner params=__zpabi_make_struct_inner__seed_0,__zpabi_make_struct_inner__seed_1 locals=__local_make_struct_inner__0,__local_make_struct_inner__1,__local_make_struct_inner__2,__local_make_struct_inner__3,__local_make_struct_inner__4,__local_make_struct_inner__5,__local_make_struct_inner__6,__local_make_struct_inner__7,__local_make_struct_inner__8,__local_make_struct_inner__9,__local_make_struct_inner__seed_1,__local_make_struct_inner__seed_0 indirect=false in_cycle=false param_regs=A,X
; def make_struct_outer params=__zpabi_make_struct_outer__seed_0,__zpabi_make_struct_outer__seed_1 locals=__local_make_struct_outer__0,__local_make_struct_outer__1,__local_make_struct_outer__2,__local_make_struct_outer__3,__local_make_struct_outer__4,__local_make_struct_outer__5,__local_make_struct_outer__6,__local_make_struct_outer__7,__local_make_struct_outer__8,__local_make_struct_outer__9,__local_make_struct_outer__10,__local_make_struct_outer__11,__local_make_struct_outer__seed_1,__local_make_struct_outer__seed_0 indirect=false in_cycle=false param_regs=A,X
; def make_struct_outermost params=__zpabi_make_struct_outermost__seed_0,__zpabi_make_struct_outermost__seed_1 locals=__local_make_struct_outermost__0,__local_make_struct_outermost__1,__local_make_struct_outermost__2,__local_make_struct_outermost__3,__local_make_struct_outermost__4,__local_make_struct_outermost__5,__local_make_struct_outermost__6,__local_make_struct_outermost__7,__local_make_struct_outermost__8,__local_make_struct_outermost__9,__local_make_struct_outermost__10,__local_make_struct_outermost__11,__local_make_struct_outermost__seed_1,__local_make_struct_outermost__seed_0 indirect=false in_cycle=false param_regs=A,X
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; call make_struct_inner -> malloc
; call make_struct_outer -> malloc
; call make_struct_outermost -> make_struct_outer
; call make_struct_outermost -> malloc
; item make_struct_inner section=.text local=false zero=false
; item make_struct_inner@run1 section=.text local=true zero=false
; item make_struct_inner@run0 section=.text local=true zero=false
; item make_struct_outer section=.text local=false zero=false
; item make_struct_outermost section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

make_struct_inner:
   SUBROUTINE

   STA   __local_make_struct_inner__seed_0
   STX   __local_make_struct_inner__seed_1
   LDA   #$0A
   STA   __zpabi_malloc__size_0
   JSR   make_struct_inner@run0
   STA   __local_make_struct_inner__8
   STX   __local_make_struct_inner__9
   LDA   __local_make_struct_inner__seed_0
   STA   HARGS
   LDA   __local_make_struct_inner__seed_1
   JSR   make_struct_inner@run1
   STA   __local_make_struct_inner__6
   LDA   HARGS+4
   STA   __local_make_struct_inner__5
   LDA   HARGS+5
   STA   __local_make_struct_inner__4
   LDA   HARGS+6
   STA   __local_make_struct_inner__3
   LDA   HARGS+7
   STA   __local_make_struct_inner__2
   LDA   HARGS+8
   STA   __local_make_struct_inner__1
   LDA   HARGS+9
   STA   __local_make_struct_inner__0
   LDA   __local_make_struct_inner__8
   STA   DPTR
   LDA   __local_make_struct_inner__9
   STA   DPTR+1
   LDA   HARGS+2
   LDY   #$00
   STA   (__local_make_struct_inner__8),Y
   LDA   __local_make_struct_inner__6
   INY
   STA   (DPTR),Y
   LDA   __local_make_struct_inner__5
   INY
   STA   (DPTR),Y
   LDA   __local_make_struct_inner__4
   INY
   STA   (DPTR),Y
   LDA   __local_make_struct_inner__3
   INY
   STA   (DPTR),Y
   LDA   __local_make_struct_inner__2
   INY
   STA   (DPTR),Y
   LDA   __local_make_struct_inner__1
   INY
   STA   (DPTR),Y
   LDA   __local_make_struct_inner__0
   INY
   STA   (DPTR),Y
   LDA   __local_make_struct_inner__8
   STA   DPTR
   LDA   __local_make_struct_inner__9
   STA   DPTR+1
   LDA   __local_make_struct_inner__seed_0
   INY
   STA   (__local_make_struct_inner__8),Y
   LDA   __local_make_struct_inner__seed_1
   INY
   STA   (DPTR),Y
   LDX   __local_make_struct_inner__9
   LDA   __local_make_struct_inner__8
   RTS

make_struct_inner@run1:
   SUBROUTINE

   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+3
   RTS

make_struct_inner@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JMP   malloc

make_struct_outer:
   SUBROUTINE

   STA   __local_make_struct_outer__seed_0
   STX   __local_make_struct_outer__seed_1
   LDA   #$0C
   STA   __zpabi_malloc__size_0
   JSR   make_struct_inner@run0
   STA   __local_make_struct_outer__10
   STX   __local_make_struct_outer__11
   LDA   __local_make_struct_outer__seed_0
   LDY   #$00
   STA   (__local_make_struct_outer__10),Y
   CLC
   ADC   #$01
   INY
   STA   (__local_make_struct_outer__10),Y
   LDA   __local_make_struct_outer__seed_0
   CLC
   ADC   #$02
   STA   HARGS
   LDA   __local_make_struct_outer__seed_1
   ADC   #$00
   JSR   make_struct_inner@run1
   STA   __local_make_struct_outer__8
   LDA   HARGS+4
   STA   __local_make_struct_outer__7
   LDA   HARGS+5
   STA   __local_make_struct_outer__6
   LDA   HARGS+6
   STA   __local_make_struct_outer__5
   LDA   HARGS+7
   STA   __local_make_struct_outer__4
   LDA   HARGS+8
   STA   __local_make_struct_outer__3
   LDA   HARGS+9
   STA   __local_make_struct_outer__0
   LDA   __local_make_struct_outer__10
   CLC
   ADC   #$02
   STA   __local_make_struct_outer__1
   LDA   __local_make_struct_outer__11
   ADC   #$00
   STA   __local_make_struct_outer__2
   LDA   HARGS+2
   LDY   #$00
   STA   (__local_make_struct_outer__1),Y
   LDA   __local_make_struct_outer__8
   INY
   STA   (__local_make_struct_outer__1),Y
   LDA   __local_make_struct_outer__7
   INY
   STA   (__local_make_struct_outer__1),Y
   LDA   __local_make_struct_outer__6
   INY
   STA   (__local_make_struct_outer__1),Y
   LDA   __local_make_struct_outer__5
   INY
   STA   (__local_make_struct_outer__1),Y
   LDA   __local_make_struct_outer__4
   INY
   STA   (__local_make_struct_outer__1),Y
   LDA   __local_make_struct_outer__3
   INY
   STA   (__local_make_struct_outer__1),Y
   LDA   __local_make_struct_outer__0
   INY
   STA   (__local_make_struct_outer__1),Y
   LDA   __local_make_struct_outer__seed_0
   CLC
   ADC   #$03
   BCC   .carry_inc@0
   INC   __local_make_struct_outer__seed_1
.carry_inc@0:
   LDY   #$08
   STA   (__local_make_struct_outer__1),Y
   LDA   __local_make_struct_outer__seed_1
   INY
   STA   (__local_make_struct_outer__1),Y
   LDX   __local_make_struct_outer__11
   LDA   __local_make_struct_outer__10
   RTS

make_struct_outermost:
   SUBROUTINE

   STA   __local_make_struct_outermost__seed_0
   STX   __local_make_struct_outermost__seed_1
   LDA   #$10
   STA   __zpabi_malloc__size_0
   JSR   make_struct_inner@run0
   STA   __local_make_struct_outermost__10
   STX   __local_make_struct_outermost__11
   STA   DPTR
   STX   DPTR+1
   LDA   __local_make_struct_outermost__seed_0
   LDY   #$00
   STA   (__local_make_struct_outermost__10),Y
   LDA   __local_make_struct_outermost__seed_1
   INY
   STA   (DPTR),Y
   LDA   __local_make_struct_outermost__seed_0
   CLC
   ADC   #$01
   STA   __local_make_struct_outermost__0
   LDA   __local_make_struct_outermost__seed_1
   ADC   #$00
   TAX
   LDA   __local_make_struct_outermost__0
   JSR   make_struct_outer
   LDY   __local_make_struct_outermost__10
   STY   DPTR
   LDY   __local_make_struct_outermost__11
   STY   DPTR+1
   LDY   #$02
   STA   (__local_make_struct_outermost__10),Y
   TXA
   INY
   STA   (DPTR),Y
   LDA   __local_make_struct_outermost__seed_0
   CLC
   ADC   #$05
   STA   __local_make_struct_outermost__0
   LDA   __local_make_struct_outermost__10
   CLC
   ADC   #$04
   STA   __local_make_struct_outermost__8
   LDA   __local_make_struct_outermost__11
   ADC   #$00
   STA   __local_make_struct_outermost__9
   LDA   __local_make_struct_outermost__0
   LDY   #$00
   STA   (__local_make_struct_outermost__8),Y
   LDA   __local_make_struct_outermost__seed_0
   CLC
   ADC   #$06
   INY
   STA   (__local_make_struct_outermost__8),Y
   LDA   __local_make_struct_outermost__seed_0
   CLC
   ADC   #$07
   STA   HARGS
   LDA   __local_make_struct_outermost__seed_1
   ADC   #$00
   JSR   make_struct_inner@run1
   STA   __local_make_struct_outermost__6
   LDA   HARGS+4
   STA   __local_make_struct_outermost__5
   LDA   HARGS+5
   STA   __local_make_struct_outermost__4
   LDA   HARGS+6
   STA   __local_make_struct_outermost__3
   LDA   HARGS+7
   STA   __local_make_struct_outermost__2
   LDA   HARGS+8
   STA   __local_make_struct_outermost__1
   LDA   HARGS+9
   STA   __local_make_struct_outermost__0
   LDA   HARGS+2
   LDY   #$02
   STA   (__local_make_struct_outermost__8),Y
   LDA   __local_make_struct_outermost__6
   INY
   STA   (__local_make_struct_outermost__8),Y
   LDA   __local_make_struct_outermost__5
   INY
   STA   (__local_make_struct_outermost__8),Y
   LDA   __local_make_struct_outermost__4
   INY
   STA   (__local_make_struct_outermost__8),Y
   LDA   __local_make_struct_outermost__3
   INY
   STA   (__local_make_struct_outermost__8),Y
   LDA   __local_make_struct_outermost__2
   INY
   STA   (__local_make_struct_outermost__8),Y
   LDA   __local_make_struct_outermost__1
   INY
   STA   (__local_make_struct_outermost__8),Y
   LDA   __local_make_struct_outermost__0
   INY
   STA   (__local_make_struct_outermost__8),Y
   LDA   __local_make_struct_outermost__seed_0
   CLC
   ADC   #$08
   BCC   .carry_inc@1
   INC   __local_make_struct_outermost__seed_1
.carry_inc@1:
   LDY   #$0A
   STA   (__local_make_struct_outermost__8),Y
   LDA   __local_make_struct_outermost__seed_1
   INY
   STA   (__local_make_struct_outermost__8),Y
   LDX   __local_make_struct_outermost__11
   LDA   __local_make_struct_outermost__10
   RTS
