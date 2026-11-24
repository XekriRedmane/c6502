SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__b_0	EQU	$80
__zpabi_target__b_1	EQU	$81
__zpabi_target__c_0	EQU	$82
__zpabi_target__c_1	EQU	$83
__zpabi_target__d_0	EQU	$84
__zpabi_target__d_1	EQU	$85
__zpabi_target__e_0	EQU	$86
__zpabi_target__e_1	EQU	$87
__local_target__a_1	EQU	$89
__local_target__a_0	EQU	$8A
__local_target__0	EQU	$8B
__local_target__1	EQU	$8C
__local_target__2	EQU	$8D
__local_target__3	EQU	$8E
__local_target__4	EQU	$8F
__local_target__5	EQU	$90
__local_target__6	EQU	$91
__local_target__7	EQU	$92
__local_target__8	EQU	$93
__local_target__9	EQU	$94
__local_target__10	EQU	$95
__local_target__11	EQU	$96
__local_target__12	EQU	$97
__local_target__13	EQU	$98
__local_target__14	EQU	$99
__local_target__15	EQU	$9A
__local_target__16	EQU	$9B
__local_target__17	EQU	$9C
__local_target__18	EQU	$9D
__local_target__19	EQU	$9E
__local_target__20	EQU	$9F
__local_target__21	EQU	$A0
__local_target__22	EQU	$A1
__local_target__23	EQU	$A2
__local_target__24	EQU	$A3
__local_target__25	EQU	$A4
__local_target__26	EQU	$A5
__local_target__27	EQU	$A6
__local_target__28	EQU	$A7
__local_target__29	EQU	$A8
__local_target__30	EQU	$A9
__local_target__31	EQU	$AA
__local_target__32	EQU	$AB
__local_target__33	EQU	$AC
__local_target__34	EQU	$AD
__local_target__35	EQU	$AE
__local_target__36	EQU	$AF
__local_target__37	EQU	$B0

; @zp-link-meta-begin
; def callee params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params=__zpabi_target__a_0,__zpabi_target__a_1,__zpabi_target__b_0,__zpabi_target__b_1,__zpabi_target__c_0,__zpabi_target__c_1,__zpabi_target__d_0,__zpabi_target__d_1,__zpabi_target__e_0,__zpabi_target__e_1 locals=__local_target__a_1,__local_target__a_0,__local_target__0,__local_target__1,__local_target__2,__local_target__3,__local_target__4,__local_target__5,__local_target__6,__local_target__7,__local_target__8,__local_target__9,__local_target__10,__local_target__11,__local_target__12,__local_target__13,__local_target__14,__local_target__15,__local_target__16,__local_target__17,__local_target__18,__local_target__19,__local_target__20,__local_target__21,__local_target__22,__local_target__23,__local_target__24,__local_target__25,__local_target__26,__local_target__27,__local_target__28,__local_target__29,__local_target__30,__local_target__31,__local_target__32,__local_target__33,__local_target__34,__local_target__35,__local_target__36,__local_target__37,__local_target__38,__local_target__39 indirect=false in_cycle=false param_regs=A,X,,,,,,,,
; call main -> target
; call target -> callee
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item target@run2 section=.text local=true zero=false
; item target@run1 section=.text local=true zero=false
; item target@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

callee:
   SUBROUTINE

   ; prologue: 88 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@0:
   LDY   #$12
   LDA   (FP),Y
   CMP   #$3F
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   CMP   #$F0
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@0
   DEY
   LDA   (FP),Y
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@1:
   LDY   #$1A
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   DEY
   LDA   (FP),Y
   BEQ   .if_end@2
.jcmp_skip@1:
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@2:
   LDY   #$22
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   CMP   #$08
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@2
   DEY
   LDA   (FP),Y
   BEQ   .if_end@3
.jcmp_skip@2:
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@3:
   LDY   #$2A
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@3
   DEY
   LDA   (FP),Y
   BEQ   .if_end@4
.jcmp_skip@3:
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@4:
   LDY   #$32
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@4
   DEY
   LDA   (FP),Y
   BEQ   .if_end@5
.jcmp_skip@4:
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@5:
   LDY   #$3A
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   CMP   #$18
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@5
   DEY
   LDA   (FP),Y
   BEQ   .if_end@6
