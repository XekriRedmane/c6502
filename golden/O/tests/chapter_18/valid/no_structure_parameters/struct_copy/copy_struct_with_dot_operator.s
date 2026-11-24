SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_test_copy_from_conditional__small_struct_0	EQU	$82
__local_test_copy_from_conditional__small_struct_4	EQU	$83
__local_test_copy_from_conditional__small_struct_3	EQU	$84
__local_test_copy_from_conditional__small_struct_2	EQU	$85
__local_test_copy_from_conditional__small_struct_1	EQU	$86
__local_test_copy_from_conditional__0	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_copy_from_assignment params= locals=__local_test_copy_from_assignment__big_struct_6,__local_test_copy_from_assignment__big_struct_5,__local_test_copy_from_assignment__big_struct_4,__local_test_copy_from_assignment__big_struct_3,__local_test_copy_from_assignment__big_struct_2,__local_test_copy_from_assignment__big_struct_1,__local_test_copy_from_assignment__big_struct_0 indirect=false in_cycle=false
; def test_copy_from_conditional params= locals=__local_test_copy_from_conditional__small_struct_0,__local_test_copy_from_conditional__small_struct_4,__local_test_copy_from_conditional__small_struct_3,__local_test_copy_from_conditional__small_struct_2,__local_test_copy_from_conditional__small_struct_1,__local_test_copy_from_conditional__0,__local_test_copy_from_conditional__big_struct2_6,__local_test_copy_from_conditional__big_struct2_5,__local_test_copy_from_conditional__big_struct2_4,__local_test_copy_from_conditional__big_struct2_3,__local_test_copy_from_conditional__big_struct2_2,__local_test_copy_from_conditional__big_struct2_1,__local_test_copy_from_conditional__big_struct2_0,__local_test_copy_from_conditional__big_struct_6,__local_test_copy_from_conditional__big_struct_5,__local_test_copy_from_conditional__big_struct_4,__local_test_copy_from_conditional__big_struct_3,__local_test_copy_from_conditional__big_struct_2,__local_test_copy_from_conditional__big_struct_1,__local_test_copy_from_conditional__big_struct_0,__local_test_copy_from_conditional__addr_%293,__local_test_copy_from_conditional__2,__local_test_copy_from_conditional__3,__local_test_copy_from_conditional__4,__local_test_copy_from_conditional__5,__local_test_copy_from_conditional__6,__local_test_copy_from_conditional__7,__local_test_copy_from_conditional__addr_%327,__local_test_copy_from_conditional__9,__local_test_copy_from_conditional__10,__local_test_copy_from_conditional__11,__local_test_copy_from_conditional__12,__local_test_copy_from_conditional__13,__local_test_copy_from_conditional__14 indirect=false in_cycle=false
; def test_copy_from_member params= locals=__local_test_copy_from_member__substruct_4,__local_test_copy_from_member__substruct_3,__local_test_copy_from_member__substruct_2,__local_test_copy_from_member__substruct_1,__local_test_copy_from_member__substruct_0,__local_test_copy_from_member__0,__local_test_copy_from_member__1,__local_test_copy_from_member__arr2_2,__local_test_copy_from_member__arr2_1,__local_test_copy_from_member__arr2_0,__local_test_copy_from_member__arr_2,__local_test_copy_from_member__arr_1,__local_test_copy_from_member__arr_0 indirect=false in_cycle=false
; def test_copy_from_nested_member params= locals=__local_test_copy_from_nested_member__0,__local_test_copy_from_nested_member__1,__local_test_copy_from_nested_member__biggest_struct,__local_test_copy_from_nested_member__3,__local_test_copy_from_nested_member__4,__local_test_copy_from_nested_member__5,__local_test_copy_from_nested_member__6,__local_test_copy_from_nested_member__7,__local_test_copy_from_nested_member__8,__local_test_copy_from_nested_member__9,__local_test_copy_from_nested_member__10 indirect=false in_cycle=false
; def test_copy_to_member params= locals=__local_test_copy_to_member__0,__local_test_copy_to_member__1,__local_test_copy_to_member__small_struct_4,__local_test_copy_to_member__small_struct_3,__local_test_copy_to_member__small_struct_2,__local_test_copy_to_member__small_struct_1,__local_test_copy_to_member__small_struct_0 indirect=false in_cycle=false
; def test_copy_to_nested_member params= locals=__local_test_copy_to_nested_member__0,__local_test_copy_to_nested_member__1,__local_test_copy_to_nested_member__biggest_struct,__local_test_copy_to_nested_member__3,__local_test_copy_to_nested_member__4,__local_test_copy_to_nested_member__5,__local_test_copy_to_nested_member__6,__local_test_copy_to_nested_member__7,__local_test_copy_to_nested_member__8,__local_test_copy_to_nested_member__9,__local_test_copy_to_nested_member__10 indirect=false in_cycle=false
; call main -> test_copy_from_assignment
; call main -> test_copy_from_conditional
; call main -> test_copy_from_member
; call main -> test_copy_from_nested_member
; call main -> test_copy_to_member
; call main -> test_copy_to_nested_member
; item test_copy_from_member section=.text local=false zero=false
; item test_copy_from_member@tail1 section=.text local=true zero=false
; item test_copy_from_member@tail0 section=.text local=true zero=false
; item test_copy_to_member section=.text local=false zero=false
; item test_copy_from_nested_member section=.text local=false zero=false
; item test_copy_to_nested_member section=.text local=false zero=false
; item test_copy_from_conditional section=.text local=false zero=false
; item test_copy_from_assignment section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @0.big_struct section=.data local=true zero=false
; item @4.big_struct section=.bss local=true zero=true
; item @7.small_struct section=.bss local=true zero=true
; item @9.small_struct section=.data local=true zero=false
; item @12.t section=.data local=true zero=false
; item @13.f section=.bss local=true zero=true
; item @16.big_struct2 section=.bss local=true zero=true
; item @17.small_struct section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_copy_from_member:
   SUBROUTINE

   LDA   @0.big_struct
   CMP   #$0A
   BNE   .test_copy_from_member@jthread@8
   LDA   @0.big_struct+1
   CMP   #$09
   BNE   .test_copy_from_member@jthread@8
   LDA   @0.big_struct+2
   CMP   #$08
   BNE   .test_copy_from_member@jthread@8
   LDA   @0.big_struct+3
   CMP   #$07
   BNE   .test_copy_from_member@jthread@8
   LDA   @0.big_struct+4
   CMP   #$06
   BEQ   .if_end@19
   JMP   test_copy_from_member@tail1
