SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_calloc__size_0	EQU	$84
__zpabi_calloc__size_1	EQU	$85
__zpabi_calloc__size_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87
__local_main__my_struct_0	EQU	$88
__local_main__my_struct_3	EQU	$89
__local_main__my_struct_2	EQU	$8A
__local_main__my_struct_1	EQU	$8B
__local_main__0	EQU	$8C
__local_main__1	EQU	$8D
__local_main__my_struct_4	EQU	$8E
__local_main__my_struct_5	EQU	$8F
__local_main__2	EQU	$90
__local_main__3	EQU	$91
__local_main__my_struct_11	EQU	$92
__local_main__my_struct_10	EQU	$93
__local_main__my_struct_9	EQU	$94
__local_main__my_struct_8	EQU	$95
__local_main__my_struct_7	EQU	$96
__local_main__my_struct_6	EQU	$97

; @zp-link-meta-begin
; def main params= locals=__local_main__my_struct_0,__local_main__my_struct_3,__local_main__my_struct_2,__local_main__my_struct_1,__local_main__0,__local_main__1,__local_main__my_struct_4,__local_main__my_struct_5,__local_main__2,__local_main__3,__local_main__my_struct_11,__local_main__my_struct_10,__local_main__my_struct_9,__local_main__my_struct_8,__local_main__my_struct_7,__local_main__my_struct_6,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; call main -> calloc
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$5C
   STA   __local_main__my_struct_0
   LDA   #$00
   STA   __local_main__my_struct_1
   STA   __local_main__my_struct_2
   LDA   #$80
   STA   __local_main__my_struct_3
   LDA   #$03
   STA   __zpabi_calloc__nmemb_0
   STA   __zpabi_calloc__size_0
   LDA   #$00
   STA   __zpabi_calloc__nmemb_1
   STA   __zpabi_calloc__nmemb_2
   STA   __zpabi_calloc__nmemb_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   JSR   calloc
   STA   __local_main__my_struct_4
   STX   __local_main__my_struct_5
   LDA   #$18
   STA   __local_main__my_struct_6
   LDA   #$FC
   STA   __local_main__my_struct_7
   LDA   #$30
   STA   __local_main__my_struct_8
   LDA   #$F8
   STA   __local_main__my_struct_9
   LDA   #$48
   STA   __local_main__my_struct_10
   LDA   #$F4
   STA   __local_main__my_struct_11
   INC   __local_main__my_struct_0
   BNE   .inc_done@0
   INC   __local_main__my_struct_1
   BNE   .inc_done@0
   INC   __local_main__my_struct_2
   BNE   .inc_done@0
   INC   __local_main__my_struct_3
.inc_done@0:
   LDA   __local_main__my_struct_0
   STA   __local_main__3
   LDA   __local_main__my_struct_1
   STA   __local_main__2
   LDA   __local_main__my_struct_2
   STA   __local_main__1
   LDA   __local_main__my_struct_3
   STA   __local_main__0
   CMP   #$80
   BNE   .jcmp_skip@0
   LDA   __local_main__my_struct_2
   BNE   .jcmp_skip@0
   LDA   __local_main__my_struct_1
   BNE   .jcmp_skip@0
   LDA   __local_main__my_struct_0
   CMP   #$5D
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_main__my_struct_4
   STA   DPTR
   STX   DPTR+1
   LDY   #$01
   LDA   (__local_main__my_struct_4),Y
   STA   __local_main__my_struct_0
   INY
   LDA   (__local_main__my_struct_4),Y
   STA   __local_main__my_struct_3
   LDA   __local_main__my_struct_0
   BNE   .carry_inc@0
   DEC   __local_main__my_struct_3
.carry_inc@0:
   DEC   __local_main__my_struct_0
   LDA   __local_main__my_struct_0
   LDY   #$01
   STA   (__local_main__my_struct_4),Y
   LDA   __local_main__my_struct_3
   INY
   STA   (__local_main__my_struct_4),Y
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   __local_main__my_struct_3
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   __local_main__my_struct_0
   CMP   #$FF
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDY   #$00
   LDA   (__local_main__my_struct_4),Y
   STA   __local_main__my_struct_3
   CLC
   ADC   #$01
   STA   (__local_main__my_struct_4),Y
   LDA   __local_main__my_struct_3
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__my_struct_8
   SEC
   SBC   #$01
   STA   __local_main__my_struct_1
   LDA   __local_main__my_struct_9
   SBC   #$00
   STA   __local_main__my_struct_2
   LDA   __local_main__my_struct_9
   CMP   #$F8
   BNE   .jcmp_skip@2
   LDA   __local_main__my_struct_8
   CMP   #$30
   BEQ   .if_end@3
.jcmp_skip@2:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __local_main__0
   CMP   #$80
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   BNE   .jcmp_skip@3
   LDA   __local_main__2
   BNE   .jcmp_skip@3
   LDA   __local_main__3
   CMP   #$5D
   BEQ   .if_end@4
