SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_malloc__size_0	EQU	$80
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_malloc__size_1	EQU	$81
__zpabi_strcmp__s2_1	EQU	$81
__local_main__0	EQU	$82
__zpabi_malloc__size_2	EQU	$82
__zpabi_malloc__size_3	EQU	$83
__local_test_auto__x_8	EQU	$84
__local_test_conditional__y_8	EQU	$84
__local_test_static__0	EQU	$84
__local_test_wonky_size__x_6	EQU	$84
__local_test_auto__x_7	EQU	$85
__local_test_conditional__y_7	EQU	$85
__local_test_static__1	EQU	$85
__local_test_auto__x_6	EQU	$86
__local_test_conditional__y_6	EQU	$86
__local_test_static__2	EQU	$86
__local_test_auto__x_5	EQU	$87
__local_test_conditional__y_5	EQU	$87
__local_test_static__3	EQU	$87
__local_test_auto__x_4	EQU	$88
__local_test_conditional__y_4	EQU	$88
__local_test_auto__x_3	EQU	$89
__local_test_conditional__y_3	EQU	$89
__local_test_conditional__y_2	EQU	$8A
__local_test_conditional__y_1	EQU	$8B
__local_test_conditional__y_0	EQU	$8C
__local_test_auto__y	EQU	$8D
__local_test_conditional__0	EQU	$8D
__local_test_conditional__z	EQU	$8F

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_auto params= locals=__local_test_auto__x_8,__local_test_auto__x_7,__local_test_auto__x_6,__local_test_auto__x_5,__local_test_auto__x_4,__local_test_auto__x_3,__local_test_auto__x_2,__local_test_auto__x_1,__local_test_auto__x_0,__local_test_auto__y,__local_test_auto__1,__local_test_auto__2,__local_test_auto__3,__local_test_auto__4,__local_test_auto__5,__local_test_auto__6,__local_test_auto__7,__local_test_auto__8 indirect=false in_cycle=false
; def test_conditional params= locals=__local_test_conditional__y_8,__local_test_conditional__y_7,__local_test_conditional__y_6,__local_test_conditional__y_5,__local_test_conditional__y_4,__local_test_conditional__y_3,__local_test_conditional__y_2,__local_test_conditional__y_1,__local_test_conditional__y_0,__local_test_conditional__0,__local_test_conditional__1,__local_test_conditional__z,__local_test_conditional__3,__local_test_conditional__4,__local_test_conditional__5,__local_test_conditional__6,__local_test_conditional__7,__local_test_conditional__8,__local_test_conditional__9,__local_test_conditional__10,__local_test_conditional__11,__local_test_conditional__12 indirect=false in_cycle=false
; def test_static params= locals=__local_test_static__0,__local_test_static__1,__local_test_static__2,__local_test_static__3 indirect=false in_cycle=false
; def test_wonky_size params= locals=__local_test_wonky_size__x_6,__local_test_wonky_size__x_5,__local_test_wonky_size__x_4,__local_test_wonky_size__x_3,__local_test_wonky_size__x_2,__local_test_wonky_size__x_1,__local_test_wonky_size__x_0 indirect=false in_cycle=false
; def true_flag params= locals= indirect=false in_cycle=false
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call main -> test_auto
; call main -> test_conditional
; call main -> test_static
; call main -> test_wonky_size
; call test_auto -> strcmp
; call test_conditional -> strcmp
; call test_conditional -> true_flag
; call test_static -> strcmp
; call test_wonky_size -> strcmp
; item test_auto section=.text local=false zero=false
; item test_auto@tail1 section=.text local=true zero=false
; item test_auto@tail0 section=.text local=true zero=false
; item test_static section=.text local=false zero=false
; item test_wonky_size section=.text local=false zero=false
; item true_flag section=.text local=false zero=false
; item test_conditional section=.text local=false zero=false
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item str@3 section=.rodata local=true zero=false
; item @5.x section=.data local=true zero=false
; item @6.y section=.bss local=true zero=true
; item @8.y section=.bss local=true zero=true
; item @9.x section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_auto:
   SUBROUTINE

   LDA   #$FF
   STA   __local_test_auto__x_3
   STA   __local_test_auto__x_4
   LDY   #$00
   STY   __local_test_auto__y+6
   STY   __local_test_auto__y+7
   LDA   #$61
   STA   __local_test_auto__y
   LDA   #$62
   STA   __local_test_auto__y+1
   STY   __local_test_auto__y+2
   LDA   #$FF
   STA   __local_test_auto__y+3
   STA   __local_test_auto__y+4
   LDA   #$02
   STA   __local_test_auto__y+5
   STY   __local_test_auto__y+8
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDX   #>__local_test_auto__y
   LDA   #<__local_test_auto__y
   JSR   strcmp
   STX   __local_test_auto__x_8
   ORA   __local_test_auto__x_8
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__y+3
   STA   __local_test_auto__x_7
   LDA   __local_test_auto__y+4
   CMP   #$FF
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__x_7
   CMP   #$FF
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__y+5
   STA   __local_test_auto__x_5
   LDA   __local_test_auto__y+6
   STA   __local_test_auto__x_6
   LDA   __local_test_auto__y+7
   STA   __local_test_auto__x_7
   LDA   __local_test_auto__y+8
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__x_7
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__x_6
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__x_5
   CMP   #$02
   BNE   .test_auto@jthread@2
   LDY   #$00
   BNE   .test_auto@jthread@2
   LDA   #$14
   CMP   #$14
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__x_4
   CMP   #$FF
   BNE   .test_auto@jthread@2
   LDA   __local_test_auto__x_3
   CMP   #$FF
   BEQ   .if_end@7