.test_copy_from_member@jthread@8:
   LDX   #$00
   TXA
   RTS
.if_end@19:
   JMP   test_copy_from_member@tail0

test_copy_from_member@tail1:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_copy_from_member@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_copy_to_member:
   SUBROUTINE

   LDA   #$FE
   STA   @4.big_struct+1
   LDA   #$FD
   STA   @4.big_struct+2
   LDA   #$FC
   STA   @4.big_struct+3
   LDA   #$FB
   STA   @4.big_struct+4
   LDA   #$FF
   STA   @4.big_struct
   BPL   .test_copy_to_member@jthread@4
   CMP   #$FF
   BNE   .test_copy_to_member@jthread@4
   LDA   @4.big_struct+1
   BPL   .test_copy_to_member@jthread@4
   CMP   #$FE
   BNE   .test_copy_to_member@jthread@4
   LDA   @4.big_struct+2
   BPL   .test_copy_to_member@jthread@4
   CMP   #$FD
   BNE   .test_copy_to_member@jthread@4
   LDA   @4.big_struct+3
   BPL   .test_copy_to_member@jthread@4
   CMP   #$FC
   BNE   .test_copy_to_member@jthread@4
   LDA   @4.big_struct+4
   BPL   .test_copy_to_member@jthread@4
   CMP   #$FB
   BNE   .test_copy_to_member@jthread@4
   LDA   @4.big_struct+5
   BNE   .test_copy_to_member@jthread@4
   LDA   @4.big_struct+6
   BEQ   .if_end@31
.test_copy_to_member@jthread@4:
   JMP   test_copy_from_member@tail1
.if_end@31:
   JMP   test_copy_from_member@tail0

test_copy_from_nested_member:
   SUBROUTINE

   LDA   #$FE
   STA   @7.small_struct+1
   LDA   #$FD
   STA   @7.small_struct+2
   LDA   #$FC
   STA   @7.small_struct+3
   LDA   #$FB
   STA   @7.small_struct+4
   LDA   #$FF
   STA   @7.small_struct
   BPL   .test_copy_from_nested_member@jthread@3
   CMP   #$FF
   BNE   .test_copy_from_nested_member@jthread@3
   LDA   @7.small_struct+1
   BPL   .test_copy_from_nested_member@jthread@3
   CMP   #$FE
   BNE   .test_copy_from_nested_member@jthread@3
   LDA   @7.small_struct+2
   BPL   .test_copy_from_nested_member@jthread@3
   CMP   #$FD
   BNE   .test_copy_from_nested_member@jthread@3
   LDA   @7.small_struct+3
   BPL   .test_copy_from_nested_member@jthread@3
   CMP   #$FC
   BNE   .test_copy_from_nested_member@jthread@3
   LDA   @7.small_struct+4
   BPL   .test_copy_from_nested_member@jthread@3
   CMP   #$FB
   BEQ   .if_end@40
