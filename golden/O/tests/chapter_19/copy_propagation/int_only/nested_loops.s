SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
inner_flag	EQU	$26
outer_flag	EQU	$28
__local_inner_loop4__a_0	EQU	$80
__zpabi_inner_loop1__f_0	EQU	$80
__local_inner_loop2__a_0	EQU	$81
__local_inner_loop3__a_0	EQU	$81
__local_inner_loop4__a_1	EQU	$81
__local_validate__a_0	EQU	$81
__zpabi_inner_loop1__f_1	EQU	$81
__zpabi_inner_loop1__b_0	EQU	$82
__zpabi_inner_loop1__b_1	EQU	$83
__zpabi_inner_loop1__c_0	EQU	$84
__zpabi_inner_loop1__c_1	EQU	$85
__zpabi_inner_loop1__d_0	EQU	$86
__zpabi_inner_loop1__d_1	EQU	$87
__zpabi_inner_loop1__e_0	EQU	$88
__zpabi_inner_loop1__e_1	EQU	$89
__zpabi_inner_loop2__b_0	EQU	$8A
__zpabi_inner_loop3__b_0	EQU	$8A
__zpabi_inner_loop4__b_0	EQU	$8A
__zpabi_validate__b_0	EQU	$8A
__local_inner_loop1__a_0	EQU	$8B
__zpabi_inner_loop2__b_1	EQU	$8B
__zpabi_inner_loop3__b_1	EQU	$8B
__zpabi_inner_loop4__b_1	EQU	$8B
__zpabi_validate__b_1	EQU	$8B
__zpabi_inner_loop2__c_0	EQU	$8C
__zpabi_inner_loop3__c_0	EQU	$8C
__zpabi_inner_loop4__c_0	EQU	$8C
__zpabi_validate__c_0	EQU	$8C
__zpabi_inner_loop2__c_1	EQU	$8D
__zpabi_inner_loop3__c_1	EQU	$8D
__zpabi_inner_loop4__c_1	EQU	$8D
__zpabi_validate__c_1	EQU	$8D
__zpabi_inner_loop2__d_0	EQU	$8E
__zpabi_inner_loop3__d_0	EQU	$8E
__zpabi_inner_loop4__d_0	EQU	$8E
__zpabi_validate__d_0	EQU	$8E
__zpabi_inner_loop2__d_1	EQU	$8F
__zpabi_inner_loop3__d_1	EQU	$8F
__zpabi_inner_loop4__d_1	EQU	$8F
__zpabi_validate__d_1	EQU	$8F
__zpabi_inner_loop2__e_0	EQU	$90
__zpabi_inner_loop3__e_0	EQU	$90
__zpabi_inner_loop4__e_0	EQU	$90
__zpabi_validate__e_0	EQU	$90
__zpabi_inner_loop2__e_1	EQU	$91
__zpabi_inner_loop3__e_1	EQU	$91
__zpabi_inner_loop4__e_1	EQU	$91
__zpabi_validate__e_1	EQU	$91
__zpabi_inner_loop2__f_0	EQU	$92
__zpabi_inner_loop3__f_0	EQU	$92
__zpabi_inner_loop4__f_0	EQU	$92
__zpabi_validate__f_0	EQU	$92
__zpabi_inner_loop2__f_1	EQU	$93
__zpabi_inner_loop3__f_1	EQU	$93
__zpabi_inner_loop4__f_1	EQU	$93
__zpabi_validate__f_1	EQU	$93
__local_target__0	EQU	$95
__local_target__a_0	EQU	$98

