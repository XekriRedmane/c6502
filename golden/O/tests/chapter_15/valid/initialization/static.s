SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_test_global__0	EQU	$82
__local_test_local__0	EQU	$82
__local_test_global__1	EQU	$83
__local_test_local__1	EQU	$83
__local_test_local__local_double_arr	EQU	$84
__local_check_long_arr__0	EQU	$9C
__local_check_uint_arr__0	EQU	$9C
__local_check_double_arr__1	EQU	$9D
__local_check_long_arr__1	EQU	$9D
__local_check_uint_arr__1	EQU	$9D
__local_check_ulong_arr__1	EQU	$9D
__local_check_double_arr__2	EQU	$9E
__local_check_long_arr__2	EQU	$9E
__local_check_uint_arr__2	EQU	$9E
__local_check_ulong_arr__2	EQU	$9E
__local_check_double_arr__3	EQU	$9F
__local_check_uint_arr__3	EQU	$9F
__local_check_ulong_arr__3	EQU	$9F
__local_check_double_arr__4	EQU	$A0
__local_check_long_arr__4	EQU	$A0
__local_check_uint_arr__arr	EQU	$A0
__local_check_ulong_arr__4	EQU	$A0
__local_check_double_arr__5	EQU	$A1
__local_check_long_arr__5	EQU	$A1
__local_check_uint_arr__4	EQU	$A1
__local_check_ulong_arr__5	EQU	$A1
__local_check_double_arr__6	EQU	$A2
__local_check_long_arr__i	EQU	$A2
__local_check_ulong_arr__6	EQU	$A2
__local_check_double_arr__7	EQU	$A3
__local_check_long_arr__arr_1	EQU	$A3
__local_check_ulong_arr__7	EQU	$A3
__local_check_double_arr__arr	EQU	$A4
__local_check_long_arr__arr_0	EQU	$A4
__local_check_ulong_arr__arr	EQU	$A4
__local_check_double_arr__8	EQU	$A5
__local_check_ulong_arr__8	EQU	$A5

; @zp-link-meta-begin
; def check_double_arr params=__zpabi_check_double_arr__arr_0,__zpabi_check_double_arr__arr_1 locals=__local_check_double_arr__0,__local_check_double_arr__1,__local_check_double_arr__2,__local_check_double_arr__3,__local_check_double_arr__4,__local_check_double_arr__5,__local_check_double_arr__6,__local_check_double_arr__7,__local_check_double_arr__arr,__local_check_double_arr__8 indirect=false in_cycle=false param_regs=A,X
; def check_long_arr params=__zpabi_check_long_arr__arr_0,__zpabi_check_long_arr__arr_1 locals=__local_check_long_arr__0,__local_check_long_arr__1,__local_check_long_arr__2,__local_check_long_arr__3,__local_check_long_arr__4,__local_check_long_arr__5,__local_check_long_arr__i,__local_check_long_arr__arr_1,__local_check_long_arr__arr_0 indirect=false in_cycle=false param_regs=A,X
; def check_uint_arr params=__zpabi_check_uint_arr__arr_0,__zpabi_check_uint_arr__arr_1 locals=__local_check_uint_arr__0,__local_check_uint_arr__1,__local_check_uint_arr__2,__local_check_uint_arr__3,__local_check_uint_arr__arr,__local_check_uint_arr__4 indirect=false in_cycle=false param_regs=A,X
; def check_ulong_arr params=__zpabi_check_ulong_arr__arr_0,__zpabi_check_ulong_arr__arr_1 locals=__local_check_ulong_arr__0,__local_check_ulong_arr__1,__local_check_ulong_arr__2,__local_check_ulong_arr__3,__local_check_ulong_arr__4,__local_check_ulong_arr__5,__local_check_ulong_arr__6,__local_check_ulong_arr__7,__local_check_ulong_arr__arr,__local_check_ulong_arr__8 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_global params= locals=__local_test_global__0,__local_test_global__1 indirect=false in_cycle=false
; def test_local params= locals=__local_test_local__0,__local_test_local__1,__local_test_local__local_double_arr,__local_test_local__3,__local_test_local__4,__local_test_local__5,__local_test_local__6,__local_test_local__7,__local_test_local__8,__local_test_local__9,__local_test_local__10,__local_test_local__11,__local_test_local__12,__local_test_local__13,__local_test_local__14,__local_test_local__15,__local_test_local__16,__local_test_local__17,__local_test_local__18,__local_test_local__19,__local_test_local__20,__local_test_local__21,__local_test_local__22,__local_test_local__23,__local_test_local__24,__local_test_local__25 indirect=false in_cycle=false
; call main -> test_global
; call main -> test_local
; call test_global -> check_double_arr
; call test_global -> check_long_arr
; call test_global -> check_uint_arr
; call test_global -> check_ulong_arr
; call test_local -> check_double_arr
; call test_local -> check_long_arr
; call test_local -> check_uint_arr
; call test_local -> check_ulong_arr
; item check_double_arr section=.text local=false zero=false
; item check_double_arr@tail0 section=.text local=true zero=false
; item check_uint_arr section=.text local=false zero=false
; item check_long_arr section=.text local=false zero=false
; item check_ulong_arr section=.text local=false zero=false
; item test_global section=.text local=false zero=false
; item test_local section=.text local=false zero=false
; item main section=.text local=false zero=false
; item double_arr section=.data local=false zero=false
; item uint_arr section=.data local=false zero=false
; item long_arr section=.bss local=false zero=true
; item ulong_arr section=.data local=false zero=false
; item @7.local_uint_arr section=.data local=true zero=false
; item @8.local_long_arr section=.bss local=true zero=true
; item @9.local_ulong_arr section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

