SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__local_main__0	EQU	$82
__local_test_nested_auto_without_null_byte__0	EQU	$84
__local_test_nested_static_without_null_byte__0	EQU	$84
__local_test_nested_auto_without_null_byte__2	EQU	$86
__local_test_nested_static_without_null_byte__2	EQU	$86
__local_test_nested_static_without_null_byte__3	EQU	$87
__local_test_nested_auto_without_null_byte__4	EQU	$88
__local_test_nested_static_without_null_byte__4	EQU	$88
__local_test_nested_auto_without_null_byte__5	EQU	$89
__local_test_nested_static_without_null_byte__5	EQU	$89
__local_test_nested_static_without_null_byte__6	EQU	$8A
__local_test_nested_auto_without_null_byte__7	EQU	$8B
__local_test_nested_static_without_null_byte__7	EQU	$8B
__local_test_nested_auto_without_null_byte__8	EQU	$8C
__local_test_nested_auto_without_null_byte__10	EQU	$8E
__local_test_nested_auto_without_null_byte__nested	EQU	$8F

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_flat_auto_with_null_byte params= locals=__local_test_flat_auto_with_null_byte__flat_auto_1,__local_test_flat_auto_with_null_byte__flat_auto_0 indirect=false in_cycle=false
; def test_flat_auto_without_null_byte params= locals=__local_test_flat_auto_without_null_byte__letters_3,__local_test_flat_auto_without_null_byte__letters_2,__local_test_flat_auto_without_null_byte__letters_1,__local_test_flat_auto_without_null_byte__letters_0 indirect=false in_cycle=false
; def test_flat_static_with_null_byte params= locals=__local_test_flat_static_with_null_byte__0 indirect=false in_cycle=false
; def test_flat_static_without_null_byte params= locals=__local_test_flat_static_without_null_byte__0 indirect=false in_cycle=false
; def test_nested_auto_with_null_byte params= locals=__local_test_nested_auto_with_null_byte__nested_auto_7,__local_test_nested_auto_with_null_byte__nested_auto_6,__local_test_nested_auto_with_null_byte__nested_auto_5,__local_test_nested_auto_with_null_byte__nested_auto_4,__local_test_nested_auto_with_null_byte__nested_auto_3,__local_test_nested_auto_with_null_byte__nested_auto_2,__local_test_nested_auto_with_null_byte__nested_auto_1,__local_test_nested_auto_with_null_byte__nested_auto_0 indirect=false in_cycle=false
; def test_nested_auto_without_null_byte params= locals=__local_test_nested_auto_without_null_byte__0,__local_test_nested_auto_without_null_byte__1,__local_test_nested_auto_without_null_byte__2,__local_test_nested_auto_without_null_byte__3,__local_test_nested_auto_without_null_byte__4,__local_test_nested_auto_without_null_byte__5,__local_test_nested_auto_without_null_byte__6,__local_test_nested_auto_without_null_byte__7,__local_test_nested_auto_without_null_byte__8,__local_test_nested_auto_without_null_byte__9,__local_test_nested_auto_without_null_byte__10,__local_test_nested_auto_without_null_byte__nested,__local_test_nested_auto_without_null_byte__12,__local_test_nested_auto_without_null_byte__13,__local_test_nested_auto_without_null_byte__14,__local_test_nested_auto_without_null_byte__15,__local_test_nested_auto_without_null_byte__16,__local_test_nested_auto_without_null_byte__17,__local_test_nested_auto_without_null_byte__18,__local_test_nested_auto_without_null_byte__19 indirect=false in_cycle=false
; def test_nested_static_with_null_byte params= locals=__local_test_nested_static_with_null_byte__0 indirect=false in_cycle=false
; def test_nested_static_without_null_byte params= locals=__local_test_nested_static_without_null_byte__0,__local_test_nested_static_without_null_byte__1,__local_test_nested_static_without_null_byte__2,__local_test_nested_static_without_null_byte__3,__local_test_nested_static_without_null_byte__4,__local_test_nested_static_without_null_byte__5,__local_test_nested_static_without_null_byte__6,__local_test_nested_static_without_null_byte__7 indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call main -> test_flat_auto_with_null_byte
; call main -> test_flat_auto_without_null_byte
; call main -> test_flat_static_with_null_byte
; call main -> test_flat_static_without_null_byte
; call main -> test_nested_auto_with_null_byte
; call main -> test_nested_auto_without_null_byte
; call main -> test_nested_static_with_null_byte
; call main -> test_nested_static_without_null_byte
; call test_nested_auto_without_null_byte -> strcmp
; call test_nested_static_without_null_byte -> strcmp
; item test_flat_static_with_null_byte section=.text local=false zero=false
; item test_flat_static_with_null_byte@tail1 section=.text local=true zero=false
; item test_flat_static_with_null_byte@tail0 section=.text local=true zero=false
; item test_nested_static_with_null_byte section=.text local=false zero=false
; item test_flat_auto_with_null_byte section=.text local=false zero=false
; item test_nested_auto_with_null_byte section=.text local=false zero=false
; item test_flat_static_without_null_byte section=.text local=false zero=false
; item test_nested_static_without_null_byte section=.text local=false zero=false
; item test_flat_auto_without_null_byte section=.text local=false zero=false
; item test_nested_auto_without_null_byte section=.text local=false zero=false
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item str@3 section=.rodata local=true zero=false
; item str@4 section=.rodata local=true zero=false
; item str@5 section=.rodata local=true zero=false
; item str@6 section=.rodata local=true zero=false
; item str@7 section=.rodata local=true zero=false
; item @2.flat section=.data local=true zero=false
; item @3.nested section=.data local=true zero=false
; item @6.letters section=.data local=true zero=false
; item nested section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_flat_static_with_null_byte:
   SUBROUTINE

   LDA   @2.flat
   CMP   #$64
   BNE   .and_false@0
   LDA   @2.flat+1
   CMP   #$6F
   BNE   .and_false@0
   LDA   @2.flat+2
   CMP   #$67
   BNE   .and_false@0
   LDA   @2.flat+3
   BNE   .and_false@0
   JMP   test_flat_static_with_null_byte@tail0