; @zp-link-meta-begin
; def inner_loop1 params=__zpabi_inner_loop1__a_0,__zpabi_inner_loop1__a_1,__zpabi_inner_loop1__b_0,__zpabi_inner_loop1__b_1,__zpabi_inner_loop1__c_0,__zpabi_inner_loop1__c_1,__zpabi_inner_loop1__d_0,__zpabi_inner_loop1__d_1,__zpabi_inner_loop1__e_0,__zpabi_inner_loop1__e_1,__zpabi_inner_loop1__f_0,__zpabi_inner_loop1__f_1 locals=__local_inner_loop1__a_1,__local_inner_loop1__a_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; def inner_loop2 params=__zpabi_inner_loop2__a_0,__zpabi_inner_loop2__a_1,__zpabi_inner_loop2__b_0,__zpabi_inner_loop2__b_1,__zpabi_inner_loop2__c_0,__zpabi_inner_loop2__c_1,__zpabi_inner_loop2__d_0,__zpabi_inner_loop2__d_1,__zpabi_inner_loop2__e_0,__zpabi_inner_loop2__e_1,__zpabi_inner_loop2__f_0,__zpabi_inner_loop2__f_1 locals=__local_inner_loop2__a_1,__local_inner_loop2__a_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; def inner_loop3 params=__zpabi_inner_loop3__a_0,__zpabi_inner_loop3__a_1,__zpabi_inner_loop3__b_0,__zpabi_inner_loop3__b_1,__zpabi_inner_loop3__c_0,__zpabi_inner_loop3__c_1,__zpabi_inner_loop3__d_0,__zpabi_inner_loop3__d_1,__zpabi_inner_loop3__e_0,__zpabi_inner_loop3__e_1,__zpabi_inner_loop3__f_0,__zpabi_inner_loop3__f_1 locals=__local_inner_loop3__a_1,__local_inner_loop3__a_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; def inner_loop4 params=__zpabi_inner_loop4__a_0,__zpabi_inner_loop4__a_1,__zpabi_inner_loop4__b_0,__zpabi_inner_loop4__b_1,__zpabi_inner_loop4__c_0,__zpabi_inner_loop4__c_1,__zpabi_inner_loop4__d_0,__zpabi_inner_loop4__d_1,__zpabi_inner_loop4__e_0,__zpabi_inner_loop4__e_1,__zpabi_inner_loop4__f_0,__zpabi_inner_loop4__f_1 locals=__local_inner_loop4__a_0,__local_inner_loop4__a_1 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals=__local_target__0,__local_target__1,__local_target__a_1,__local_target__a_0 indirect=false in_cycle=false
; def validate params=__zpabi_validate__a_0,__zpabi_validate__a_1,__zpabi_validate__b_0,__zpabi_validate__b_1,__zpabi_validate__c_0,__zpabi_validate__c_1,__zpabi_validate__d_0,__zpabi_validate__d_1,__zpabi_validate__e_0,__zpabi_validate__e_1,__zpabi_validate__f_0,__zpabi_validate__f_1 locals=__local_validate__a_1,__local_validate__a_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; call main -> target
; call target -> inner_loop1
; call target -> inner_loop2
; call target -> inner_loop3
; call target -> inner_loop4
; call target -> validate
; share member=inner_loop1:__zpabi_inner_loop1__b_0,__zpabi_inner_loop1__b_1 holder=target live=target:inner_loop1,inner_loop3,inner_loop4 forward=target>inner_loop1 writer=target
; share member=inner_loop1:__zpabi_inner_loop1__c_0,__zpabi_inner_loop1__c_1 holder=target live=target:inner_loop1,inner_loop2,inner_loop4 forward=target>inner_loop1 writer=target
; share member=inner_loop1:__zpabi_inner_loop1__d_0,__zpabi_inner_loop1__d_1 holder=target live=target:inner_loop1,inner_loop2,inner_loop3 forward=target>inner_loop1 writer=target
; share member=inner_loop1:__zpabi_inner_loop1__e_0,__zpabi_inner_loop1__e_1 holder=target live=target:inner_loop1,inner_loop2,inner_loop3,inner_loop4 forward=target>inner_loop1 writer=target
; item inner_loop1 section=.text local=false zero=false
; item inner_loop1@tail1 section=.text local=true zero=false
; item inner_loop1@tail0 section=.text local=true zero=false
; item inner_loop2 section=.text local=false zero=false
; item inner_loop3 section=.text local=false zero=false
; item inner_loop4 section=.text local=false zero=false
; item validate section=.text local=false zero=false
; item target section=.text local=false zero=false
; item target@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp inner_flag size=2 at=auto def=true init=$01,$00 weight=521 local=true
; zp outer_flag size=2 at=auto def=true init=$00,$00 weight=26 local=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