.test_auto@jthread@2:
   JMP   test_auto@tail1
.if_end@7:
   JMP   test_auto@tail0

test_auto@tail1:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_auto@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_static:
   SUBROUTINE

   LDA   @5.x
   STA   @6.y
   LDA   @5.x+1
   STA   @6.y+1
   LDA   @5.x+2
   STA   @6.y+2
   LDA   @5.x+3
   STA   @6.y+3
   LDA   @5.x+4
   STA   @6.y+4
   LDA   @5.x+5
   STA   @6.y+5
   LDA   @5.x+6
   STA   @6.y+6
   LDA   @5.x+7
   STA   @6.y+7
   LDA   @5.x+8
   STA   @6.y+8
   LDA   #<str@1
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@1
   STA   __zpabi_strcmp__s2_1
   LDX   #>@6.y
   LDA   #<@6.y
   JSR   strcmp
   STX   __local_test_static__0
   ORA   __local_test_static__0
   BNE   .test_static@jthread@1
   LDA   @6.y+3
   STA   __local_test_static__1
   LDA   @6.y+4
   BNE   .test_static@jthread@1
   LDA   __local_test_static__1
   CMP   #$01
   BNE   .test_static@jthread@1
   LDA   @6.y+5
   STA   __local_test_static__3
   LDA   @6.y+6
   STA   __local_test_static__2
   LDA   @6.y+7
   STA   __local_test_static__1
   LDA   @6.y+8
   BNE   .test_static@jthread@1
   LDA   __local_test_static__1
   BNE   .test_static@jthread@1
   LDA   __local_test_static__2
   BNE   .test_static@jthread@1
   LDA   __local_test_static__3
   CMP   #$02
   BEQ   .if_end@12
.test_static@jthread@1:
   JMP   test_auto@tail1
.if_end@12:
   JMP   test_auto@tail0

test_wonky_size:
   SUBROUTINE

   LDA   #$61
   STA   @8.y
   LDA   #$62
   STA   @8.y+1
   LDA   #$63
   STA   @8.y+2
   LDA   #$64
   STA   @8.y+3
   LDA   #$65
   STA   @8.y+4
   LDA   #$66
   STA   @8.y+5
   LDA   #$00
   STA   @8.y+6
   LDA   #<str@2
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@2
   STA   __zpabi_strcmp__s2_1
   LDX   #>@8.y
   LDA   #<@8.y
   JSR   strcmp
   STX   __local_test_wonky_size__x_6
   ORA   __local_test_wonky_size__x_6
   BEQ   .if_end@13
   JMP   test_auto@tail1
.if_end@13:
   JMP   test_auto@tail0

true_flag:
   SUBROUTINE

   JMP   test_auto@tail0

