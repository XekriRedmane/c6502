SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_validate_nested__0	EQU	$80
__zpabi_strcmp__s2_0	EQU	$80
__local_validate_nested__1	EQU	$81
__local_validate_simple__1	EQU	$81
__local_validate_simple_converted__1	EQU	$81
__zpabi_strcmp__s2_1	EQU	$81
__local_validate_nested__2	EQU	$82
__local_validate_simple__2	EQU	$82
__local_validate_simple_converted__2	EQU	$82
__local_validate_nested__i_1	EQU	$83
__local_validate_simple__3	EQU	$83
__local_validate_simple_converted__3	EQU	$83
__local_validate_nested__i_0	EQU	$84
__local_validate_simple__4	EQU	$84
__local_validate_simple_converted__4	EQU	$84
__local_validate_simple__5	EQU	$85
__local_validate_simple_converted__5	EQU	$85
__local_validate_simple__6	EQU	$86
__local_validate_simple_converted__6	EQU	$86
__local_validate_simple__7	EQU	$87
__local_validate_simple_converted__7	EQU	$87
__local_validate_nested__ptr	EQU	$88
__local_validate_simple__ptr	EQU	$88
__local_validate_simple_converted__ptr	EQU	$88
__local_validate_nested__6	EQU	$89
__local_validate_simple__8	EQU	$89
__local_validate_simple_converted__8	EQU	$89

; @zp-link-meta-begin
; def validate_nested params=__zpabi_validate_nested__ptr_0,__zpabi_validate_nested__ptr_1 locals=__local_validate_nested__0,__local_validate_nested__1,__local_validate_nested__2,__local_validate_nested__i_1,__local_validate_nested__i_0,__local_validate_nested__3,__local_validate_nested__4,__local_validate_nested__5,__local_validate_nested__ptr,__local_validate_nested__6 indirect=false in_cycle=false param_regs=A,X
; def validate_nested_partial params=__zpabi_validate_nested_partial__ptr_0,__zpabi_validate_nested_partial__ptr_1 locals=__local_validate_nested_partial__0,__local_validate_nested_partial__1,__local_validate_nested_partial__2,__local_validate_nested_partial__3,__local_validate_nested_partial__4,__local_validate_nested_partial__5,__local_validate_nested_partial__6,__local_validate_nested_partial__7,__local_validate_nested_partial__ptr,__local_validate_nested_partial__8 indirect=false in_cycle=false param_regs=A,X
; def validate_simple params=__zpabi_validate_simple__ptr_0,__zpabi_validate_simple__ptr_1 locals=__local_validate_simple__0,__local_validate_simple__1,__local_validate_simple__2,__local_validate_simple__3,__local_validate_simple__4,__local_validate_simple__5,__local_validate_simple__6,__local_validate_simple__7,__local_validate_simple__ptr,__local_validate_simple__8 indirect=false in_cycle=false param_regs=A,X
; def validate_simple_converted params=__zpabi_validate_simple_converted__ptr_0,__zpabi_validate_simple_converted__ptr_1 locals=__local_validate_simple_converted__0,__local_validate_simple_converted__1,__local_validate_simple_converted__2,__local_validate_simple_converted__3,__local_validate_simple_converted__4,__local_validate_simple_converted__5,__local_validate_simple_converted__6,__local_validate_simple_converted__7,__local_validate_simple_converted__ptr,__local_validate_simple_converted__8 indirect=false in_cycle=false param_regs=A,X
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call validate_nested_partial -> strcmp
; item validate_simple section=.text local=false zero=false
; item validate_simple_converted section=.text local=false zero=false
; item validate_nested section=.text local=false zero=false
; item validate_nested_partial section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

