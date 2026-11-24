SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__local_test_array_of_structs__0	EQU	$82
__local_test_partially_initialized__0	EQU	$82

; @zp-link-meta-begin
; def test_array_of_structs params= locals=__local_test_array_of_structs__0,__local_test_array_of_structs__1,__local_test_array_of_structs__2,__local_test_array_of_structs__3,__local_test_array_of_structs__4,__local_test_array_of_structs__5,__local_test_array_of_structs__6,__local_test_array_of_structs__7 indirect=false in_cycle=false
; def test_fully_intialized params= locals=__local_test_fully_intialized__0,__local_test_fully_intialized__1,__local_test_fully_intialized__2,__local_test_fully_intialized__3,__local_test_fully_intialized__4,__local_test_fully_intialized__5,__local_test_fully_intialized__6,__local_test_fully_intialized__7 indirect=false in_cycle=false
; def test_implicit_conversions params= locals=__local_test_implicit_conversions__0,__local_test_implicit_conversions__1,__local_test_implicit_conversions__2,__local_test_implicit_conversions__3,__local_test_implicit_conversions__4,__local_test_implicit_conversions__5,__local_test_implicit_conversions__6,__local_test_implicit_conversions__7 indirect=false in_cycle=false
; def test_partially_initialized params= locals=__local_test_partially_initialized__0,__local_test_partially_initialized__1,__local_test_partially_initialized__2,__local_test_partially_initialized__3,__local_test_partially_initialized__4,__local_test_partially_initialized__5,__local_test_partially_initialized__6,__local_test_partially_initialized__7 indirect=false in_cycle=false
; def test_uninitialized params= locals=__local_test_uninitialized__0,__local_test_uninitialized__1,__local_test_uninitialized__2,__local_test_uninitialized__3,__local_test_uninitialized__4,__local_test_uninitialized__5,__local_test_uninitialized__6,__local_test_uninitialized__7 indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call test_array_of_structs -> strcmp
; call test_fully_intialized -> strcmp
; call test_partially_initialized -> strcmp
; item test_uninitialized section=.text local=false zero=false
; item test_uninitialized@tail1 section=.text local=true zero=false
; item test_uninitialized@tail0 section=.text local=true zero=false
; item test_partially_initialized section=.text local=false zero=false
; item test_fully_intialized section=.text local=false zero=false
; item test_implicit_conversions section=.text local=false zero=false
; item test_array_of_structs section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item str@3 section=.rodata local=true zero=false
; item str@4 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_uninitialized:
   SUBROUTINE

   LDA   all_zeros
   ORA   all_zeros+1
   ORA   all_zeros+2
   ORA   all_zeros+3
   BNE   .test_uninitialized@jthread@5
   LDA   all_zeros+11
   ORA   all_zeros+12
   BNE   .test_uninitialized@jthread@5
   LDA   all_zeros+13
   ORA   all_zeros+14
   ORA   all_zeros+15
   ORA   all_zeros+16
   ORA   all_zeros+17
   ORA   all_zeros+18
   ORA   all_zeros+19
   ORA   all_zeros+20
   BNE   .test_uninitialized@jthread@5
   LDA   all_zeros+4
   ORA   all_zeros+5
   BNE   .test_uninitialized@jthread@5
   LDA   all_zeros+6
   BNE   .test_uninitialized@jthread@5
   LDA   all_zeros+7
   BNE   .test_uninitialized@jthread@5
   LDA   all_zeros+8
   BNE   .test_uninitialized@jthread@5
   LDA   all_zeros+9
   ORA   all_zeros+10
   BEQ   .if_end@13
.test_uninitialized@jthread@5:
   JMP   test_uninitialized@tail0
.if_end@13:
   JMP   test_uninitialized@tail1

test_uninitialized@tail1:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_uninitialized@tail0:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_partially_initialized:
   SUBROUTINE

   LDA   partial+3
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+2
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+1
   BNE   .test_partially_initialized@jthread@3
   LDA   partial
   CMP   #$64
   BNE   .test_partially_initialized@jthread@3
   LDX   partial+12
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDA   partial+11
   JSR   strcmp
   STX   __local_test_partially_initialized__0
   ORA   __local_test_partially_initialized__0
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+13
   ORA   partial+14
   ORA   partial+15
   ORA   partial+16
   ORA   partial+17
   ORA   partial+18
   ORA   partial+19
   ORA   partial+20
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+5
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+4
   CMP   #$0A
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+6
   CMP   #$0A
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+7
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+8
   BNE   .test_partially_initialized@jthread@3
   LDA   partial+9
   ORA   partial+10
   BEQ   .if_end@25
.test_partially_initialized@jthread@3:
   JMP   test_uninitialized@tail0
.if_end@25:
   JMP   test_uninitialized@tail1

test_fully_intialized:
   SUBROUTINE

   LDA   full+3
   BMI   .test_fully_intialized@jthread@1
   LDA   #$00
   BEQ   .test_fully_intialized@jthread@5
.test_fully_intialized@jthread@1:
   JMP   test_uninitialized@tail0
.test_fully_intialized@jthread@5:
   LDX   #$00
   RTS

test_implicit_conversions:
   SUBROUTINE

   LDA   converted+3
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted+2
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted+1
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted
   CMP   #$0A
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted+11
   ORA   converted+12
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted+20
   CMP   #$43
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted+19
   CMP   #$E0
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted+18
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted+17
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted+16
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted+15
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted+14
   BNE   .test_implicit_conversions@jthread@5
   LDA   converted+13
   CMP   #$01