inner_loop1:
   SUBROUTINE

   STA   __local_inner_loop1__a_0
   TXA
   BNE   .inner_loop1@jthread@4
   LDA   __local_inner_loop1__a_0
   CMP   #$01
   BNE   .inner_loop1@jthread@4
   LDA   __zpabi_inner_loop1__b_1
   BNE   .inner_loop1@jthread@4
   LDA   __zpabi_inner_loop1__b_0
   CMP   #$0B
   BNE   .inner_loop1@jthread@4
   LDA   __zpabi_inner_loop1__c_1
   BNE   .inner_loop1@jthread@4
   LDA   __zpabi_inner_loop1__c_0
   CMP   #$0C
   BNE   .inner_loop1@jthread@4
   LDA   __zpabi_inner_loop1__d_1
   BNE   .inner_loop1@jthread@4
   LDA   __zpabi_inner_loop1__d_0
   CMP   #$04
   BNE   .inner_loop1@jthread@4
   LDA   __zpabi_inner_loop1__e_1
   BNE   .inner_loop1@jthread@4
   LDA   __zpabi_inner_loop1__e_0
   CMP   #$14
   BNE   .inner_loop1@jthread@4
   LDA   __zpabi_inner_loop1__f_1
   BNE   .inner_loop1@jthread@4
   LDA   __zpabi_inner_loop1__f_0
   CMP   #$64
   BEQ   .if_end@10
.inner_loop1@jthread@4:
   JMP   inner_loop1@tail1
.if_end@10:
   JMP   inner_loop1@tail0

inner_loop1@tail1:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

inner_loop1@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

inner_loop2:
   SUBROUTINE

   STA   __local_inner_loop2__a_0
   LDA   outer_flag
   ORA   outer_flag+1
   BNE   .if_else@12
   TXA
   BNE   .inner_loop2@jthread@4
   LDA   __local_inner_loop2__a_0
   CMP   #$01
   BNE   .inner_loop2@jthread@4
   LDA   __zpabi_inner_loop2__b_1
   BNE   .inner_loop2@jthread@4
   LDA   __zpabi_inner_loop2__b_0
   CMP   #$02
   BNE   .inner_loop2@jthread@4
   LDA   __zpabi_inner_loop2__c_1
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__c_0
   CMP   #$03
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__d_1
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__d_0
   CMP   #$04
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__e_1
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__e_0
   CMP   #$05
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__f_1
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__f_0
   CMP   #$64
   BEQ   .if_end@11
.inner_loop2@jthread@4:
   JMP   inner_loop1@tail1
.if_else@12:
   TXA
   BNE   .inner_loop2@jthread@9
   LDA   __local_inner_loop2__a_0
   CMP   #$0A
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__b_1
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__b_0
   CMP   #$0B
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__c_1
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__c_0
   CMP   #$0C
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__d_1
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__d_0
   CMP   #$04
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__e_1
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__e_0
   CMP   #$14
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__f_1
   BNE   .inner_loop2@jthread@9
   LDA   __zpabi_inner_loop2__f_0
   CMP   #$64
   BEQ   .if_end@34
.inner_loop2@jthread@9:
   LDX   #$00
   TXA
   RTS
.if_end@34:
.if_end@11:
   JMP   inner_loop1@tail0

inner_loop3:
   SUBROUTINE

   STA   __local_inner_loop3__a_0
   LDA   outer_flag
   ORA   outer_flag+1
   BEQ   .lb_skip@1
   JMP   .if_else@36
.lb_skip@1:
   LDA   inner_flag+1
   BNE   .if_else@38
   LDA   inner_flag
   CMP   #$02
   BNE   .if_else@38
   TXA
   BNE   .inner_loop3@jthread@4
   LDA   __local_inner_loop3__a_0
   CMP   #$01
   BNE   .inner_loop3@jthread@4
   LDA   __zpabi_inner_loop3__b_1
   BNE   .inner_loop3@jthread@4
   LDA   __zpabi_inner_loop3__b_0
   CMP   #$0B
   BNE   .inner_loop3@jthread@4
   LDA   __zpabi_inner_loop3__c_1
   BNE   .inner_loop3@jthread@4
   LDA   __zpabi_inner_loop3__c_0
   CMP   #$03
   BNE   .inner_loop3@jthread@4
   LDA   __zpabi_inner_loop3__d_1
   BNE   .inner_loop3@jthread@4
   LDA   __zpabi_inner_loop3__d_0
   CMP   #$04
   BNE   .inner_loop3@jthread@4
   LDA   __zpabi_inner_loop3__e_1
   BNE   .inner_loop3@jthread@4
   LDA   __zpabi_inner_loop3__e_0
   CMP   #$05
   BNE   .inner_loop3@jthread@4
   LDA   __zpabi_inner_loop3__f_1
   BNE   .inner_loop3@jthread@4
   LDA   __zpabi_inner_loop3__f_0
   CMP   #$64
   BNE   .lb_skip@0
   JMP   .if_end@35