.jcmp_skip@3:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDY   #$00
   LDA   (__local_main__my_struct_4),Y
   CMP   #$01
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDY   #$01
   LDA   (__local_main__my_struct_4),Y
   STA   __local_main__my_struct_3
   INY
   LDA   (__local_main__my_struct_4),Y
   STA   __local_main__my_struct_0
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_main__my_struct_0
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_main__my_struct_3
   CMP   #$FF
   BEQ   .if_end@6
.jcmp_skip@4:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   __local_main__my_struct_2
   CMP   #$F8
   BNE   .jcmp_skip@5
   LDA   __local_main__my_struct_1
   CMP   #$2F
   BEQ   .if_end@7
.jcmp_skip@5:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   __local_main__my_struct_7
   CMP   #$FC
   BNE   .main@jthread@0
   LDA   __local_main__my_struct_6
   CMP   #$18
   BNE   .main@jthread@0
   LDA   __local_main__my_struct_11
   CMP   #$F4
   BNE   .main@jthread@0
   LDA   __local_main__my_struct_10
   CMP   #$48
   BEQ   .if_end@10
.main@jthread@0:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@10:
   LDA   #$FF
   LDY   #$03
   STA   (__local_main__my_struct_4),Y
   LDA   #$01
   INY
   STA   (__local_main__my_struct_4),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   __local_main__my_struct_4
   STA   DPTR
   STX   DPTR+1
   LDA   #$58
   INY
   STA   (__local_main__my_struct_4),Y
   LDA   #$A0
   INY
   STA   (__local_main__my_struct_4),Y
   LDA   #$86
   INY
   STA   (DPTR),Y
   LDA   __local_main__my_struct_4
   CLC
   ADC   #$03
   STA   __local_main__my_struct_4
   BCC   .carry_inc@1
   INC   __local_main__my_struct_5
.carry_inc@1:
   LDY   #$00
   LDA   (__local_main__my_struct_4),Y
   SEC
   SBC   #$01
   STA   (__local_main__my_struct_4),Y
   LDA   __local_main__my_struct_4
   CLC
   ADC   #$03
   STA   __local_main__0
   LDA   __local_main__my_struct_5
   ADC   #$00
   STA   __local_main__1
   CLC
   INY
   LDA   (__local_main__my_struct_4),Y
   STA   __local_main__my_struct_0
   INY
   LDA   (__local_main__my_struct_4),Y
   STA   __local_main__my_struct_3
   INC   __local_main__my_struct_0
   BNE   .inc_done@1
   INC   __local_main__my_struct_3
.inc_done@1:
   LDA   __local_main__my_struct_0
   LDY   #$01
   STA   (__local_main__my_struct_4),Y
   LDA   __local_main__my_struct_3
   INY
   STA   (__local_main__my_struct_4),Y
   LDA   #$FA
   ADC   __local_main__0
   STA   __local_main__my_struct_0
   LDA   #$FF
   ADC   __local_main__1
   STA   __local_main__my_struct_3
   LDY   #$00
   LDA   (__local_main__my_struct_0),Y
   CMP   #$01
   BNE   .main@jthread@1
   LDA   #$FB
   CLC
   ADC   __local_main__0
   STA   __local_main__my_struct_2
   LDA   #$FF
   ADC   __local_main__1
   STA   __local_main__my_struct_1
   LDA   (__local_main__my_struct_2),Y
   STA   __local_main__my_struct_3
   INY
   LDA   (__local_main__my_struct_2),Y
   STA   __local_main__my_struct_0
   LDA   #$00
   CMP   #$FF
   BNE   .main@jthread@1
   LDA   __local_main__my_struct_0
   CMP   #$FF
   BNE   .main@jthread@1
   LDA   __local_main__my_struct_3
   CMP   #$FF
   BEQ   .if_end@13
.main@jthread@1:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@13:
   LDA   #$FD
   CLC
   ADC   __local_main__0
   STA   __local_main__my_struct_0
   LDA   #$FF
   ADC   __local_main__1
   STA   __local_main__my_struct_3
   LDY   #$00
   LDA   (__local_main__my_struct_0),Y
   STA   __local_main__my_struct_0
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __local_main__my_struct_0
   CMP   #$FE
   BEQ   .if_end@14
.jcmp_skip@6:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@14:
   LDA   #$FE
   CLC
   ADC   __local_main__0
   STA   __local_main__my_struct_2
   LDA   #$FF
   ADC   __local_main__1
   STA   __local_main__my_struct_1
   LDY   #$00
   LDA   (__local_main__my_struct_2),Y
   STA   __local_main__my_struct_3
   INY
   LDA   (__local_main__my_struct_2),Y
   BNE   .jcmp_skip@7
   LDA   __local_main__my_struct_3
   CMP   #$02
   BEQ   .if_end@15
.jcmp_skip@7:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@15:
   LDY   #$00
   LDA   (__local_main__0),Y
   CMP   #$58
   BNE   .main@jthread@2
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__my_struct_3
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__my_struct_0
   LDA   #$00
   CMP   #$01
   BNE   .main@jthread@2
   LDA   __local_main__my_struct_0
   CMP   #$86
   BNE   .main@jthread@2
   LDA   __local_main__my_struct_3
   CMP   #$A0
   BEQ   .if_end@18
.main@jthread@2:
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@18:
   LDX   #$00
   TXA
   RTS