.test_implicit_conversions@jthread@5:
   JMP   test_uninitialized@tail0

test_array_of_structs:
   SUBROUTINE

   LDA   struct_array+3
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array+2
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array+1
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array
   CMP   #$01
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array+11
   ORA   struct_array+12
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array+20
   CMP   #$40
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array+19
   CMP   #$14
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array+18
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array+17
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array+16
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array+15
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array+14
   BNE   .test_array_of_structs@jthread@1
   LDA   struct_array+13
   BEQ   .if_end@58
.test_array_of_structs@jthread@1:
   JMP   test_uninitialized@tail0
.if_end@58:
   LDA   struct_array+5
   BNE   .test_array_of_structs@jthread@3
   LDA   struct_array+4
   CMP   #$02
   BNE   .test_array_of_structs@jthread@3
   LDX   #>(struct_array+6)
   LDA   #<str@2
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@2
   STA   __zpabi_strcmp__s2_1
   LDA   #<(struct_array+6)
   JSR   strcmp
   STX   __local_test_array_of_structs__0
   ORA   __local_test_array_of_structs__0
   BNE   .test_array_of_structs@jthread@3
   LDA   struct_array+10
   BNE   .test_array_of_structs@jthread@3
   LDA   struct_array+9
   CMP   #$03
   BEQ   .if_end@63
.test_array_of_structs@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@63:
   LDA   struct_array+24
   BNE   .test_array_of_structs@jthread@5
   LDA   struct_array+23
   BNE   .test_array_of_structs@jthread@5
   LDA   struct_array+22
   BNE   .test_array_of_structs@jthread@5
   LDA   struct_array+21
   CMP   #$06
   BNE   .test_array_of_structs@jthread@5
   LDX   struct_array+33
   LDA   #<str@3
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@3
   STA   __zpabi_strcmp__s2_1
   LDA   struct_array+32
   JSR   strcmp
   STX   __local_test_array_of_structs__0
   ORA   __local_test_array_of_structs__0
   BNE   .test_array_of_structs@jthread@5
   LDA   struct_array+41
   CMP   #$40
   BNE   .test_array_of_structs@jthread@5
   LDA   struct_array+40
   CMP   #$22
   BNE   .test_array_of_structs@jthread@5
   LDA   struct_array+39
   BNE   .test_array_of_structs@jthread@5
   LDA   struct_array+38
   BNE   .test_array_of_structs@jthread@5
   LDA   struct_array+37
   BNE   .test_array_of_structs@jthread@5
   LDA   struct_array+36
   BNE   .test_array_of_structs@jthread@5
   LDA   struct_array+35
   BNE   .test_array_of_structs@jthread@5
   LDA   struct_array+34
   BEQ   .if_end@68
.test_array_of_structs@jthread@5:
   LDX   #$00
   TXA
   RTS
.if_end@68:
   LDA   struct_array+26
   BNE   .test_array_of_structs@jthread@7
   LDA   struct_array+25
   CMP   #$07
   BNE   .test_array_of_structs@jthread@13
   LDX   #>(struct_array+27)
   LDA   #<str@4
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@4
   STA   __zpabi_strcmp__s2_1
   LDA   #<(struct_array+27)
   JSR   strcmp
   STX   __local_test_array_of_structs__0
   ORA   __local_test_array_of_structs__0
   BNE   .test_array_of_structs@jthread@13
   LDA   struct_array+31
   BNE   .test_array_of_structs@jthread@13
   LDA   struct_array+30
   CMP   #$08
   BEQ   .if_end@73
.test_array_of_structs@jthread@7:
   LDX   #$00
   TXA
   RTS
.if_end@73:
   LDA   struct_array+42
   ORA   struct_array+43
   ORA   struct_array+44
   ORA   struct_array+45
   BNE   .test_array_of_structs@jthread@13
   LDA   struct_array+53
   ORA   struct_array+54
   BNE   .test_array_of_structs@jthread@13
   LDA   struct_array+55
   ORA   struct_array+56
   ORA   struct_array+57
   ORA   struct_array+58
   ORA   struct_array+59
   ORA   struct_array+60
   ORA   struct_array+61
   ORA   struct_array+62
   BNE   .test_array_of_structs@jthread@13
   LDA   struct_array+46
   ORA   struct_array+47
   BNE   .test_array_of_structs@jthread@13
   LDA   struct_array+48
   BNE   .test_array_of_structs@jthread@13
   LDA   struct_array+49
   BNE   .test_array_of_structs@jthread@13
   LDA   struct_array+50
   BNE   .test_array_of_structs@jthread@13
   LDA   struct_array+51
   ORA   struct_array+52
   BEQ   .if_end@87
.test_array_of_structs@jthread@13:
   LDX   #$00
   TXA
   RTS
.if_end@87:
   JMP   test_uninitialized@tail1

str@0:
   DC.B  $48, $65, $6C, $6C, $6F, $21, $00

str@1:
   DC.B  $41, $6E, $6F, $74, $68, $65, $72, $20, $6D, $65, $73, $73, $61, $67, $65, $00

str@2:
   DC.B  $61, $62, $00

str@3:
   DC.B  $4D, $65, $73, $73, $61, $67, $65, $00

str@4:
   DC.B  $63, $64, $00
