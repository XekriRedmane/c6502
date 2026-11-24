SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_f__in_reg_0	EQU	$80
__zpabi_f__in_reg_1	EQU	$81
__zpabi_f__in_reg_2	EQU	$82
__zpabi_f__in_reg_3	EQU	$83
__zpabi_f__in_reg_4	EQU	$84
__zpabi_f__in_reg_5	EQU	$85
__zpabi_f__in_reg_6	EQU	$86
__zpabi_f__in_reg_7	EQU	$87
__zpabi_f__in_reg_8	EQU	$88
__zpabi_f__in_reg_9	EQU	$89
__zpabi_f__in_reg_10	EQU	$8A
__zpabi_f__a_0	EQU	$8B
__zpabi_f__a_1	EQU	$8C
__zpabi_f__b_0	EQU	$8D
__zpabi_f__b_1	EQU	$8E
__zpabi_f__c_0	EQU	$8F
__zpabi_f__c_1	EQU	$90
__zpabi_f__d_0	EQU	$91
__zpabi_f__d_1	EQU	$92
__zpabi_f__e_0	EQU	$93
__zpabi_f__e_1	EQU	$94
__zpabi_f__on_stack_0	EQU	$95
__zpabi_f__on_stack_1	EQU	$96
__zpabi_f__on_stack_2	EQU	$97
__zpabi_f__on_stack_3	EQU	$98
__zpabi_f__on_stack_4	EQU	$99
__zpabi_f__on_stack_5	EQU	$9A
__zpabi_f__on_stack_6	EQU	$9B
__zpabi_f__on_stack_7	EQU	$9C
__zpabi_f__on_stack_8	EQU	$9D
__zpabi_f__on_stack_9	EQU	$9E
__zpabi_f__on_stack_10	EQU	$9F
__local_f__0	EQU	$A1
__local_f__2	EQU	$A3

; @zp-link-meta-begin
; def f params=__zpabi_f__in_reg_0,__zpabi_f__in_reg_1,__zpabi_f__in_reg_2,__zpabi_f__in_reg_3,__zpabi_f__in_reg_4,__zpabi_f__in_reg_5,__zpabi_f__in_reg_6,__zpabi_f__in_reg_7,__zpabi_f__in_reg_8,__zpabi_f__in_reg_9,__zpabi_f__in_reg_10,__zpabi_f__a_0,__zpabi_f__a_1,__zpabi_f__b_0,__zpabi_f__b_1,__zpabi_f__c_0,__zpabi_f__c_1,__zpabi_f__d_0,__zpabi_f__d_1,__zpabi_f__e_0,__zpabi_f__e_1,__zpabi_f__on_stack_0,__zpabi_f__on_stack_1,__zpabi_f__on_stack_2,__zpabi_f__on_stack_3,__zpabi_f__on_stack_4,__zpabi_f__on_stack_5,__zpabi_f__on_stack_6,__zpabi_f__on_stack_7,__zpabi_f__on_stack_8,__zpabi_f__on_stack_9,__zpabi_f__on_stack_10 locals=__local_f__0,__local_f__1,__local_f__2 indirect=false in_cycle=false changes=__zpabi_f__in_reg_0,__zpabi_f__in_reg_1,__zpabi_f__in_reg_2,__zpabi_f__in_reg_3,__zpabi_f__in_reg_4,__zpabi_f__in_reg_5,__zpabi_f__in_reg_6,__zpabi_f__in_reg_7,__zpabi_f__in_reg_8,__zpabi_f__in_reg_9,__zpabi_f__in_reg_10,__zpabi_f__on_stack_0,__zpabi_f__on_stack_1,__zpabi_f__on_stack_2,__zpabi_f__on_stack_3,__zpabi_f__on_stack_4,__zpabi_f__on_stack_5,__zpabi_f__on_stack_6,__zpabi_f__on_stack_7,__zpabi_f__on_stack_8,__zpabi_f__on_stack_9,__zpabi_f__on_stack_10
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> f
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
   SUBROUTINE

   LDY   #$00
   LDX   #$00