.and_false@0:
   JMP   test_flat_static_with_null_byte@tail1

test_flat_static_with_null_byte@tail1:
   SUBROUTINE

   LDA   #$00
   LDX   #$00
   RTS

test_flat_static_with_null_byte@tail0:
   SUBROUTINE

   LDA   #$01
   LDX   #$00
   RTS

test_nested_static_with_null_byte:
   SUBROUTINE

   LDA   @3.nested
   CMP   #$79
   BNE   .and_false@6
   LDA   @3.nested+1
   CMP   #$65
   BNE   .and_false@6
   LDA   @3.nested+2
   CMP   #$73
   BNE   .and_false@6
   LDA   @3.nested+3
   BNE   .and_false@6
   LDA   @3.nested+4
   CMP   #$79
   BNE   .and_false@6
   LDA   @3.nested+5
   CMP   #$75
   BNE   .and_false@6
   LDA   @3.nested+6
   CMP   #$70
   BNE   .and_false@6
   LDA   @3.nested+7
   BNE   .and_false@6
   JMP   test_flat_static_with_null_byte@tail0
.and_false@6:
   JMP   test_flat_static_with_null_byte@tail1

test_flat_auto_with_null_byte:
   SUBROUTINE

   LDA   #$78
   LDY   #$00
   CMP   #$78
   BNE   .and_false@20
   TYA
   BNE   .and_false@20
   JMP   test_flat_static_with_null_byte@tail0
.and_false@20:
   TYA
   LDX   #$00
   RTS

test_nested_auto_with_null_byte:
   SUBROUTINE

   LDY   #$64
   LDA   #$61
   CMP   #$61
   BNE   .and_false@22
   LDA   #$00
   BNE   .and_false@22
   LDA   #$62
   CMP   #$62
   BNE   .and_false@22
   LDA   #$00
   BNE   .and_false@22
   LDA   #$63
   CMP   #$63
   BNE   .and_false@22
   LDA   #$00
   BNE   .and_false@22
   CPY   #$64
   BNE   .vthread@0
   ORA   #$00
   BNE   .and_false@22
   JMP   test_flat_static_with_null_byte@tail0
.and_false@22:
   LDA   #$00
.vthread@0:
   LDX   #$00
   RTS