test_conditional:
   SUBROUTINE

   LDA   #$21
   STA   __local_test_conditional__y_0
   LDA   #$F6
   STA   __local_test_conditional__y_3
   LDA   #$FF
   STA   __local_test_conditional__y_4
   LDA   #$00
   STA   __local_test_conditional__y_1
   STA   __local_test_conditional__y_2
   STA   __local_test_conditional__y_5
   LDY   #$00
   STY   __local_test_conditional__y_6
   STY   __local_test_conditional__y_7
   STA   __local_test_conditional__y_8
   JSR   true_flag
   STX   __local_test_conditional__0
   ORA   __local_test_conditional__0
   BEQ   .cond_else@14
   LDA   @9.x
   STA   __local_test_conditional__y_0
   LDA   @9.x+1
   STA   __local_test_conditional__y_1
   LDA   @9.x+2
   STA   __local_test_conditional__y_2
   LDA   @9.x+3
   STA   __local_test_conditional__y_3
   LDA   @9.x+4
   STA   __local_test_conditional__y_4
   LDA   @9.x+5
   STA   __local_test_conditional__y_5
   LDA   @9.x+6
   STA   __local_test_conditional__y_6
   LDA   @9.x+7
   STA   __local_test_conditional__y_7
   LDA   @9.x+8
   STA   __local_test_conditional__y_8
.cond_else@14:
   LDA   __local_test_conditional__y_0
   STA   __local_test_conditional__z
   LDA   __local_test_conditional__y_1
   STA   __local_test_conditional__z+1
   LDA   __local_test_conditional__y_2
   STA   __local_test_conditional__z+2
   LDA   __local_test_conditional__y_3
   STA   __local_test_conditional__z+3
   LDA   __local_test_conditional__y_4
   STA   __local_test_conditional__z+4
   LDA   __local_test_conditional__y_5
   STA   __local_test_conditional__z+5
   LDA   __local_test_conditional__y_6
   STA   __local_test_conditional__z+6
   LDA   __local_test_conditional__y_7
   STA   __local_test_conditional__z+7
   LDA   __local_test_conditional__y_8
   STA   __local_test_conditional__z+8
   LDA   #<str@3
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@3
   STA   __zpabi_strcmp__s2_1
   LDX   #>__local_test_conditional__z
   LDA   #<__local_test_conditional__z
   JSR   strcmp
   STX   __local_test_conditional__y_8
   ORA   __local_test_conditional__y_8
   BNE   .test_conditional@jthread@1
   LDA   __local_test_conditional__z+3
   STA   __local_test_conditional__y_7
   LDA   __local_test_conditional__z+4
   CMP   #$04
   BNE   .test_conditional@jthread@1
   LDA   __local_test_conditional__y_7
   CMP   #$D2
   BNE   .test_conditional@jthread@1
   LDA   __local_test_conditional__z+5
   STA   __local_test_conditional__y_5
   LDA   __local_test_conditional__z+6
   STA   __local_test_conditional__y_6
   LDA   __local_test_conditional__z+7
   STA   __local_test_conditional__y_7
   LDA   __local_test_conditional__z+8
   BNE   .test_conditional@jthread@1
   LDA   __local_test_conditional__y_7
   BNE   .test_conditional@jthread@1
   LDA   __local_test_conditional__y_6
   CMP   #$16
   BNE   .test_conditional@jthread@1
   LDA   __local_test_conditional__y_5
   CMP   #$2E
   BEQ   .if_end@20
.test_conditional@jthread@1:
   JMP   test_auto@tail1
.if_end@20:
   JMP   test_auto@tail0

main:
   SUBROUTINE

   JSR   test_auto
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@21
   JMP   test_auto@tail0
.if_end@21:
   JSR   test_static
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@22
   LDX   #$00
   LDA   #$02
   RTS
.if_end@22:
   JSR   test_wonky_size
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@23
   LDX   #$00
   LDA   #$03
   RTS
.if_end@23:
   JSR   test_conditional
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@24
   LDX   #$00
   LDA   #$04
   RTS
.if_end@24:
   JMP   test_auto@tail1

str@0:
   DC.B  $61, $62, $00

str@1:
   DC.B  $61, $62, $00

str@2:
   DC.B  $61, $62, $63, $64, $65, $66, $00

str@3:
   DC.B  $78, $79, $00

@5.x:
   DC.B  $61, $62, $00
   DC.W  $0001
   DC.L  $00000002

@6.y:
   DS.B  9

@8.y:
   DS.B  7

@9.x:
   DC.B  $78, $79, $00
   DC.W  $04D2
   DC.L  $0000162E