.loop@0_start:
   TYA
   SEC
   SBC   #$09
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __zpabi_f__in_reg_0,Y
   STA   __local_f__2
   LDA   __zpabi_f__on_stack_0,Y
   STA   __local_f__0
   CPX   #$00
   BNE   .if_else@1
   CPY   #$02
   BNE   .if_else@1
   LDA   __local_f__2
   CMP   #$04
   BNE   .f@jthread@0
   LDA   __local_f__0
   CMP   #$04
   BEQ   .if_end@0
.f@jthread@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_else@1:
   CPX   #$00
   BNE   .if_else@6
   CPY   #$03
   BNE   .if_else@6
   LDA   __local_f__2
   CMP   #$05
   BNE   .f@jthread@1
   LDA   __local_f__0
   CMP   #$05
   BEQ   .if_end@5
.f@jthread@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_else@6:
   CPX   #$00
   BNE   .if_else@11
   CPY   #$08
   BNE   .if_else@11
   LDA   __local_f__2
   CMP   #$06
   BNE   .f@jthread@2
   LDA   __local_f__0
   CMP   #$06
   BEQ   .if_end@10
.f@jthread@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_else@11:
   LDA   __local_f__2
   BNE   .f@jthread@3
   LDA   __local_f__0
   BEQ   .if_end@17
.f@jthread@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@17:
.if_end@10:
.if_end@5:
.if_end@0:
   INY
   BNE   .loop@0_start
   INX
   JMP   .loop@0_start
.loop@0_break:
   LDA   __zpabi_f__a_1
   BNE   .f@jthread@7
   LDA   __zpabi_f__a_0
   CMP   #$65
   BNE   .f@jthread@7
   LDA   __zpabi_f__b_1
   BNE   .f@jthread@7
   LDA   __zpabi_f__b_0
   CMP   #$66
   BNE   .f@jthread@7
   LDA   __zpabi_f__c_1
   BNE   .f@jthread@7
   LDA   __zpabi_f__c_0
   CMP   #$67
   BNE   .f@jthread@7
   LDA   __zpabi_f__d_1
   BNE   .f@jthread@7
   LDA   __zpabi_f__d_0
   CMP   #$68
   BNE   .f@jthread@7
   LDA   __zpabi_f__e_1
   BNE   .f@jthread@7
   LDA   __zpabi_f__e_0
   CMP   #$69
   BEQ   .if_end@26
.f@jthread@7:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@26:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #$04
   STA   on_page_boundary+2
   STA   __zpabi_f__in_reg_2
   STA   __zpabi_f__on_stack_2
   LDA   #$05
   STA   on_page_boundary+3
   STA   __zpabi_f__in_reg_3
   STA   __zpabi_f__on_stack_3
   LDA   #$06
   STA   on_page_boundary+8
   STA   __zpabi_f__in_reg_8
   STA   __zpabi_f__on_stack_8
   LDA   on_page_boundary
   STA   __zpabi_f__in_reg_0
   STA   __zpabi_f__on_stack_0
   LDA   on_page_boundary+1
   STA   __zpabi_f__in_reg_1
   STA   __zpabi_f__on_stack_1
   LDA   on_page_boundary+4
   STA   __zpabi_f__in_reg_4
   STA   __zpabi_f__on_stack_4
   LDA   on_page_boundary+5
   STA   __zpabi_f__in_reg_5
   STA   __zpabi_f__on_stack_5
   LDA   on_page_boundary+6
   STA   __zpabi_f__in_reg_6
   STA   __zpabi_f__on_stack_6
   LDA   on_page_boundary+7
   STA   __zpabi_f__in_reg_7
   STA   __zpabi_f__on_stack_7
   LDA   on_page_boundary+9
   STA   __zpabi_f__in_reg_9
   STA   __zpabi_f__on_stack_9
   LDA   #$65
   STA   __zpabi_f__a_0
   LDA   #$00
   STA   __zpabi_f__a_1
   STA   __zpabi_f__b_1
   STA   __zpabi_f__c_1
   STA   __zpabi_f__d_1
   STA   __zpabi_f__e_1
   LDA   #$66
   STA   __zpabi_f__b_0
   LDA   #$67
   STA   __zpabi_f__c_0
   LDA   #$68
   STA   __zpabi_f__d_0
   LDA   #$69
   STA   __zpabi_f__e_0
   LDA   on_page_boundary+10
   STA   __zpabi_f__in_reg_10
   STA   __zpabi_f__on_stack_10
   JMP   f
