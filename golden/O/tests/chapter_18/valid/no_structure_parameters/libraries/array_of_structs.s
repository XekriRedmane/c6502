SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_validate_struct_array__0	EQU	$80
__local_validate_struct_array__1	EQU	$81
__local_validate_struct_array__2	EQU	$82
__local_validate_struct_array__3	EQU	$83
__local_validate_struct_array__4	EQU	$84
__local_validate_struct_array__5	EQU	$85
__local_validate_struct_array__6	EQU	$86
__local_validate_struct_array__7	EQU	$87
__local_validate_struct_array__8	EQU	$88
__local_validate_struct_array__9	EQU	$89
__local_validate_struct_array__i_1	EQU	$8A
__local_validate_struct_array__i_0	EQU	$8B
__local_validate_struct_array__struct_array_0	EQU	$8D

; @zp-link-meta-begin
; def validate_struct_array params=__zpabi_validate_struct_array__struct_array_0,__zpabi_validate_struct_array__struct_array_1 locals=__local_validate_struct_array__0,__local_validate_struct_array__1,__local_validate_struct_array__2,__local_validate_struct_array__3,__local_validate_struct_array__4,__local_validate_struct_array__5,__local_validate_struct_array__6,__local_validate_struct_array__7,__local_validate_struct_array__8,__local_validate_struct_array__9,__local_validate_struct_array__i_1,__local_validate_struct_array__i_0,__local_validate_struct_array__struct_array_1,__local_validate_struct_array__struct_array_0 indirect=false in_cycle=false param_regs=A,X
; item validate_struct_array section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

validate_struct_array:
   SUBROUTINE

   STA   __local_validate_struct_array__struct_array_0
   LDA   #$00
   STA   __local_validate_struct_array__i_0
   LDY   #$00
   STY   __local_validate_struct_array__i_1
.loop@0_start:
   LDA   __local_validate_struct_array__i_0
   SEC
   SBC   #$03
   LDA   __local_validate_struct_array__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   __local_validate_struct_array__i_0
   ASL
   STA   __local_validate_struct_array__2
   LDA   __local_validate_struct_array__i_1
   ROL
   ASL   __local_validate_struct_array__2
   ROL
   ASL   __local_validate_struct_array__2
   ROL
   STA   __local_validate_struct_array__3
   LDA   __local_validate_struct_array__2
   SEC
   SBC   __local_validate_struct_array__i_0
   STA   __local_validate_struct_array__1
   LDA   __local_validate_struct_array__3
   SBC   __local_validate_struct_array__i_1
   STA   __local_validate_struct_array__0
   LDA   __local_validate_struct_array__struct_array_0
   CLC
   ADC   __local_validate_struct_array__1
   STA   __local_validate_struct_array__8
   TXA
   ADC   __local_validate_struct_array__0
   STA   __local_validate_struct_array__9
   LDY   #$00
   LDA   (__local_validate_struct_array__8),Y
   STA   __local_validate_struct_array__2
   LDA   __local_validate_struct_array__i_0
   ASL
   STA   __local_validate_struct_array__0
   LDA   __local_validate_struct_array__i_1
   ROL
   STA   __local_validate_struct_array__1
   LDA   #$00
   CMP   __local_validate_struct_array__1
   BNE   .jcmp_skip@1
   LDA   __local_validate_struct_array__2
   CMP   __local_validate_struct_array__0
   BEQ   .if_end@0
.jcmp_skip@1:
   LDX   #$00
   TXA
   RTS
.if_end@0:
   LDY   #$01
   LDA   (__local_validate_struct_array__8),Y
   STA   __local_validate_struct_array__7
   INY
   LDA   (__local_validate_struct_array__8),Y
   STA   __local_validate_struct_array__6
   INY
   LDA   (__local_validate_struct_array__8),Y
   STA   __local_validate_struct_array__5
   INY
   LDA   (__local_validate_struct_array__8),Y
   STA   __local_validate_struct_array__4
   LDA   __local_validate_struct_array__0
   CLC
   ADC   __local_validate_struct_array__i_0
   STA   __local_validate_struct_array__3
   LDA   __local_validate_struct_array__1
   ADC   __local_validate_struct_array__i_1
   STA   __local_validate_struct_array__2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_validate_struct_array__1
   STA   __local_validate_struct_array__0
   LDA   __local_validate_struct_array__4
   CMP   __local_validate_struct_array__0
   BNE   .jcmp_skip@6
   LDA   __local_validate_struct_array__5
   CMP   __local_validate_struct_array__1
   BNE   .jcmp_skip@6
   LDA   __local_validate_struct_array__6
   CMP   __local_validate_struct_array__2
   BNE   .jcmp_skip@6
   LDA   __local_validate_struct_array__7
   CMP   __local_validate_struct_array__3
   BNE   .jcmp_skip@6
   LDY   #$05
   LDA   (__local_validate_struct_array__8),Y
   STA   __local_validate_struct_array__0
   LDA   __local_validate_struct_array__i_0
   ASL
   STA   __local_validate_struct_array__3
   LDA   __local_validate_struct_array__i_1
   ROL
   ASL   __local_validate_struct_array__3
   ROL
   STA   __local_validate_struct_array__4
   LDA   #$00
   CMP   __local_validate_struct_array__4
   BNE   .jcmp_skip@6
   LDA   __local_validate_struct_array__0
   CMP   __local_validate_struct_array__3
   BNE   .jcmp_skip@6
   INY
   LDA   (__local_validate_struct_array__8),Y
   STA   __local_validate_struct_array__2
   LDA   __local_validate_struct_array__3
   CLC
   ADC   __local_validate_struct_array__i_0
   STA   __local_validate_struct_array__1
   LDA   __local_validate_struct_array__4
   ADC   __local_validate_struct_array__i_1
   STA   __local_validate_struct_array__0
   LDA   #$00
   CMP   __local_validate_struct_array__0
   BNE   .jcmp_skip@6
   LDA   __local_validate_struct_array__2
   CMP   __local_validate_struct_array__1
   BEQ   .if_end@3
.jcmp_skip@6:
   LDX   #$00
   TXA
   RTS
.if_end@3:
   INC   __local_validate_struct_array__i_0
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_validate_struct_array__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$00
   LDA   #$01
   RTS