check_double_arr:
   SUBROUTINE

   STA   __local_check_double_arr__arr
   STX   __local_check_double_arr__8
   LDY   #$00
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__7
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__6
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__5
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__4
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__3
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__2
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__1
   INY
   LDA   (__local_check_double_arr__arr),Y
   CMP   #$3F
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__1
   CMP   #$F0
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__2
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__3
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__4
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__5
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__6
   BNE   .jcmp_skip@0
   LDA   __local_check_double_arr__7
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDY   #$08
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__7
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__6
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__5
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__4
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__3
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__2
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__1
   INY
   LDA   (__local_check_double_arr__arr),Y
   CMP   #$40
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__1
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__2
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__3
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__4
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__5
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__6
   BNE   .jcmp_skip@1
   LDA   __local_check_double_arr__7
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDY   #$10
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__7
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__6
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__5
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__4
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__3
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__2
   INY
   LDA   (__local_check_double_arr__arr),Y
   STA   __local_check_double_arr__1
   INY
   LDA   (__local_check_double_arr__arr),Y
   CMP   #$40
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__1
   CMP   #$08
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__2
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__3
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__4
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__5
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__6
   BNE   .jcmp_skip@2
   LDA   __local_check_double_arr__7
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JMP   check_double_arr@tail0

check_double_arr@tail0:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

check_uint_arr:
   SUBROUTINE

   STA   __local_check_uint_arr__arr
   STX   __local_check_uint_arr__4
   LDY   #$00
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__3
   INY
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__2
   INY
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__1
   INY
   LDA   (__local_check_uint_arr__arr),Y
   BNE   .jcmp_skip@3
   LDA   __local_check_uint_arr__1
   BNE   .jcmp_skip@3
   LDA   __local_check_uint_arr__2
   BNE   .jcmp_skip@3
   LDA   __local_check_uint_arr__3
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDY   #$05
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__2
   INY
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__1
   INY
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__0
   LDY   #$04
   LDA   (__local_check_uint_arr__arr),Y
   ORA   __local_check_uint_arr__2
   ORA   __local_check_uint_arr__1
   ORA   __local_check_uint_arr__0
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDY   #$08
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__3
   INY
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__2
   INY
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__1
   INY
   LDA   (__local_check_uint_arr__arr),Y
   BNE   .jcmp_skip@4
   LDA   __local_check_uint_arr__1
   BNE   .jcmp_skip@4
   LDA   __local_check_uint_arr__2
   CMP   #$80
   BNE   .jcmp_skip@4
   LDA   __local_check_uint_arr__3
   CMP   #$0D
   BEQ   .if_end@5