test_flat_static_without_null_byte:
   SUBROUTINE

   LDA   @6.letters
   CMP   #$61
   BNE   .and_false@36
   LDA   @6.letters+1
   CMP   #$62
   BNE   .and_false@36
   LDA   @6.letters+2
   CMP   #$63
   BNE   .and_false@36
   LDA   @6.letters+3
   CMP   #$64
   BNE   .and_false@36
   JMP   test_flat_static_with_null_byte@tail0
.and_false@36:
   JMP   test_flat_static_with_null_byte@tail1

test_nested_static_without_null_byte:
   SUBROUTINE

   LDA   #<nested
   STA   __local_test_nested_static_without_null_byte__7
   LDA   #>nested
   STA   __local_test_nested_static_without_null_byte__6
   LDA   #<(nested+3)
   STA   __local_test_nested_static_without_null_byte__5
   LDA   #>(nested+3)
   STA   __local_test_nested_static_without_null_byte__4
   LDA   #<(nested+6)
   STA   __local_test_nested_static_without_null_byte__3
   LDA   #>(nested+6)
   STA   __local_test_nested_static_without_null_byte__2
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDX   #>nested
   LDA   #<nested
   JSR   strcmp
   STX   __local_test_nested_static_without_null_byte__0
   ORA   __local_test_nested_static_without_null_byte__0
   BNE   .or_true@42
   LDA   #<str@1
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@1
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_nested_static_without_null_byte__6
   LDA   __local_test_nested_static_without_null_byte__7
   JSR   strcmp
   STX   __local_test_nested_static_without_null_byte__0
   ORA   __local_test_nested_static_without_null_byte__0
   BNE   .or_true@42
   LDA   #<str@2
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@2
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_nested_static_without_null_byte__4
   LDA   __local_test_nested_static_without_null_byte__5
   JSR   strcmp
   STX   __local_test_nested_static_without_null_byte__0
   ORA   __local_test_nested_static_without_null_byte__0
   BNE   .or_true@42
   LDA   #<str@3
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@3
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_nested_static_without_null_byte__2
   LDA   __local_test_nested_static_without_null_byte__3
   JSR   strcmp
   STX   __local_test_nested_static_without_null_byte__0
   ORA   __local_test_nested_static_without_null_byte__0
   BNE   .or_true@42
   JMP   test_flat_static_with_null_byte@tail0
.or_true@42:
   JMP   test_flat_static_with_null_byte@tail1

test_flat_auto_without_null_byte:
   SUBROUTINE

   LDA   #$61
   CMP   #$61
   BNE   .and_false@48
   LDA   #$62
   CMP   #$62
   BNE   .and_false@48
   LDA   #$63
   CMP   #$63
   BNE   .and_false@48
   LDA   #$64
   CMP   #$64
   BNE   .and_false@48
   JMP   test_flat_static_with_null_byte@tail0
.and_false@48:
   JMP   test_flat_static_with_null_byte@tail1

test_nested_auto_without_null_byte:
   SUBROUTINE

   LDA   #$79
   STA   __local_test_nested_auto_without_null_byte__nested
   LDA   #$65
   STA   __local_test_nested_auto_without_null_byte__nested+1
   LDA   #$73
   STA   __local_test_nested_auto_without_null_byte__nested+2
   LDA   #$6E
   STA   __local_test_nested_auto_without_null_byte__nested+3
   LDA   #$6F
   STA   __local_test_nested_auto_without_null_byte__nested+4
   LDY   #$00
   STY   __local_test_nested_auto_without_null_byte__nested+5
   STA   __local_test_nested_auto_without_null_byte__nested+6
   LDA   #$6B
   STA   __local_test_nested_auto_without_null_byte__nested+7
   STY   __local_test_nested_auto_without_null_byte__nested+8
   LDA   #>__local_test_nested_auto_without_null_byte__nested
   STA   __local_test_nested_auto_without_null_byte__10
   LDA   #<__local_test_nested_auto_without_null_byte__nested
   CLC
   STA   __local_test_nested_auto_without_null_byte__8
   BCC   .carry_inc@0
   INC   __local_test_nested_auto_without_null_byte__10