.jcmp_skip@5:
   LDX   #$00
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@6:
   LDY   #$42
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   CMP   #$1C
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@6
   DEY
   LDA   (FP),Y
   BEQ   .if_end@7
.jcmp_skip@6:
   LDX   #$00
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@7:
   LDY   #$4A
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   CMP   #$20
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@7
   DEY
   LDA   (FP),Y
   BEQ   .if_end@8
.jcmp_skip@7:
   LDX   #$00
   LDA   #$09

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@8:
   LDY   #$52
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@8
   DEY
   LDA   (FP),Y
   CMP   #$22
   BNE   .jcmp_skip@8
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@8
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@8
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@8
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@8
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@8
   DEY
   LDA   (FP),Y
   BEQ   .if_end@9
.jcmp_skip@8:
   LDX   #$00
   LDA   #$0A

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@9:
   LDY   #$5A
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@9
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .jcmp_skip@9
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@9
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@9
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@9
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@9
   DEY
   LDA   (FP),Y
   BNE   .jcmp_skip@9
   DEY
   LDA   (FP),Y
   BEQ   .if_end@10
.jcmp_skip@9:
   LDX   #$00
   LDA   #$0B

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@10:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS

target:
   SUBROUTINE

   STA   __local_target__a_0
   STX   __local_target__a_1
   LDA   __zpabi_target__e_0
   STA   HARGS
   LDA   __zpabi_target__e_1
   JSR   target@run2
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   STA   __local_target__37
   LDA   HARGS+17
   STA   __local_target__36
   LDA   HARGS+18
   STA   __local_target__35
   LDA   HARGS+19
   STA   __local_target__34
   LDA   HARGS+20
   STA   __local_target__33
   LDA   HARGS+21
   STA   __local_target__32
   LDA   HARGS+22
   STA   __local_target__31
   LDA   HARGS+23
   STA   __local_target__30
   LDA   __zpabi_target__d_0
   STA   HARGS
   LDA   __zpabi_target__d_1
   JSR   target@run2
   LDA   #$08
   JSR   target@run1
   STA   __local_target__29
   LDA   HARGS+17
   STA   __local_target__28
   LDA   HARGS+18
   STA   __local_target__27
   LDA   HARGS+19
   STA   __local_target__26
   LDA   HARGS+20
   STA   __local_target__25
   LDA   HARGS+21
   STA   __local_target__24
   LDA   HARGS+22
   STA   __local_target__23
   LDA   HARGS+23
   STA   __local_target__22
   LDA   __zpabi_target__c_0
   STA   HARGS
   LDA   __zpabi_target__c_1
   JSR   target@run2
   LDA   #$14
   JSR   target@run1
   STA   __local_target__21
   LDA   HARGS+17
   STA   __local_target__20
   LDA   HARGS+18
   STA   __local_target__19
   LDA   HARGS+19
   STA   __local_target__18
   LDA   HARGS+20
   STA   __local_target__17
   LDA   HARGS+21
   STA   __local_target__16
   LDA   HARGS+22
   STA   __local_target__15
   LDA   HARGS+23
   STA   __local_target__14
   LDA   __zpabi_target__b_0
   STA   HARGS
   LDA   __zpabi_target__b_1
   JSR   target@run2
   LDA   #$1C
   JSR   target@run1
   STA   __local_target__13
   LDA   HARGS+17
   STA   __local_target__12
   LDA   HARGS+18
   STA   __local_target__11
   LDA   HARGS+19
   STA   __local_target__10
   LDA   HARGS+20
   STA   __local_target__9
   LDA   HARGS+21
   STA   __local_target__8
   LDA   HARGS+22
   STA   __local_target__7
   LDA   HARGS+23
   STA   __local_target__6
   LDA   __local_target__a_0
   STA   HARGS
   LDA   __local_target__a_1
   JSR   target@run2
   LDA   #$22
   JSR   target@run1
   STA   __local_target__5
   LDA   HARGS+17
   STA   __local_target__4
   LDA   HARGS+18
   STA   __local_target__3
   LDA   HARGS+19
   STA   __local_target__2
   LDA   HARGS+20
   STA   __local_target__1
   LDA   HARGS+21
   STA   __local_target__0
   LDA   HARGS+22
   STA   __local_target__a_0
   LDA   HARGS+23
   STA   __local_target__a_1
   SEC
   LDA   SSP
   SBC   #$58
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$F0
   INY
   STA   (SSP),Y
   LDA   #$3F
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$08
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$10
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$14
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   __local_target__37
   INY
   STA   (SSP),Y
   LDA   __local_target__36
   INY
   STA   (SSP),Y
   LDA   __local_target__35
   INY
   STA   (SSP),Y
   LDA   __local_target__34
   INY
   STA   (SSP),Y
   LDA   __local_target__33
   INY
   STA   (SSP),Y
   LDA   __local_target__32
   INY
   STA   (SSP),Y
   LDA   __local_target__31
   INY
   STA   (SSP),Y
   LDA   __local_target__30
   INY
   STA   (SSP),Y
   LDA   __local_target__29
   INY
   STA   (SSP),Y
   LDA   __local_target__28
   INY
   STA   (SSP),Y
   LDA   __local_target__27
   INY
   STA   (SSP),Y
   LDA   __local_target__26
   INY
   STA   (SSP),Y
   LDA   __local_target__25
   INY
   STA   (SSP),Y
   LDA   __local_target__24
   INY
   STA   (SSP),Y
   LDA   __local_target__23
   INY
   STA   (SSP),Y
   LDA   __local_target__22
   INY
   STA   (SSP),Y
   LDA   __local_target__21
   INY
   STA   (SSP),Y
   LDA   __local_target__20
   INY
   STA   (SSP),Y
   LDA   __local_target__19
   INY
   STA   (SSP),Y
   LDA   __local_target__18
   INY
   STA   (SSP),Y
   LDA   __local_target__17
   INY
   STA   (SSP),Y
   LDA   __local_target__16
   INY
   STA   (SSP),Y
   LDA   __local_target__15
   INY
   STA   (SSP),Y
   LDA   __local_target__14
   INY
   STA   (SSP),Y
   LDA   __local_target__13
   INY
   STA   (SSP),Y
   LDA   __local_target__12
   INY
   STA   (SSP),Y
   LDA   __local_target__11
   INY
   STA   (SSP),Y
   LDA   __local_target__10
   INY
   STA   (SSP),Y
   LDA   __local_target__9
   INY
   STA   (SSP),Y
   LDA   __local_target__8
   INY
   STA   (SSP),Y
   LDA   __local_target__7
   INY
   STA   (SSP),Y
   LDA   __local_target__6
   INY
   STA   (SSP),Y
   LDA   __local_target__5
   INY
   STA   (SSP),Y
   LDA   __local_target__4
   INY
   STA   (SSP),Y
   LDA   __local_target__3
   INY
   STA   (SSP),Y
   LDA   __local_target__2
   INY
   STA   (SSP),Y
   LDA   __local_target__1
   INY
   STA   (SSP),Y
   LDA   __local_target__0
   INY
   STA   (SSP),Y
   LDA   __local_target__a_0
   INY
   STA   (SSP),Y
   LDA   __local_target__a_1
   INY
   STA   (SSP),Y
   JMP   callee

target@run2:
   SUBROUTINE

   JSR   target@run0
   STY   HARGS+12
   STA   HARGS+13
   RTS

target@run1:
   SUBROUTINE

   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   RTS

target@run0:
   SUBROUTINE

   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   STA   HARGS
   LDA   HARGS+3
   STA   HARGS+1
   LDA   HARGS+4
   STA   HARGS+2
   LDA   HARGS+5
   STA   HARGS+3
   LDA   HARGS+6
   STA   HARGS+4
   LDA   HARGS+7
   STA   HARGS+5
   LDA   HARGS+8
   STA   HARGS+6
   LDA   HARGS+9
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$02
   STA   __zpabi_target__b_0
   LDA   #$03
   STA   __zpabi_target__c_0
   LDA   #$04
   STA   __zpabi_target__d_0
   LDA   #$05
   STA   __zpabi_target__e_0
   LDA   #$00
   STA   __zpabi_target__b_1
   STA   __zpabi_target__c_1
   STA   __zpabi_target__d_1
   STA   __zpabi_target__e_1
   LDX   #$00
   LDA   #$01
   JMP   target