.jcmp_skip@4:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDY   #$0D
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__2
   INY
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__1
   INY
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__0
   LDY   #$0C
   LDA   (__local_check_uint_arr__arr),Y
   ORA   __local_check_uint_arr__2
   ORA   __local_check_uint_arr__1
   ORA   __local_check_uint_arr__0
   BNE   .check_uint_arr@jthread@0
   LDY   #$11
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__2
   INY
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__1
   INY
   LDA   (__local_check_uint_arr__arr),Y
   STA   __local_check_uint_arr__0
   LDY   #$10
   LDA   (__local_check_uint_arr__arr),Y
   ORA   __local_check_uint_arr__2
   ORA   __local_check_uint_arr__1
   ORA   __local_check_uint_arr__0
   BEQ   .if_end@8
.check_uint_arr@jthread@0:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@8:
   JMP   check_double_arr@tail0

check_long_arr:
   SUBROUTINE

   STA   __local_check_long_arr__arr_0
   STX   __local_check_long_arr__arr_1
   LDA   #$00
   STA   __local_check_long_arr__i
   LDX   #$00
.loop@0_start:
   LDA   __local_check_long_arr__i
   SEC
   SBC   #$64
   TXA
   SBC   #$00
   BVC   .jcmp_novf@5
   EOR   #$80
.jcmp_novf@5:
   BPL   .loop@0_break
   LDA   __local_check_long_arr__i
   ASL
   STA   __local_check_long_arr__0
   STX   __local_check_long_arr__1
   ROL   __local_check_long_arr__1
   ASL   __local_check_long_arr__0
   ROL   __local_check_long_arr__1
   LDA   __local_check_long_arr__arr_0
   CLC
   ADC   __local_check_long_arr__0
   STA   __local_check_long_arr__4
   LDA   __local_check_long_arr__arr_1
   ADC   __local_check_long_arr__1
   STA   __local_check_long_arr__5
   LDY   #$01
   LDA   (__local_check_long_arr__4),Y
   STA   __local_check_long_arr__2
   INY
   LDA   (__local_check_long_arr__4),Y
   STA   __local_check_long_arr__1
   INY
   LDA   (__local_check_long_arr__4),Y
   STA   __local_check_long_arr__0
   LDY   #$00
   LDA   (__local_check_long_arr__4),Y
   ORA   __local_check_long_arr__2
   ORA   __local_check_long_arr__1
   ORA   __local_check_long_arr__0
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$08
   RTS
.if_end@9:
   INC   __local_check_long_arr__i
   BNE   .loop@0_start
   INX
   JMP   .loop@0_start
.loop@0_break:
   JMP   check_double_arr@tail0

check_ulong_arr:
   SUBROUTINE

   STA   __local_check_ulong_arr__arr
   STX   __local_check_ulong_arr__8
   LDY   #$00
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__7
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__6
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__5
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__4
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__3
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__2
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__1
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   BNE   .jcmp_skip@6
   LDA   __local_check_ulong_arr__1
   BNE   .jcmp_skip@6
   LDA   __local_check_ulong_arr__2
   BNE   .jcmp_skip@6
   LDA   __local_check_ulong_arr__3
   BNE   .jcmp_skip@6
   LDA   __local_check_ulong_arr__4
   BNE   .jcmp_skip@6
   LDA   __local_check_ulong_arr__5
   BNE   .jcmp_skip@6
   LDA   __local_check_ulong_arr__6
   BNE   .jcmp_skip@6
   LDA   __local_check_ulong_arr__7
   CMP   #$64
   BEQ   .if_end@10
