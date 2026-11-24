SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__local_test_nested__x	EQU	$84
__local_test_nested_partial_init__x	EQU	$84
__local_test_simple__x	EQU	$84
__local_test_simple_converted__x	EQU	$84

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_nested params= locals=__local_test_nested__0,__local_test_nested__1,__local_test_nested__x,__local_test_nested__3,__local_test_nested__4,__local_test_nested__5,__local_test_nested__6,__local_test_nested__7,__local_test_nested__8,__local_test_nested__9,__local_test_nested__10,__local_test_nested__11,__local_test_nested__12,__local_test_nested__13,__local_test_nested__14,__local_test_nested__15,__local_test_nested__16 indirect=false in_cycle=false
; def test_nested_partial_init params= locals=__local_test_nested_partial_init__0,__local_test_nested_partial_init__1,__local_test_nested_partial_init__x,__local_test_nested_partial_init__3,__local_test_nested_partial_init__4,__local_test_nested_partial_init__5,__local_test_nested_partial_init__6,__local_test_nested_partial_init__7,__local_test_nested_partial_init__8,__local_test_nested_partial_init__9,__local_test_nested_partial_init__10,__local_test_nested_partial_init__11,__local_test_nested_partial_init__12,__local_test_nested_partial_init__13,__local_test_nested_partial_init__14,__local_test_nested_partial_init__15,__local_test_nested_partial_init__16 indirect=false in_cycle=false
; def test_simple params= locals=__local_test_simple__0,__local_test_simple__1,__local_test_simple__x,__local_test_simple__3,__local_test_simple__4,__local_test_simple__5,__local_test_simple__6,__local_test_simple__7,__local_test_simple__8,__local_test_simple__9 indirect=false in_cycle=false
; def test_simple_converted params= locals=__local_test_simple_converted__0,__local_test_simple_converted__1,__local_test_simple_converted__x,__local_test_simple_converted__3,__local_test_simple_converted__4,__local_test_simple_converted__5,__local_test_simple_converted__6,__local_test_simple_converted__7,__local_test_simple_converted__8,__local_test_simple_converted__9 indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; ext validate_nested params=__zpabi_validate_nested__ptr_0,__zpabi_validate_nested__ptr_1 param_regs=A,X
; ext validate_nested_partial params=__zpabi_validate_nested_partial__ptr_0,__zpabi_validate_nested_partial__ptr_1 param_regs=A,X
; ext validate_simple params=__zpabi_validate_simple__ptr_0,__zpabi_validate_simple__ptr_1 param_regs=A,X
; ext validate_simple_converted params=__zpabi_validate_simple_converted__ptr_0,__zpabi_validate_simple_converted__ptr_1 param_regs=A,X
; call main -> test_nested
; call main -> test_nested_partial_init
; call main -> test_simple
; call main -> test_simple_converted
; call test_nested -> validate_nested
; call test_nested_partial_init -> validate_nested_partial
; call test_simple -> validate_simple
; call test_simple_converted -> validate_simple_converted
; item test_simple section=.text local=false zero=false
; item test_simple_converted section=.text local=false zero=false
; item test_nested section=.text local=false zero=false
; item test_nested_partial_init section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_simple:
   SUBROUTINE

   LDA   #$CD
   STA   __local_test_simple__x
   LDA   #$CC
   STA   __local_test_simple__x+1
   LDY   #$CC
   STY   __local_test_simple__x+2
   STY   __local_test_simple__x+3
   STA   __local_test_simple__x+4
   LDA   #$DC
   STA   __local_test_simple__x+5
   LDA   #$5E
   STA   __local_test_simple__x+6
   LDA   #$40
   STA   __local_test_simple__x+7
   LDX   #>__local_test_simple__x
   LDA   #<__local_test_simple__x
   JMP   validate_simple

test_simple_converted:
   SUBROUTINE

   LDA   #$00
   STA   __local_test_simple_converted__x
   STA   __local_test_simple_converted__x+1
   LDA   #$60
   STA   __local_test_simple_converted__x+2
   LDA   #$5D
   STA   __local_test_simple_converted__x+3
   LDA   #$FF
   STA   __local_test_simple_converted__x+4
   STA   __local_test_simple_converted__x+5
   LDA   #$EF
   STA   __local_test_simple_converted__x+6
   LDA   #$41
   STA   __local_test_simple_converted__x+7
   LDX   #>__local_test_simple_converted__x
   LDA   #<__local_test_simple_converted__x
   JMP   validate_simple_converted

test_nested:
   SUBROUTINE

   LDA   #$63
   STA   __local_test_nested__x
   LDA   #$00
   STA   __local_test_nested__x+1
   LDY   #$00
   STY   __local_test_nested__x+2
   STA   __local_test_nested__x+3
   LDA   #$FF
   STA   __local_test_nested__x+4
   LDA   #$FE
   STA   __local_test_nested__x+5
   LDA   #$FD
   STA   __local_test_nested__x+6
   LDA   #$FC
   STA   __local_test_nested__x+7
   LDA   #$FB
   STA   __local_test_nested__x+8
   LDA   #$FA
   STA   __local_test_nested__x+9
   LDA   #$F9
   STA   __local_test_nested__x+10
   LDA   #$F8
   STA   __local_test_nested__x+11
   LDA   #$F7
   STA   __local_test_nested__x+12
   TYA
   STA   __local_test_nested__x+13
   STA   __local_test_nested__x+14
   LDX   #>__local_test_nested__x
   LDA   #<__local_test_nested__x
   JMP   validate_nested

test_nested_partial_init:
   SUBROUTINE

   LDA   #$F8
   STA   __local_test_nested_partial_init__x
   LDA   #$7F
   STA   __local_test_nested_partial_init__x+1
   LDA   #$16
   STA   __local_test_nested_partial_init__x+2
   LDA   #$77
   STA   __local_test_nested_partial_init__x+3
   LDA   #$73
   STA   __local_test_nested_partial_init__x+4
   LDA   #$74
   STA   __local_test_nested_partial_init__x+5
   LDA   #$72
   STA   __local_test_nested_partial_init__x+6
   LDA   #$69
   STA   __local_test_nested_partial_init__x+7
   LDA   #$6E
   STA   __local_test_nested_partial_init__x+8
   LDA   #$67
   STA   __local_test_nested_partial_init__x+9
   LDA   #$00
   STA   __local_test_nested_partial_init__x+10
   LDY   #$00
   STY   __local_test_nested_partial_init__x+11
   STY   __local_test_nested_partial_init__x+12
   STY   __local_test_nested_partial_init__x+13
   STA   __local_test_nested_partial_init__x+14
   LDX   #>__local_test_nested_partial_init__x
   LDA   #<__local_test_nested_partial_init__x
   JMP   validate_nested_partial

main:
   SUBROUTINE

   JSR   test_simple
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   test_simple_converted
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   test_nested
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   test_nested_partial_init
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