.lb_skip@0:
.inner_loop3@jthread@4:
   JMP   inner_loop1@tail1
.if_else@38:
   TXA
   BNE   .inner_loop3@jthread@9
   LDA   __local_inner_loop3__a_0
   CMP   #$01
   BNE   .inner_loop3@jthread@9
   LDA   __zpabi_inner_loop3__b_1
   BNE   .inner_loop3@jthread@9
   LDA   __zpabi_inner_loop3__b_0
   CMP   #$0B
   BNE   .inner_loop3@jthread@9
   LDA   __zpabi_inner_loop3__c_1
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__c_0
   CMP   #$0C
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__d_1
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__d_0
   CMP   #$04
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__e_1
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__e_0
   CMP   #$05
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__f_1
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__f_0
   CMP   #$64
   BEQ   .if_end@35
.inner_loop3@jthread@9:
   LDX   #$00
   TXA
   RTS
.if_else@36:
   TXA
   BNE   .inner_loop3@jthread@14
   LDA   __local_inner_loop3__a_0
   CMP   #$0A
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__b_1
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__b_0
   CMP   #$0B
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__c_1
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__c_0
   CMP   #$0C
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__d_1
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__d_0
   CMP   #$04
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__e_1
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__e_0
   CMP   #$14
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__f_1
   BNE   .inner_loop3@jthread@14
   LDA   __zpabi_inner_loop3__f_0
   CMP   #$64
   BEQ   .if_end@71
.inner_loop3@jthread@14:
   LDX   #$00
   TXA
   RTS
.if_end@71:
.if_end@35:
   JMP   inner_loop1@tail0

inner_loop4:
   SUBROUTINE

   CLC
   ADC   __zpabi_inner_loop4__b_0
   STA   __local_inner_loop4__a_0
   TXA
   ADC   __zpabi_inner_loop4__b_1
   STA   __local_inner_loop4__a_1
   LDA   __local_inner_loop4__a_0
   CLC
   ADC   __zpabi_inner_loop4__c_0
   STA   __local_inner_loop4__a_0
   LDA   __local_inner_loop4__a_1
   ADC   __zpabi_inner_loop4__c_1
   STA   __local_inner_loop4__a_1
   LDA   __local_inner_loop4__a_0
   CLC
   ADC   __zpabi_inner_loop4__d_0
   STA   __local_inner_loop4__a_0
   LDA   __local_inner_loop4__a_1
   ADC   __zpabi_inner_loop4__d_1
   STA   __local_inner_loop4__a_1
   LDA   __local_inner_loop4__a_0
   CLC
   ADC   __zpabi_inner_loop4__e_0
   STA   __local_inner_loop4__a_0
   LDA   __local_inner_loop4__a_1
   ADC   __zpabi_inner_loop4__e_1
   STA   __local_inner_loop4__a_1
   LDA   __local_inner_loop4__a_0
   CLC
   ADC   __zpabi_inner_loop4__f_0
   STA   __local_inner_loop4__a_0
   LDA   __local_inner_loop4__a_1
   ADC   __zpabi_inner_loop4__f_1
   TAX
   LDA   __local_inner_loop4__a_0
   RTS

validate:
   SUBROUTINE

   STA   __local_validate__a_0
   TXA
   BNE   .validate@jthread@4
   LDA   __local_validate__a_0
   CMP   #$0A
   BNE   .validate@jthread@4
   LDA   __zpabi_validate__b_1
   BNE   .validate@jthread@4
   LDA   __zpabi_validate__b_0
   CMP   #$0B
   BNE   .validate@jthread@4
   LDA   __zpabi_validate__c_1
   BNE   .validate@jthread@4
   LDA   __zpabi_validate__c_0
   CMP   #$0C
   BNE   .validate@jthread@4
   LDA   __zpabi_validate__d_1
   BNE   .validate@jthread@4
   LDA   __zpabi_validate__d_0
   CMP   #$04
   BNE   .validate@jthread@4
   LDA   __zpabi_validate__e_1
   BNE   .validate@jthread@4
   LDA   __zpabi_validate__e_0
   CMP   #$14
   BNE   .validate@jthread@4
   LDA   __zpabi_validate__f_1
   BNE   .validate@jthread@4
   LDA   __zpabi_validate__f_0
   CMP   #$64
   BEQ   .if_end@82
