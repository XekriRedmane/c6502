SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_array_to_list__count_0	EQU	$80
__zpabi_array_to_list__count_1	EQU	$81
__zpabi_malloc__size_0	EQU	$82
__zpabi_malloc__size_1	EQU	$83
__zpabi_malloc__size_2	EQU	$84
__zpabi_malloc__size_3	EQU	$85
__local_main__0	EQU	$86
__local_main__1	EQU	$87
__local_main__2	EQU	$88
__local_main__3	EQU	$89
__local_main__i	EQU	$8A
__local_main__elem	EQU	$8B
__local_main__4	EQU	$8C
__local_main__arr	EQU	$8D
__local_array_to_list__0	EQU	$95
__local_array_to_list__1	EQU	$96
__local_array_to_list__2	EQU	$97
__local_array_to_list__3	EQU	$98
__local_array_to_list__i_1	EQU	$99
__local_array_to_list__i_0	EQU	$9A
__local_array_to_list__current	EQU	$9B
__local_array_to_list__4	EQU	$9C
__local_array_to_list__5	EQU	$9D
__local_array_to_list__6	EQU	$9E
__local_array_to_list__array	EQU	$9F
__local_array_to_list__7	EQU	$A0

; @zp-link-meta-begin
; def array_to_list params=__zpabi_array_to_list__array_0,__zpabi_array_to_list__array_1,__zpabi_array_to_list__count_0,__zpabi_array_to_list__count_1 locals=__local_array_to_list__0,__local_array_to_list__1,__local_array_to_list__2,__local_array_to_list__3,__local_array_to_list__i_1,__local_array_to_list__i_0,__local_array_to_list__current,__local_array_to_list__4,__local_array_to_list__5,__local_array_to_list__6,__local_array_to_list__array,__local_array_to_list__7 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__i,__local_main__elem,__local_main__4,__local_main__arr,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12 indirect=false in_cycle=false
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; call array_to_list -> malloc
; call main -> array_to_list
; item array_to_list section=.text local=false zero=false
; item array_to_list@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

array_to_list:
   SUBROUTINE

   STA   __local_array_to_list__array
   STX   __local_array_to_list__7
   JSR   array_to_list@run0
   STA   __local_array_to_list__5
   STX   __local_array_to_list__6
   STA   DPTR
   LDY   #$01
   LDA   (__local_array_to_list__array),Y
   STA   __local_array_to_list__0
   STX   DPTR+1
   DEY
   LDA   (__local_array_to_list__array),Y
   STA   (__local_array_to_list__5),Y
   LDA   __local_array_to_list__0
   INY
   STA   (DPTR),Y
   LDA   __local_array_to_list__5
   STA   DPTR
   STX   DPTR+1
   STA   __local_array_to_list__current
   LDA   #$00
   INY
   STA   (__local_array_to_list__5),Y
   INY
   STA   (DPTR),Y
   STX   __local_array_to_list__4
   LDA   #$01
   STA   __local_array_to_list__i_0
   LDY   #$00
   STY   __local_array_to_list__i_1
.loop@0_start:
   LDA   __local_array_to_list__i_0
   SEC
   SBC   __zpabi_array_to_list__count_0
   LDA   __local_array_to_list__i_1
   SBC   __zpabi_array_to_list__count_1
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   JSR   array_to_list@run0
   LDY   __local_array_to_list__current
   STY   DPTR
   LDY   __local_array_to_list__4
   STY   DPTR+1
   LDY   #$02
   STA   (__local_array_to_list__current),Y
   TXA
   INY
   STA   (DPTR),Y
   DEY
   LDA   (__local_array_to_list__current),Y
   STA   __local_array_to_list__0
   STA   DPTR
   INY
   LDA   (__local_array_to_list__current),Y
   STA   __local_array_to_list__1
   STA   DPTR+1
   LDA   #$00
   DEY
   STA   (__local_array_to_list__0),Y
   INY
   STA   (DPTR),Y
   LDA   __local_array_to_list__i_0
   ASL
   STA   __local_array_to_list__2
   LDA   __local_array_to_list__i_1
   ROL
   STA   __local_array_to_list__3
   LDA   __local_array_to_list__array
   CLC
   ADC   __local_array_to_list__2
   STA   __local_array_to_list__0
   LDA   __local_array_to_list__7
   ADC   __local_array_to_list__3
   STA   __local_array_to_list__1
   LDY   #$00
   LDA   (__local_array_to_list__0),Y
   STA   __local_array_to_list__3
   INY
   LDA   (__local_array_to_list__0),Y
   STA   __local_array_to_list__2
   INY
   LDA   (__local_array_to_list__current),Y
   STA   __local_array_to_list__0
   INY
   LDA   (__local_array_to_list__current),Y
   STA   __local_array_to_list__1
   LDA   __local_array_to_list__3
   LDY   #$00
   STA   (__local_array_to_list__0),Y
   LDA   __local_array_to_list__2
   INY
   STA   (__local_array_to_list__0),Y
   INY
   LDA   (__local_array_to_list__current),Y
   STA   __local_array_to_list__0
   INY
   LDA   (__local_array_to_list__current),Y
   STA   __local_array_to_list__4
   INC   __local_array_to_list__i_0
   BNE   .inc_done@0
   INC   __local_array_to_list__i_1
.inc_done@0:
   LDA   __local_array_to_list__0
   STA   __local_array_to_list__current
   JMP   .loop@0_start
.loop@0_break:
   LDX   __local_array_to_list__6
   LDA   __local_array_to_list__5
   RTS

array_to_list@run0:
   SUBROUTINE

   LDA   #$04
   STA   __zpabi_malloc__size_0
   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JMP   malloc

main:
   SUBROUTINE

   LDA   #$09
   STA   __local_main__arr
   LDA   #$00
   STA   __local_main__arr+1
   LDY   #$08
   STY   __local_main__arr+2
   STA   __local_main__arr+3
   DEY
   STY   __local_main__arr+4
   STA   __local_main__arr+5
   DEY
   STY   __local_main__arr+6
   STA   __local_main__arr+7
   LDA   #$04
   STA   __zpabi_array_to_list__count_0
   LDA   #$00
   STA   __zpabi_array_to_list__count_1
   LDX   #>__local_main__arr
   LDA   #<__local_main__arr
   JSR   array_to_list
   STA   __local_main__elem
   STX   __local_main__4
   LDA   #$00
   STA   __local_main__i
   LDX   #$00
.loop@1_start:
   LDA   __local_main__i
   SEC
   SBC   #$04
   TXA
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .loop@1_break
   LDA   __local_main__i
   ASL
   STA   __local_main__0
   LDA   #<__local_main__arr
   CLC
   ADC   __local_main__0
   STA   __local_main__0
   LDA   #>__local_main__arr
   ADC   #$00
   STA   __local_main__1
   LDY   #$00
   LDA   (__local_main__0),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__2
   DEY
   LDA   (__local_main__elem),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__elem),Y
   CMP   __local_main__2
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   __local_main__3
   BEQ   .if_end@0
.jcmp_skip@2:
   LDA   __local_main__i
   CLC
   ADC   #$01
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   RTS
.if_end@0:
   LDY   #$02
   LDA   (__local_main__elem),Y
   STA   __local_main__0
   INY
   LDA   (__local_main__elem),Y
   STA   __local_main__4
   INC   __local_main__i
   BNE   .inc_done@1
   INX
.inc_done@1:
   LDA   __local_main__0
   STA   __local_main__elem
   JMP   .loop@1_start
.loop@1_break:
   LDX   #$00
   TXA
   RTS