.test_copy_from_nested_member@jthread@3:
   JMP   test_copy_from_member@tail1
.if_end@40:
   JMP   test_copy_from_member@tail0

test_copy_to_nested_member:
   SUBROUTINE

   LDY   #$00
   LDA   @9.small_struct
   CMP   #$32
   BNE   .jcmp_skip@20
   LDA   @9.small_struct+1
   CMP   #$33
   BNE   .jcmp_skip@20
   LDA   @9.small_struct+2
   CMP   #$34
   BNE   .jcmp_skip@20
   LDA   @9.small_struct+3
   CMP   #$35
   BNE   .jcmp_skip@20
   LDA   @9.small_struct+4
   CMP   #$36
   BNE   .jcmp_skip@20
   TYA
   BNE   .jcmp_skip@20
   BEQ   .if_end@53
   LDX   #$00
   RTS
.jcmp_skip@20:
   JMP   test_copy_from_member@tail1
.if_end@53:
   JMP   test_copy_from_member@tail0

test_copy_from_conditional:
   SUBROUTINE

   LDA   @13.f
   ORA   @13.f+1
   BEQ   .cond_else@54
   LDA   #$7F
   STA   __local_test_copy_from_conditional__0
   LDA   #$80
   STA   __local_test_copy_from_conditional__small_struct_1
   LDA   #$3D
   STA   __local_test_copy_from_conditional__small_struct_2
   LDA   #$3E
   STA   __local_test_copy_from_conditional__small_struct_3
   LDA   #$3F
   STA   __local_test_copy_from_conditional__small_struct_4
   JMP   .cond_end@55
.cond_else@54:
   LDA   #$00
   STA   __local_test_copy_from_conditional__0
   LDA   #$01
   STA   __local_test_copy_from_conditional__small_struct_1
   LDA   #$02
   STA   __local_test_copy_from_conditional__small_struct_2
   LDA   #$03
   STA   __local_test_copy_from_conditional__small_struct_3
   LDA   #$04
   STA   __local_test_copy_from_conditional__small_struct_4
.cond_end@55:
   LDA   __local_test_copy_from_conditional__0
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   ORA   __local_test_copy_from_conditional__0
   BNE   .test_copy_from_conditional@jthread@3
   LDA   __local_test_copy_from_conditional__small_struct_1
   CMP   #$01
   BNE   .test_copy_from_conditional@jthread@3
   LDA   __local_test_copy_from_conditional__small_struct_2
   CMP   #$02
   BNE   .test_copy_from_conditional@jthread@7
   LDA   __local_test_copy_from_conditional__small_struct_3
   CMP   #$03
   BNE   .test_copy_from_conditional@jthread@7
   LDA   __local_test_copy_from_conditional__small_struct_4
   CMP   #$04
   BEQ   .if_end@64
.test_copy_from_conditional@jthread@3:
   JMP   test_copy_from_member@tail1
.if_end@64:
   LDA   @12.t
   ORA   @12.t+1
   BEQ   .cond_else@65
   LDA   #$7F
   STA   __local_test_copy_from_conditional__small_struct_0
   LDA   #$80
   STA   __local_test_copy_from_conditional__small_struct_1
   LDA   #$3D
   STA   __local_test_copy_from_conditional__small_struct_2
   LDA   #$3E
   STA   __local_test_copy_from_conditional__small_struct_3
   LDA   #$3F
   STA   __local_test_copy_from_conditional__small_struct_4
   JMP   .cond_end@66
.cond_else@65:
   LDA   #$00
   STA   __local_test_copy_from_conditional__small_struct_0
   LDA   #$01
   STA   __local_test_copy_from_conditional__small_struct_1
   LDA   #$02
   STA   __local_test_copy_from_conditional__small_struct_2
   LDA   #$03
   STA   __local_test_copy_from_conditional__small_struct_3
   LDA   #$04
   STA   __local_test_copy_from_conditional__small_struct_4
