SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__i_1	EQU	$84
__local_main__i_0	EQU	$85
__local_main__j	EQU	$86
__local_main__4	EQU	$87
__local_main__5	EQU	$88
__local_main__6	EQU	$89
__local_main__7	EQU	$8A
__local_main__arr	EQU	$8B
__local_main__nested_arr	EQU	$9B
__local_array_param__a	EQU	$B3
__local_nested_array_param__a	EQU	$B3
__local_array_param__0	EQU	$B4
__local_nested_array_param__0	EQU	$B4

; @zp-link-meta-begin
; def array_param params=__zpabi_array_param__a_0,__zpabi_array_param__a_1 locals=__local_array_param__a,__local_array_param__0 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__i_1,__local_main__i_0,__local_main__j,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__arr,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__nested_arr,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37,__local_main__38,__local_main__39,__local_main__40,__local_main__41,__local_main__42,__local_main__43,__local_main__44,__local_main__45,__local_main__46,__local_main__47 indirect=false in_cycle=false
; def nested_array_param params=__zpabi_nested_array_param__a_0,__zpabi_nested_array_param__a_1 locals=__local_nested_array_param__a,__local_nested_array_param__0 indirect=false in_cycle=false param_regs=A,X
; call main -> array_param
; call main -> nested_array_param
; item array_param section=.text local=false zero=false
; item array_param@tail0 section=.text local=true zero=false
; item nested_array_param section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

array_param:
   SUBROUTINE

   STA   __local_array_param__a
   STX   __local_array_param__0
   STA   DPTR
   STX   DPTR+1
   LDA   #$00
   LDY   #$08
   STA   (__local_array_param__a),Y
   JMP   array_param@tail0

array_param@tail0:
   SUBROUTINE

   LDY   #$09
   STA   (DPTR),Y
   LDX   #$00
   RTS

nested_array_param:
   SUBROUTINE

   STA   __local_nested_array_param__a
   STX   __local_nested_array_param__0
   STA   DPTR
   STX   DPTR+1
   LDA   #$01
   LDY   #$08
   STA   (__local_nested_array_param__a),Y
   LDA   #$00
   JMP   array_param@tail0

main:
   SUBROUTINE

   LDA   #$08
   STA   __local_main__arr
   LDA   #$00
   STA   __local_main__arr+1
   LDY   #$07
   STY   __local_main__arr+2
   STA   __local_main__arr+3
   DEY
   STY   __local_main__arr+4
   STA   __local_main__arr+5
   DEY
   STY   __local_main__arr+6
   STA   __local_main__arr+7
   DEY
   STY   __local_main__arr+8
   STA   __local_main__arr+9
   DEY
   STY   __local_main__arr+10
   STA   __local_main__arr+11
   DEY
   STY   __local_main__arr+12
   STA   __local_main__arr+13
   DEY
   STY   __local_main__arr+14
   STA   __local_main__arr+15
   LDX   #>__local_main__arr
   LDA   #<__local_main__arr
   JSR   array_param
   LDA   __local_main__arr+8
   ORA   __local_main__arr+9
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$00
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$08
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_main__i_1
   BNE   .jcmp_skip@1
   LDA   __local_main__i_0
   CMP   #$04
   BEQ   .if_end@3
.jcmp_skip@1:
   LDA   __local_main__i_0
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
   LDA   #$08
   SEC
   SBC   __local_main__i_0
   STA   __local_main__1
   LDA   #$00
   SBC   __local_main__i_1
   STA   __local_main__0
   LDA   __local_main__2
   CMP   __local_main__0
   BNE   .jcmp_skip@2
   LDA   __local_main__3
   CMP   __local_main__1
   BEQ   .if_end@3
.jcmp_skip@2:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   INC   __local_main__i_0
   BNE   .loop@0_start
   INC   __local_main__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$FF
   STA   __local_main__nested_arr
   LDY   #$FF
   STY   __local_main__nested_arr+1
   STY   __local_main__nested_arr+2
   STY   __local_main__nested_arr+3
   STY   __local_main__nested_arr+4
   STA   __local_main__nested_arr+5
   DEY
   STY   __local_main__nested_arr+6
   STA   __local_main__nested_arr+7
   STY   __local_main__nested_arr+8
   STA   __local_main__nested_arr+9
   STY   __local_main__nested_arr+10
   STA   __local_main__nested_arr+11
   DEY
   STY   __local_main__nested_arr+12
   STA   __local_main__nested_arr+13
   STY   __local_main__nested_arr+14
   STA   __local_main__nested_arr+15
   STY   __local_main__nested_arr+16
   STA   __local_main__nested_arr+17
   DEY
   STY   __local_main__nested_arr+18
   STA   __local_main__nested_arr+19
   STY   __local_main__nested_arr+20
   STA   __local_main__nested_arr+21
   STY   __local_main__nested_arr+22
   STA   __local_main__nested_arr+23
   LDX   #>__local_main__nested_arr
   LDA   #<__local_main__nested_arr
   JSR   nested_array_param
   LDA   __local_main__nested_arr+8
   STA   __local_main__1
   LDA   __local_main__nested_arr+9
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@3:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@4:
   LDA   #$00
   STA   __local_main__7
   LDY   #$00
   STY   __local_main__6
.loop@1_start:
   LDA   __local_main__7
   SEC
   SBC   #$04
   LDA   __local_main__6
   SBC   #$00
   BVC   .jcmp_novf@4
   EOR   #$80
.jcmp_novf@4:
   BMI   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDA   #$FF
   SEC
   SBC   __local_main__7
   STA   __local_main__5
   LDA   #$FF
   SBC   __local_main__6
   STA   __local_main__4
   LDA   #$00
   STA   __local_main__j
   LDX   #$00
.loop@2_start:
   LDA   __local_main__j
   SEC
   SBC   #$03
   TXA
   SBC   #$00
   BVC   .jcmp_novf@5
   EOR   #$80
.jcmp_novf@5:
   BPL   .loop@2_break
   LDA   __local_main__6
   BNE   .main@jthread@1
   LDA   __local_main__7
   CMP   #$01
   BNE   .main@jthread@1
   CPX   #$00
   BNE   .main@jthread@1
   LDA   __local_main__j
   CMP   #$01
   BEQ   .if_end@9
.main@jthread@1:
   LDA   #>__local_main__nested_arr
   STA   __local_main__i_0
   LDA   __local_main__7
   ASL
   STA   __local_main__1
   ASL
   CLC
   ADC   __local_main__1
   STA   __local_main__0
   LDA   #<__local_main__nested_arr
   CLC
   ADC   __local_main__0
   STA   __local_main__1
   BCC   .carry_inc@0
   INC   __local_main__i_0
.carry_inc@0:
   LDA   __local_main__j
   ASL
   CLC
   ADC   __local_main__1
   STA   __local_main__2
   LDA   __local_main__i_0
   ADC   #$00
   STA   __local_main__3
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   CMP   __local_main__4
   BNE   .jcmp_skip@6
   LDA   __local_main__1
   CMP   __local_main__5
   BEQ   .if_end@9
.jcmp_skip@6:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@9:
   INC   __local_main__j
   BNE   .loop@2_start
   INX
   JMP   .loop@2_start
.loop@2_break:
   INC   __local_main__7
   BEQ   .lb_skip@0
   JMP   .loop@1_start
.lb_skip@0:
   INC   __local_main__6
   JMP   .loop@1_start
.loop@1_break:
   LDX   #$00
   TXA
   RTS