.jcmp_skip@6:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@10:
   LDY   #$08
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__7
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__6
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__5
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__4
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__3
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__2
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__1
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   BNE   .jcmp_skip@7
   LDA   __local_check_ulong_arr__1
   BNE   .jcmp_skip@7
   LDA   __local_check_ulong_arr__2
   BNE   .jcmp_skip@7
   LDA   __local_check_ulong_arr__3
   BNE   .jcmp_skip@7
   LDA   __local_check_ulong_arr__4
   BNE   .jcmp_skip@7
   LDA   __local_check_ulong_arr__5
   BNE   .jcmp_skip@7
   LDA   __local_check_ulong_arr__6
   BNE   .jcmp_skip@7
   LDA   __local_check_ulong_arr__7
   CMP   #$0B
   BEQ   .if_end@11
.jcmp_skip@7:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@11:
   LDY   #$10
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__7
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__6
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__5
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__4
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__3
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__2
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__1
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   BNE   .jcmp_skip@8
   LDA   __local_check_ulong_arr__1
   BNE   .jcmp_skip@8
   LDA   __local_check_ulong_arr__2
   BNE   .jcmp_skip@8
   LDA   __local_check_ulong_arr__3
   BNE   .jcmp_skip@8
   LDA   __local_check_ulong_arr__4
   BNE   .jcmp_skip@8
   LDA   __local_check_ulong_arr__5
   BNE   .jcmp_skip@8
   LDA   __local_check_ulong_arr__6
   CMP   #$30
   BNE   .jcmp_skip@8
   LDA   __local_check_ulong_arr__7
   CMP   #$39
   BEQ   .if_end@12
.jcmp_skip@8:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@12:
   LDY   #$18
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__7
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__6
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__5
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__4
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__3
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__2
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   STA   __local_check_ulong_arr__1
   INY
   LDA   (__local_check_ulong_arr__arr),Y
   BNE   .jcmp_skip@9
   LDA   __local_check_ulong_arr__1
   BNE   .jcmp_skip@9
   LDA   __local_check_ulong_arr__2
   BNE   .jcmp_skip@9
   LDA   __local_check_ulong_arr__3
   BNE   .jcmp_skip@9
   LDA   __local_check_ulong_arr__4
   BNE   .jcmp_skip@9
   LDA   __local_check_ulong_arr__5
   BNE   .jcmp_skip@9
   LDA   __local_check_ulong_arr__6
   CMP   #$FF
   BNE   .jcmp_skip@9
   LDA   __local_check_ulong_arr__7
   CMP   #$FF
   BEQ   .if_end@13
.jcmp_skip@9:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@13:
   JMP   check_double_arr@tail0

test_global:
   SUBROUTINE

   LDX   #>double_arr
   LDA   #<double_arr
   JSR   check_double_arr
   STA   __local_test_global__1
   STX   __local_test_global__0
   ORA   __local_test_global__0
   BNE   .tail@0
   LDX   #>uint_arr
   LDA   #<uint_arr
   JSR   check_uint_arr
   STA   __local_test_global__1
   STX   __local_test_global__0
   ORA   __local_test_global__0
   BNE   .tail@0
   LDX   #>long_arr
   LDA   #<long_arr
   JSR   check_long_arr
   STA   __local_test_global__1
   STX   __local_test_global__0
   ORA   __local_test_global__0
   BNE   .tail@0
   LDX   #>ulong_arr
   LDA   #<ulong_arr
   JSR   check_ulong_arr
   STA   __local_test_global__1
   STX   __local_test_global__0
   ORA   __local_test_global__0
   BEQ   .if_end@17
.tail@0:
   LDA   __local_test_global__1
   RTS
.if_end@17:
   JMP   check_double_arr@tail0