.validate@jthread@4:
   JMP   inner_loop1@tail1
.if_end@82:
   JMP   inner_loop1@tail0

target:
   SUBROUTINE

   LDA   #$02
   STA   __zpabi_inner_loop1__b_0
   LDA   #$00
   STA   __zpabi_inner_loop1__b_1
   LDA   #$03
   STA   __zpabi_inner_loop1__c_0
   LDA   #$00
   STA   __zpabi_inner_loop1__c_1
   LDA   #$04
   STA   __zpabi_inner_loop1__d_0
   LDA   #$00
   STA   __zpabi_inner_loop1__d_1
   LDA   #$05
   STA   __zpabi_inner_loop1__e_0
   LDA   #$00
   STA   __zpabi_inner_loop1__e_1
   LDA   #$01
   STA   __local_target__a_0
.loop@0_continue:
   LDA   outer_flag
   SEC
   SBC   #$02
   LDA   outer_flag+1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .lb_skip@2
   JMP   .loop@0_break
.lb_skip@2:
.loop@1_continue:
   LDA   inner_flag
   SEC
   SBC   #$01
   LDA   inner_flag+1
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .loop@1_break
   LDA   #$64
   STA   __zpabi_inner_loop1__f_0
   LDA   #$00
   STA   __zpabi_inner_loop1__f_1
   LDX   #$00
   LDA   __local_target__a_0
   JSR   inner_loop1
   STX   __local_target__0
   ORA   __local_target__0
   BNE   .if_end@83
   JMP   inner_loop1@tail0
.if_end@83:
   LDA   #$01
   STA   inner_flag
   LDA   #$0A
   STA   __local_target__a_0
   LDA   #$00
   STA   inner_flag+1
   JMP   .loop@1_continue
.loop@1_break:
.loop@2_continue:
   LDA   inner_flag
   SEC
   SBC   #$02
   LDA   inner_flag+1
   SBC   #$00
   BVC   .jcmp_novf@2
   EOR   #$80
.jcmp_novf@2:
   BPL   .loop@2_break
   LDA   __zpabi_inner_loop1__b_0
   STA   __zpabi_inner_loop2__b_0
   LDA   #$00
   STA   __zpabi_inner_loop2__b_1
   STA   __zpabi_inner_loop2__c_1
   LDA   __zpabi_inner_loop1__c_0
   STA   __zpabi_inner_loop2__c_0
   LDA   __zpabi_inner_loop1__d_0
   STA   __zpabi_inner_loop2__d_0
   LDA   #$00
   STA   __zpabi_inner_loop2__d_1
   STA   __zpabi_inner_loop2__e_1
   LDA   __zpabi_inner_loop1__e_0
   STA   __zpabi_inner_loop2__e_0
   LDA   #$64
   STA   __zpabi_inner_loop2__f_0
   LDA   #$00
   STA   __zpabi_inner_loop2__f_1
   LDX   #$00
   LDA   __local_target__a_0
   JSR   inner_loop2
   STX   __local_target__0
   ORA   __local_target__0
   BNE   .if_end@84
   LDX   #$00
   LDA   #$02
   RTS
.if_end@84:
   LDA   #$02
   STA   inner_flag
   LDA   #$0B
   STA   __zpabi_inner_loop1__b_0
   LDA   #$00
   STA   inner_flag+1
   STA   __zpabi_inner_loop1__b_1
   JMP   .loop@2_continue
.loop@2_break:
.loop@3_continue:
   JSR   target@run0
   BVC   .jcmp_novf@3
   EOR   #$80