.carry_inc@0:
   LDA   #>__local_test_nested_auto_without_null_byte__nested
   STA   __local_test_nested_auto_without_null_byte__7
   LDA   #<__local_test_nested_auto_without_null_byte__nested
   CLC
   ADC   #$03
   STA   __local_test_nested_auto_without_null_byte__5
   BCC   .carry_inc@1
   INC   __local_test_nested_auto_without_null_byte__7
.carry_inc@1:
   LDA   #>__local_test_nested_auto_without_null_byte__nested
   STA   __local_test_nested_auto_without_null_byte__4
   LDA   #<__local_test_nested_auto_without_null_byte__nested
   CLC
   ADC   #$06
   STA   __local_test_nested_auto_without_null_byte__2
   BCC   .carry_inc@2
   INC   __local_test_nested_auto_without_null_byte__4
.carry_inc@2:
   LDA   #<str@4
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@4
   STA   __zpabi_strcmp__s2_1
   LDX   #>__local_test_nested_auto_without_null_byte__nested
   LDA   #<__local_test_nested_auto_without_null_byte__nested
   JSR   strcmp
   STX   __local_test_nested_auto_without_null_byte__0
   ORA   __local_test_nested_auto_without_null_byte__0
   BNE   .or_true@58
   LDA   #<str@5
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@5
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_nested_auto_without_null_byte__10
   LDA   __local_test_nested_auto_without_null_byte__8
   JSR   strcmp
   STX   __local_test_nested_auto_without_null_byte__0
   ORA   __local_test_nested_auto_without_null_byte__0
   BNE   .or_true@58
   LDA   #<str@6
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@6
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_nested_auto_without_null_byte__7
   LDA   __local_test_nested_auto_without_null_byte__5
   JSR   strcmp
   STX   __local_test_nested_auto_without_null_byte__0
   ORA   __local_test_nested_auto_without_null_byte__0
   BNE   .or_true@58
   LDA   #<str@7
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@7
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_nested_auto_without_null_byte__4
   LDA   __local_test_nested_auto_without_null_byte__2
   JSR   strcmp
   STX   __local_test_nested_auto_without_null_byte__0
   ORA   __local_test_nested_auto_without_null_byte__0
   BNE   .or_true@58
   JMP   test_flat_static_with_null_byte@tail0
.or_true@58:
   JMP   test_flat_static_with_null_byte@tail1

main:
   SUBROUTINE

   JSR   test_flat_static_with_null_byte
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@64
   LDX   #$00
   LDA   #$01
   RTS
.if_end@64:
   JSR   test_nested_static_with_null_byte
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@65
   LDX   #$00
   LDA   #$02
   RTS
.if_end@65:
   JSR   test_flat_auto_with_null_byte
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@66
   LDX   #$00
   LDA   #$03
   RTS
.if_end@66:
   JSR   test_nested_auto_with_null_byte
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@67
   LDX   #$00
   LDA   #$04
   RTS
.if_end@67:
   JSR   test_flat_static_without_null_byte
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@68
   LDX   #$00
   LDA   #$05
   RTS
.if_end@68:
   JSR   test_nested_static_without_null_byte
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@69
   LDX   #$00
   LDA   #$06
   RTS
.if_end@69:
   JSR   test_flat_auto_without_null_byte
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@70
   LDX   #$00
   LDA   #$07
   RTS
.if_end@70:
   JSR   test_nested_auto_without_null_byte
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@71
   LDX   #$00
   LDA   #$08
   RTS
.if_end@71:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $79, $65, $73, $6E, $6F, $00

str@1:
   DC.B  $79, $65, $73, $6E, $6F, $00

str@2:
   DC.B  $6E, $6F, $00

str@3:
   DC.B  $6F, $6B, $00

str@4:
   DC.B  $79, $65, $73, $6E, $6F, $00

str@5:
   DC.B  $79, $65, $73, $6E, $6F, $00

str@6:
   DC.B  $6E, $6F, $00

str@7:
   DC.B  $6F, $6B, $00

@2.flat:
   DC.B  $64, $6F, $67, $00

@3.nested:
   DC.B  $79, $65, $73, $00
   DC.B  $79, $75, $70, $00

@6.letters:
   DC.B  $61, $62, $63, $64

nested:
   DC.B  $79, $65, $73
   DC.B  $6E, $6F, $00
   DC.B  $6F, $6B, $00