.cond_end@66:
   LDA   __local_test_copy_from_conditional__small_struct_0
   CMP   #$7F
   BNE   .test_copy_from_conditional@jthread@7
   LDA   __local_test_copy_from_conditional__small_struct_1
   BPL   .test_copy_from_conditional@jthread@7
   CMP   #$80
   BNE   .test_copy_from_conditional@jthread@7
   LDA   __local_test_copy_from_conditional__small_struct_2
   CMP   #$3D
   BNE   .test_copy_from_conditional@jthread@7
   LDA   __local_test_copy_from_conditional__small_struct_3
   CMP   #$3E
   BNE   .test_copy_from_conditional@jthread@7
   LDA   __local_test_copy_from_conditional__small_struct_4
   CMP   #$3F
   BEQ   .if_end@75
.test_copy_from_conditional@jthread@7:
   LDX   #$00
   TXA
   RTS
.if_end@75:
   JMP   test_copy_from_member@tail0

test_copy_from_assignment:
   SUBROUTINE

   LDA   #$7F
   STA   @16.big_struct2
   STA   @17.small_struct
   LDA   #$80
   STA   @16.big_struct2+1
   STA   @17.small_struct+1
   LDA   #$3D
   STA   @16.big_struct2+2
   STA   @17.small_struct+2
   LDA   #$3E
   STA   @16.big_struct2+3
   STA   @17.small_struct+3
   LDA   #$F6
   STA   @16.big_struct2+5
   LDA   #$F5
   STA   @16.big_struct2+6
   LDA   #$3F
   STA   @16.big_struct2+4
   STA   @17.small_struct+4
   LDA   #$7F
   CMP   #$7F
   BNE   .test_copy_from_assignment@jthread@9
   LDA   @17.small_struct+1
   BPL   .test_copy_from_assignment@jthread@9
   CMP   #$80
   BNE   .test_copy_from_assignment@jthread@9
   LDA   @17.small_struct+2
   CMP   #$3D
   BNE   .test_copy_from_assignment@jthread@9
   LDA   @17.small_struct+3
   CMP   #$3E
   BNE   .test_copy_from_assignment@jthread@9
   LDA   @17.small_struct+4
   CMP   #$3F
   BNE   .test_copy_from_assignment@jthread@9
   LDA   @16.big_struct2
   CMP   #$7F
   BNE   .test_copy_from_assignment@jthread@9
   LDA   @16.big_struct2+1
   BPL   .test_copy_from_assignment@jthread@9
   CMP   #$80
   BNE   .test_copy_from_assignment@jthread@9
   LDA   @16.big_struct2+2
   CMP   #$3D
   BNE   .test_copy_from_assignment@jthread@9
   LDA   @16.big_struct2+3
   CMP   #$3E
   BNE   .test_copy_from_assignment@jthread@9
   LDA   @16.big_struct2+4
   CMP   #$3F
   BNE   .test_copy_from_assignment@jthread@9
   LDA   @16.big_struct2+5
   BPL   .test_copy_from_assignment@jthread@9
   CMP   #$F6
   BNE   .test_copy_from_assignment@jthread@9
   LDA   @16.big_struct2+6
   BPL   .test_copy_from_assignment@jthread@9
   CMP   #$F5
   BEQ   .if_end@97
.test_copy_from_assignment@jthread@9:
   JMP   test_copy_from_member@tail1
.if_end@97:
   JMP   test_copy_from_member@tail0

main:
   SUBROUTINE

   JSR   test_copy_from_member
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@98
   JMP   test_copy_from_member@tail0
.if_end@98:
   JSR   test_copy_to_member
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@99
   LDX   #$00
   LDA   #$02
   RTS
.if_end@99:
   JSR   test_copy_from_nested_member
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@100
   LDX   #$00
   LDA   #$03
   RTS
.if_end@100:
   JSR   test_copy_to_nested_member
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@101
   LDX   #$00
   LDA   #$04
   RTS
.if_end@101:
   JSR   test_copy_from_conditional
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@102
   LDX   #$00
   LDA   #$06
   RTS
.if_end@102:
   JSR   test_copy_from_assignment
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@103
   LDX   #$00
   LDA   #$07
   RTS
.if_end@103:
   JMP   test_copy_from_member@tail1

@0.big_struct:
   DC.B  $0A
   DC.B  $09
   DC.B  $08, $07, $06
   DC.B  $05
   DC.B  $04

@4.big_struct:
   DS.B  7

@7.small_struct:
   DS.B  5

@9.small_struct:
   DC.B  $32
   DC.B  $33
   DC.B  $34, $35, $36

@12.t:
   DC.W  $0001

@13.f:
   DS.B  2

@16.big_struct2:
   DS.B  7

@17.small_struct:
   DS.B  5