.jcmp_novf@3:
   BPL   .loop@3_break
   LDA   __zpabi_inner_loop1__b_0
   STA   __zpabi_inner_loop3__b_0
   LDA   #$00
   STA   __zpabi_inner_loop3__b_1
   STA   __zpabi_inner_loop3__c_1
   LDA   __zpabi_inner_loop1__c_0
   STA   __zpabi_inner_loop3__c_0
   LDA   __zpabi_inner_loop1__d_0
   STA   __zpabi_inner_loop3__d_0
   LDA   #$00
   STA   __zpabi_inner_loop3__d_1
   STA   __zpabi_inner_loop3__e_1
   LDA   __zpabi_inner_loop1__e_0
   STA   __zpabi_inner_loop3__e_0
   LDA   #$64
   STA   __zpabi_inner_loop3__f_0
   LDA   #$00
   STA   __zpabi_inner_loop3__f_1
   LDX   #$00
   LDA   __local_target__a_0
   JSR   inner_loop3
   STX   __local_target__0
   ORA   __local_target__0
   BNE   .if_end@85
   LDX   #$00
   LDA   #$03
   RTS
.if_end@85:
   INC   inner_flag
   BNE   .inc_done@0
   INC   inner_flag+1
.inc_done@0:
   LDA   #$0C
   STA   __zpabi_inner_loop1__c_0
   LDA   #$00
   STA   __zpabi_inner_loop1__c_1
   JMP   .loop@3_continue
.loop@3_break:
.loop@4_continue:
   JSR   target@run0
   BVC   .jcmp_novf@4
   EOR   #$80
.jcmp_novf@4:
   BPL   .loop@4_break
   LDA   __zpabi_inner_loop1__b_0
   STA   __zpabi_inner_loop4__b_0
   LDA   #$00
   STA   __zpabi_inner_loop4__b_1
   STA   __zpabi_inner_loop4__c_1
   LDA   __zpabi_inner_loop1__c_0
   STA   __zpabi_inner_loop4__c_0
   LDA   __zpabi_inner_loop1__d_0
   STA   __zpabi_inner_loop4__d_0
   LDA   #$00
   STA   __zpabi_inner_loop4__d_1
   STA   __zpabi_inner_loop4__e_1
   LDA   __zpabi_inner_loop1__e_0
   STA   __zpabi_inner_loop4__e_0
   LDA   #$64
   STA   __zpabi_inner_loop4__f_0
   LDA   #$00
   STA   __zpabi_inner_loop4__f_1
   LDX   #$00
   LDA   __local_target__a_0
   JSR   inner_loop4
   LDA   #$0D
   STA   __zpabi_inner_loop1__d_0
   LDA   #$00
   STA   __zpabi_inner_loop1__d_1
   JMP   .loop@4_continue
.loop@4_break:
   INC   outer_flag
   BNE   .inc_done@1
   INC   outer_flag+1
.inc_done@1:
   LDA   #$14
   STA   __zpabi_inner_loop1__e_0
   LDA   #$00
   STA   inner_flag
   STA   inner_flag+1
   STA   __zpabi_inner_loop1__e_1
   JMP   .loop@0_continue
.loop@0_break:
   LDA   __zpabi_inner_loop1__b_0
   STA   __zpabi_validate__b_0
   LDA   #$00
   STA   __zpabi_validate__b_1
   STA   __zpabi_validate__c_1
   LDA   __zpabi_inner_loop1__c_0
   STA   __zpabi_validate__c_0
   LDA   __zpabi_inner_loop1__d_0
   STA   __zpabi_validate__d_0
   LDA   #$00
   STA   __zpabi_validate__d_1
   STA   __zpabi_validate__e_1
   LDA   __zpabi_inner_loop1__e_0
   STA   __zpabi_validate__e_0
   LDA   #$64
   STA   __zpabi_validate__f_0
   LDA   #$00
   STA   __zpabi_validate__f_1
   LDX   #$00
   LDA   __local_target__a_0
   JSR   validate
   STX   __local_target__0
   ORA   __local_target__0
   BNE   .if_end@86
   LDX   #$00
   LDA   #$04
   RTS
.if_end@86:
   JMP   inner_loop1@tail1

target@run0:
   SUBROUTINE

   LDA   inner_flag
   SEC
   SBC   #$04
   LDA   inner_flag+1
   SBC   #$00
   RTS

main:
   SUBROUTINE

   JMP   target

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   outer_flag
   STA   outer_flag+1
   STA   inner_flag+1
   LDA   #$01
   STA   inner_flag
   RTS