test_local:
   SUBROUTINE

   LDA   #$00
   STA   __local_test_local__local_double_arr
   LDY   #$00
   STY   __local_test_local__local_double_arr+1
   STY   __local_test_local__local_double_arr+2
   STY   __local_test_local__local_double_arr+3
   STY   __local_test_local__local_double_arr+4
   STA   __local_test_local__local_double_arr+5
   LDA   #$F0
   STA   __local_test_local__local_double_arr+6
   LDA   #$3F
   STA   __local_test_local__local_double_arr+7
   TYA
   STA   __local_test_local__local_double_arr+8
   STY   __local_test_local__local_double_arr+9
   STY   __local_test_local__local_double_arr+10
   STY   __local_test_local__local_double_arr+11
   STY   __local_test_local__local_double_arr+12
   STY   __local_test_local__local_double_arr+13
   STA   __local_test_local__local_double_arr+14
   LDY   #$40
   STY   __local_test_local__local_double_arr+15
   LDY   #$00
   STY   __local_test_local__local_double_arr+16
   STY   __local_test_local__local_double_arr+17
   STY   __local_test_local__local_double_arr+18
   STY   __local_test_local__local_double_arr+19
   STY   __local_test_local__local_double_arr+20
   STA   __local_test_local__local_double_arr+21
   LDA   #$08
   STA   __local_test_local__local_double_arr+22
   LDA   #$40
   STA   __local_test_local__local_double_arr+23
   LDX   #>__local_test_local__local_double_arr
   LDA   #<__local_test_local__local_double_arr
   JSR   check_double_arr
   STA   __local_test_local__0
   STX   __local_test_local__1
   ORA   __local_test_local__1
   BEQ   .if_end@18
   LDA   #$64
   CLC
   ADC   __local_test_local__0
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   RTS
.if_end@18:
   LDX   #>@7.local_uint_arr
   LDA   #<@7.local_uint_arr
   JSR   check_uint_arr
   STA   __local_test_local__0
   STX   __local_test_local__1
   ORA   __local_test_local__1
   BEQ   .if_end@19
   LDA   #$64
   CLC
   ADC   __local_test_local__0
   BCC   .carry_inc@1
   INX
.carry_inc@1:
   RTS
.if_end@19:
   LDX   #>@8.local_long_arr
   LDA   #<@8.local_long_arr
   JSR   check_long_arr
   STA   __local_test_local__0
   STX   __local_test_local__1
   ORA   __local_test_local__1
   BEQ   .if_end@20
   LDA   #$64
   CLC
   ADC   __local_test_local__0
   BCC   .carry_inc@2
   INX
.carry_inc@2:
   RTS
.if_end@20:
   LDX   #>@9.local_ulong_arr
   LDA   #<@9.local_ulong_arr
   JSR   check_ulong_arr
   STA   __local_test_local__0
   STX   __local_test_local__1
   ORA   __local_test_local__1
   BEQ   .if_end@21
   LDA   #$64
   CLC
   ADC   __local_test_local__0
   BCC   .carry_inc@3
   INX
.carry_inc@3:
   RTS
.if_end@21:
   JMP   check_double_arr@tail0

main:
   SUBROUTINE

   JSR   test_global
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@22
   LDA   __local_main__1
   RTS
.if_end@22:
   JMP   test_local

double_arr:
   DC.L  $00000000
   DC.L  $3FF00000
   DC.L  $00000000
   DC.L  $40000000
   DC.L  $00000000
   DC.L  $40080000

uint_arr:
   DC.L  $00000001
   DS.B  4
   DC.L  $0000800D
   DS.B  8

long_arr:
   DS.B  400

ulong_arr:
   DC.L  $00000064
   DC.L  $00000000
   DC.L  $0000000B
   DC.L  $00000000
   DC.L  $00003039
   DC.L  $00000000
   DC.L  $0000FFFF
   DC.L  $00000000

@7.local_uint_arr:
   DC.L  $00000001
   DS.B  4
   DC.L  $0000800D
   DS.B  8

@8.local_long_arr:
   DS.B  400

@9.local_ulong_arr:
   DC.L  $00000064
   DC.L  $00000000
   DC.L  $0000000B
   DC.L  $00000000
   DC.L  $00003039
   DC.L  $00000000
   DC.L  $0000FFFF
   DC.L  $00000000