validate_simple:
   SUBROUTINE

   STA   __local_validate_simple__ptr
   STX   __local_validate_simple__8
   LDY   #$00
   LDA   (__local_validate_simple__ptr),Y
   STA   __local_validate_simple__7
   INY
   LDA   (__local_validate_simple__ptr),Y
   STA   __local_validate_simple__6
   INY
   LDA   (__local_validate_simple__ptr),Y
   STA   __local_validate_simple__5
   INY
   LDA   (__local_validate_simple__ptr),Y
   STA   __local_validate_simple__4
   INY
   LDA   (__local_validate_simple__ptr),Y
   STA   __local_validate_simple__3
   INY
   LDA   (__local_validate_simple__ptr),Y
   STA   __local_validate_simple__2
   INY
   LDA   (__local_validate_simple__ptr),Y
   STA   __local_validate_simple__1
   INY
   LDA   (__local_validate_simple__ptr),Y
   CMP   #$40
   BNE   .thread@0
   LDA   __local_validate_simple__1
   CMP   #$5E
   BNE   .thread@0
   LDA   __local_validate_simple__2
   CMP   #$DC
   BNE   .thread@0
   LDA   __local_validate_simple__3
   CMP   #$CC
   BNE   .thread@0
   LDA   __local_validate_simple__4
   CMP   #$CC
   BNE   .thread@0
   LDA   __local_validate_simple__5
   CMP   #$CC
   BNE   .thread@0
   LDA   __local_validate_simple__6
   CMP   #$CC
   BNE   .thread@0
   LDA   __local_validate_simple__7
   CMP   #$CD
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

validate_simple_converted:
   SUBROUTINE

   STA   __local_validate_simple_converted__ptr
   STX   __local_validate_simple_converted__8
   LDY   #$00
   LDA   (__local_validate_simple_converted__ptr),Y
   STA   __local_validate_simple_converted__7
   INY
   LDA   (__local_validate_simple_converted__ptr),Y
   STA   __local_validate_simple_converted__6
   INY
   LDA   (__local_validate_simple_converted__ptr),Y
   STA   __local_validate_simple_converted__5
   INY
   LDA   (__local_validate_simple_converted__ptr),Y
   STA   __local_validate_simple_converted__4
   INY
   LDA   (__local_validate_simple_converted__ptr),Y
   STA   __local_validate_simple_converted__3
   INY
   LDA   (__local_validate_simple_converted__ptr),Y
   STA   __local_validate_simple_converted__2
   INY
   LDA   (__local_validate_simple_converted__ptr),Y
   STA   __local_validate_simple_converted__1
   INY
   LDA   (__local_validate_simple_converted__ptr),Y
   CMP   #$43
   BNE   .thread@1
   LDA   __local_validate_simple_converted__1
   CMP   #$EF
   BNE   .thread@1
   LDA   __local_validate_simple_converted__2
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_validate_simple_converted__3
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_validate_simple_converted__4
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_validate_simple_converted__5
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_validate_simple_converted__6
   CMP   #$FF
   BNE   .thread@1
   LDA   __local_validate_simple_converted__7
   CMP   #$FF
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

validate_nested:
   SUBROUTINE

   STA   __local_validate_nested__ptr
   STX   __local_validate_nested__6
   LDY   #$03
   LDA   (__local_validate_nested__ptr),Y
   BMI   .jcmp_skip@8
   LDA   #$00
   BEQ   .jcmp_skip@10
.jcmp_skip@8:
   LDX   #$00
   TXA
   RTS
.loop@0_start:
   LDA   __local_validate_nested__i_0
   SEC
   SBC   #$09
   LDA   __local_validate_nested__i_1
   SBC   #$00
   BVC   .jcmp_novf@9
   EOR   #$80
.jcmp_novf@9:
   BPL   .loop@0_break
   LDA   __local_validate_nested__i_0
   CLC
   ADC   __local_validate_nested__ptr
   STA   __local_validate_nested__0
   LDA   #$00
   ADC   __local_validate_nested__6
   STA   __local_validate_nested__1
   LDY   #$04
   LDA   (__local_validate_nested__0),Y
   STA   __local_validate_nested__2
   LDA   #$FF
   SEC
   SBC   __local_validate_nested__i_0
   STA   __local_validate_nested__1
   LDA   #$FF
   SBC   __local_validate_nested__i_1
   STA   __local_validate_nested__0
   LDA   #$00
   CMP   __local_validate_nested__0
   BNE   .jcmp_skip@10
   LDA   __local_validate_nested__2
   CMP   __local_validate_nested__1
   BEQ   .if_end@1
.jcmp_skip@10:
   LDX   #$00
   RTS
.if_end@1:
   INC   __local_validate_nested__i_0
   BNE   .loop@0_start
   INC   __local_validate_nested__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$00
   LDA   #$01
   RTS

validate_nested_partial:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $73, $74, $72, $69, $6E, $67, $00
