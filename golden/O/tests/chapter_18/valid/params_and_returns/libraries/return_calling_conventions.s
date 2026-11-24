SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_return_mixed__retval_10	EQU	$80
__local_return_on_stack__retval_16	EQU	$80
__zpabi_leaf_call__t_i_0	EQU	$80
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strncmp__s2_0	EQU	$80
__local_return_on_stack__retval_15	EQU	$81
__zpabi_leaf_call__t_i_1	EQU	$81
__zpabi_strcmp__s2_1	EQU	$81
__zpabi_strncmp__s2_1	EQU	$81
__local_return_on_stack__retval_14	EQU	$82
__zpabi_leaf_call__t_i_2	EQU	$82
__zpabi_strncmp__n_0	EQU	$82
__zpabi_leaf_call__t_i_3	EQU	$83
__zpabi_strncmp__n_1	EQU	$83
__local_return_double_struct__retval_3	EQU	$84
__local_return_on_stack__retval_12	EQU	$84
__zpabi_leaf_call__t_i_4	EQU	$84
__zpabi_strncmp__n_2	EQU	$84
__local_return_double_struct__retval_2	EQU	$85
__local_return_on_stack__retval_11	EQU	$85
__zpabi_leaf_call__t_i_5	EQU	$85
__zpabi_strncmp__n_3	EQU	$85
__local_return_double_struct__retval_1	EQU	$86
__local_return_on_stack__retval_10	EQU	$86
__local_return_two_double_struct__retval_9	EQU	$86
__zpabi_leaf_call__t_i_6	EQU	$86
__zpabi_leaf_call__c_0	EQU	$87
__zpabi_leaf_call__c_1	EQU	$88
__local_return_mixed__retval_1	EQU	$89
__zpabi_leaf_call__d_0	EQU	$89
__zpabi_leaf_call__d_1	EQU	$8A
__zpabi_leaf_call__d_2	EQU	$8B
__zpabi_leaf_call__d_3	EQU	$8C
__local_return_two_double_struct__retval_2	EQU	$8D
__zpabi_leaf_call__d_4	EQU	$8D
__zpabi_leaf_call__d_5	EQU	$8E
__local_return_on_stack__retval_1	EQU	$8F
__zpabi_leaf_call__d_6	EQU	$8F
__zpabi_leaf_call__d_7	EQU	$90
__local_pass_and_return_regs__0	EQU	$91
__local_pass_and_return_regs__1	EQU	$92
__local_pass_and_return_regs__2	EQU	$93
__local_pass_and_return_regs__3	EQU	$94
__local_pass_and_return_regs__4	EQU	$95
__local_pass_and_return_regs__5	EQU	$96
__local_pass_and_return_regs__retval	EQU	$97
__local_pass_and_return_regs__stackbytes	EQU	$A8
__local_leaf_call__1	EQU	$B1

; @zp-link-meta-begin
; def leaf_call params=__zpabi_leaf_call__t_i_0,__zpabi_leaf_call__t_i_1,__zpabi_leaf_call__t_i_2,__zpabi_leaf_call__t_i_3,__zpabi_leaf_call__t_i_4,__zpabi_leaf_call__t_i_5,__zpabi_leaf_call__t_i_6,__zpabi_leaf_call__c_0,__zpabi_leaf_call__c_1,__zpabi_leaf_call__d_0,__zpabi_leaf_call__d_1,__zpabi_leaf_call__d_2,__zpabi_leaf_call__d_3,__zpabi_leaf_call__d_4,__zpabi_leaf_call__d_5,__zpabi_leaf_call__d_6,__zpabi_leaf_call__d_7 locals=__local_leaf_call__0,__local_leaf_call__1,__local_leaf_call__2,__local_leaf_call__3 indirect=false in_cycle=false changes=__zpabi_leaf_call__t_i_0,__zpabi_leaf_call__t_i_1,__zpabi_leaf_call__t_i_2,__zpabi_leaf_call__t_i_3,__zpabi_leaf_call__t_i_4,__zpabi_leaf_call__t_i_5,__zpabi_leaf_call__t_i_6
; def pass_and_return_regs params= locals=__local_pass_and_return_regs__0,__local_pass_and_return_regs__1,__local_pass_and_return_regs__2,__local_pass_and_return_regs__3,__local_pass_and_return_regs__4,__local_pass_and_return_regs__5,__local_pass_and_return_regs__retval,__local_pass_and_return_regs__7,__local_pass_and_return_regs__8,__local_pass_and_return_regs__9,__local_pass_and_return_regs__10,__local_pass_and_return_regs__11,__local_pass_and_return_regs__12,__local_pass_and_return_regs__13,__local_pass_and_return_regs__14,__local_pass_and_return_regs__15,__local_pass_and_return_regs__16,__local_pass_and_return_regs__17,__local_pass_and_return_regs__18,__local_pass_and_return_regs__19,__local_pass_and_return_regs__20,__local_pass_and_return_regs__21,__local_pass_and_return_regs__22,__local_pass_and_return_regs__stackbytes,__local_pass_and_return_regs__24,__local_pass_and_return_regs__25,__local_pass_and_return_regs__26,__local_pass_and_return_regs__27,__local_pass_and_return_regs__28,__local_pass_and_return_regs__29,__local_pass_and_return_regs__30 indirect=false in_cycle=false
; def return_double_struct params= locals=__local_return_double_struct__retval_7,__local_return_double_struct__retval_6,__local_return_double_struct__retval_5,__local_return_double_struct__retval_4,__local_return_double_struct__retval_3,__local_return_double_struct__retval_2,__local_return_double_struct__retval_1,__local_return_double_struct__retval_0 indirect=false in_cycle=false
; def return_int_struct params= locals=__local_return_int_struct__retval_2,__local_return_int_struct__retval_1,__local_return_int_struct__retval_0 indirect=false in_cycle=false
; def return_mixed params= locals=__local_return_mixed__retval_10,__local_return_mixed__retval_9,__local_return_mixed__retval_8,__local_return_mixed__retval_7,__local_return_mixed__retval_6,__local_return_mixed__retval_5,__local_return_mixed__retval_4,__local_return_mixed__retval_3,__local_return_mixed__retval_2,__local_return_mixed__retval_1,__local_return_mixed__retval_0 indirect=false in_cycle=false
; def return_mixed2 params= locals=__local_return_mixed2__retval_8,__local_return_mixed2__retval_7,__local_return_mixed2__retval_6,__local_return_mixed2__retval_5,__local_return_mixed2__retval_4,__local_return_mixed2__retval_3,__local_return_mixed2__retval_2,__local_return_mixed2__retval_1,__local_return_mixed2__retval_0 indirect=false in_cycle=false
; def return_on_stack params= locals=__local_return_on_stack__retval_16,__local_return_on_stack__retval_15,__local_return_on_stack__retval_14,__local_return_on_stack__retval_13,__local_return_on_stack__retval_12,__local_return_on_stack__retval_11,__local_return_on_stack__retval_10,__local_return_on_stack__retval_9,__local_return_on_stack__retval_8,__local_return_on_stack__retval_7,__local_return_on_stack__retval_6,__local_return_on_stack__retval_5,__local_return_on_stack__retval_4,__local_return_on_stack__retval_3,__local_return_on_stack__retval_2,__local_return_on_stack__retval_1,__local_return_on_stack__retval_0 indirect=false in_cycle=false
; def return_two_double_struct params= locals=__local_return_two_double_struct__retval_15,__local_return_two_double_struct__retval_14,__local_return_two_double_struct__retval_13,__local_return_two_double_struct__retval_12,__local_return_two_double_struct__retval_11,__local_return_two_double_struct__retval_10,__local_return_two_double_struct__retval_9,__local_return_two_double_struct__retval_8,__local_return_two_double_struct__retval_7,__local_return_two_double_struct__retval_6,__local_return_two_double_struct__retval_5,__local_return_two_double_struct__retval_4,__local_return_two_double_struct__retval_3,__local_return_two_double_struct__retval_2,__local_return_two_double_struct__retval_1,__local_return_two_double_struct__retval_0 indirect=false in_cycle=false
; def return_two_int_struct params= locals=__local_return_two_int_struct__retval_9,__local_return_two_int_struct__retval_8,__local_return_two_int_struct__retval_7,__local_return_two_int_struct__retval_6,__local_return_two_int_struct__retval_5,__local_return_two_int_struct__retval_4,__local_return_two_int_struct__retval_3,__local_return_two_int_struct__retval_2,__local_return_two_int_struct__retval_1,__local_return_two_int_struct__retval_0 indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; ext strncmp params=__zpabi_strncmp__s1_0,__zpabi_strncmp__s1_1,__zpabi_strncmp__s2_0,__zpabi_strncmp__s2_1,__zpabi_strncmp__n_0,__zpabi_strncmp__n_1,__zpabi_strncmp__n_2,__zpabi_strncmp__n_3 param_regs=A,X,,,,,,
; call pass_and_return_regs -> leaf_call
; call pass_and_return_regs -> strcmp
; item return_int_struct section=.text local=false zero=false
; item return_two_int_struct section=.text local=false zero=false
; item return_double_struct section=.text local=false zero=false
; item return_two_double_struct section=.text local=false zero=false
; item return_mixed section=.text local=false zero=false
; item return_mixed2 section=.text local=false zero=false
; item return_on_stack section=.text local=false zero=false
; item leaf_call section=.text local=false zero=false
; item pass_and_return_regs section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

return_int_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$02
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

return_two_int_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$0A
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$31
   INY
   STA   (DPTR),Y
   LDA   #$32
   INY
   STA   (DPTR),Y
   LDA   #$33
   INY
   STA   (DPTR),Y
   LDA   #$34
   INY
   STA   (DPTR),Y
   LDA   #$35
   INY
   STA   (DPTR),Y
   LDA   #$36
   INY
   STA   (DPTR),Y
   LDA   #$37
   INY
   STA   (DPTR),Y
   LDA   #$38
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
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
   RTS

return_double_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
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

   LDY   #$00
   STY   __local_return_double_struct__retval_1
   STY   __local_return_double_struct__retval_2
   STY   __local_return_double_struct__retval_3
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_return_double_struct__retval_1
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_return_double_struct__retval_2
   LDY   #$02
   STA   (DPTR),Y
   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_return_double_struct__retval_3
   DEY
   STA   (DPTR),Y
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   STA   (DPTR),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$28
   INY
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$59
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$40
   LDY   #$07
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
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
   RTS

return_two_double_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
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

   LDY   #$99
   STY   __local_return_two_double_struct__retval_2
   LDY   #$33
   STY   __local_return_two_double_struct__retval_9
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$9A
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$99
   INY
   STA   (DPTR),Y
   LDA   __local_return_two_double_struct__retval_2
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$99
   INY
   STA   (DPTR),Y
   LDA   #$21
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDA   #$33
   INY
   STA   (DPTR),Y
   LDA   __local_return_two_double_struct__retval_9
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$33
   INY
   STA   (DPTR),Y
   LDA   #$1F
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
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
   RTS

return_mixed:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
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

   LDY   #$00
   STY   __local_return_mixed__retval_1
   STY   __local_return_mixed__retval_10
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDA   __local_return_mixed__retval_1
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$24
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDA   #$61
   INY
   STA   (DPTR),Y
   LDA   #$62
   INY
   STA   (DPTR),Y
   LDA   __local_return_mixed__retval_10
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
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
   RTS

return_mixed2:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$7F
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$55
   INY
   STA   (DPTR),Y
   LDA   #$7A
   INY
   STA   (DPTR),Y
   LDA   #$65
   INY
   STA   (DPTR),Y
   LDA   #$04
   INY
   STA   (DPTR),Y
   LDA   #$7E
   INY
   STA   (DPTR),Y
   LDA   #$2E
   INY
   STA   (DPTR),Y
   LDA   #$49
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
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
   RTS

return_on_stack:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
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

   LDY   #$00
   STY   __local_return_on_stack__retval_1
   TYA
   STA   __local_return_on_stack__retval_10
   LDY   #$64
   STY   __local_return_on_stack__retval_11
   LDY   #$00
   STY   __local_return_on_stack__retval_12
   STA   __local_return_on_stack__retval_14
   LDY   #$2C
   STY   __local_return_on_stack__retval_15
   STA   __local_return_on_stack__retval_16
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDA   __local_return_on_stack__retval_1
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$F4
   INY
   STA   (DPTR),Y
   LDA   #$3F
   INY
   STA   (DPTR),Y
   LDA   #$78
   INY
   STA   (DPTR),Y
   LDA   #$79
   INY
   STA   (DPTR),Y
   LDA   __local_return_on_stack__retval_10
   INY
   STA   (DPTR),Y
   LDA   __local_return_on_stack__retval_11
   INY
   STA   (DPTR),Y
   LDA   __local_return_on_stack__retval_12
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   __local_return_on_stack__retval_14
   INY
   STA   (DPTR),Y
   LDA   __local_return_on_stack__retval_15
   INY
   STA   (DPTR),Y
   LDA   __local_return_on_stack__retval_16
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
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
   RTS

leaf_call:
   SUBROUTINE

   LDA   __zpabi_leaf_call__t_i_0
   CMP   #$5F
   BNE   .leaf_call@jthread@2
   LDA   __zpabi_leaf_call__t_i_0+1
   STA   __local_leaf_call__1
   LDA   __zpabi_leaf_call__t_i_0+2
   BNE   .leaf_call@jthread@3
   LDA   __local_leaf_call__1
   CMP   #$05
   BNE   .leaf_call@jthread@3
   LDA   __zpabi_leaf_call__t_i_0+3
   STA   __local_leaf_call__1
   LDA   __zpabi_leaf_call__t_i_0+4
   BNE   .leaf_call@jthread@3
   LDA   __local_leaf_call__1
   CMP   #$06
   BNE   .leaf_call@jthread@3
   LDA   __zpabi_leaf_call__t_i_0+5
   STA   __local_leaf_call__1
   LDA   __zpabi_leaf_call__t_i_0+6
   BNE   .leaf_call@jthread@3
   LDA   __local_leaf_call__1
   CMP   #$07
   BEQ   .if_end@6
.leaf_call@jthread@2:
   LDX   #$00
   TXA
   RTS
.if_end@6:
   LDA   __zpabi_leaf_call__c_1
   BNE   .leaf_call@jthread@3
   LDA   __zpabi_leaf_call__c_0
   CMP   #$70
   BNE   .leaf_call@jthread@3
   LDA   __zpabi_leaf_call__d_7
   CMP   #$40
   BNE   .leaf_call@jthread@3
   LDA   __zpabi_leaf_call__d_6
   CMP   #$12
   BNE   .leaf_call@jthread@3
   LDA   __zpabi_leaf_call__d_5
   CMP   #$3D
   BNE   .leaf_call@jthread@3
   LDA   __zpabi_leaf_call__d_4
   CMP   #$70
   BNE   .leaf_call@jthread@3
   LDA   __zpabi_leaf_call__d_3
   CMP   #$A3
   BNE   .leaf_call@jthread@3
   LDA   __zpabi_leaf_call__d_2
   CMP   #$D7
   BNE   .leaf_call@jthread@3
   LDA   __zpabi_leaf_call__d_1
   CMP   #$0A
   BNE   .leaf_call@jthread@3
   LDA   __zpabi_leaf_call__d_0
   CMP   #$3D
   BEQ   .if_end@9
.leaf_call@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@9:
   LDX   #$00
   LDA   #$01
   RTS

pass_and_return_regs:
   SUBROUTINE

   ; prologue: 40 arg bytes, 0 local bytes
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

   LDA   #$7A
   STA   __local_pass_and_return_regs__stackbytes
   LDA   #$79
   STA   __local_pass_and_return_regs__stackbytes+1
   LDA   #$78
   STA   __local_pass_and_return_regs__stackbytes+2
   LDA   #$77
   STA   __local_pass_and_return_regs__stackbytes+3
   LDA   #$76
   STA   __local_pass_and_return_regs__stackbytes+4
   LDA   #$75
   STA   __local_pass_and_return_regs__stackbytes+5
   LDA   #$74
   STA   __local_pass_and_return_regs__stackbytes+6
   LDA   #$00
   STA   __local_pass_and_return_regs__stackbytes+7
   LDY   #$11
.clear@10:
   DEY
   LDA   #>__local_pass_and_return_regs__retval
   STA   __local_pass_and_return_regs__1
   LDA   #$00
   STA   __local_pass_and_return_regs__retval,Y
   TYA
   BNE   .clear@10
   CLC
   LDA   FP
   ADC   #$0F
   STA   __local_pass_and_return_regs__0
   LDA   FP+1
   ADC   #$00
   STA   __local_pass_and_return_regs__0+1
   LDY   #$00
   LDA   (__local_pass_and_return_regs__0),Y
   STA   __zpabi_leaf_call__c_0
   CLC
   LDA   FP
   ADC   #$0F
   STA   __local_pass_and_return_regs__0
   LDA   FP+1
   ADC   #$00
   STA   __local_pass_and_return_regs__0+1
   LDA   __local_pass_and_return_regs__0
   STA   DPTR
   LDA   __local_pass_and_return_regs__1
   STA   DPTR+1
   INY
   LDA   (__local_pass_and_return_regs__0),Y
   STA   __zpabi_leaf_call__d_0
   INY
   LDA   (__local_pass_and_return_regs__0),Y
   STA   __zpabi_leaf_call__d_1
   INY
   LDA   (__local_pass_and_return_regs__0),Y
   STA   __zpabi_leaf_call__d_2
   INY
   LDA   (__local_pass_and_return_regs__0),Y
   STA   __zpabi_leaf_call__d_3
   INY
   LDA   (__local_pass_and_return_regs__0),Y
   STA   __zpabi_leaf_call__d_4
   INY
   LDA   (__local_pass_and_return_regs__0),Y
   STA   __zpabi_leaf_call__d_5
   INY
   LDA   (__local_pass_and_return_regs__0),Y
   STA   __zpabi_leaf_call__d_6
   INY
   LDA   (__local_pass_and_return_regs__0),Y
   STA   __zpabi_leaf_call__d_7
   LDY   #$1A
   LDA   (FP),Y
   STA   __zpabi_leaf_call__t_i_0
   INY
   LDA   (FP),Y
   STA   __zpabi_leaf_call__t_i_1
   INY
   LDA   (FP),Y
   STA   __zpabi_leaf_call__t_i_2
   INY
   LDA   (FP),Y
   STA   __zpabi_leaf_call__t_i_3
   INY
   LDA   (FP),Y
   STA   __zpabi_leaf_call__t_i_4
   INY
   LDA   (FP),Y
   STA   __zpabi_leaf_call__t_i_5
   INY
   LDA   (FP),Y
   STA   __zpabi_leaf_call__t_i_6
   LDA   #$00
   STA   __zpabi_leaf_call__c_1
   JSR   leaf_call
   STA   __local_pass_and_return_regs__1
   STX   __local_pass_and_return_regs__0
   ORA   __local_pass_and_return_regs__0
   BEQ   .lb_skip@3
   JMP   .if_end@11
.lb_skip@3:
   LDA   #$01
   STA   __local_pass_and_return_regs__retval+15
   LDA   #$00
   STA   __local_pass_and_return_regs__retval+16
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_pass_and_return_regs__retval
   LDY   #$00
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+1
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+2
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+3
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+4
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+5
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+6
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+7
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+8
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+9
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+10
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+11
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+12
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+13
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+14
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+15
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+16
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2A
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
   RTS
.if_end@11:
   LDY   #$06
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .pass_and_return_regs@jthread@3
   LDY   #$0E
   LDA   (FP),Y
   CMP   #$40
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   LDY   #$19
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   CMP   #$05
   BNE   .pass_and_return_regs@jthread@3
   LDY   #$24
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   CMP   #$4D
   BNE   .pass_and_return_regs@jthread@3
   LDY   #$2A
   LDA   (FP),Y
   BNE   .pass_and_return_regs@jthread@3
   DEY
   LDA   (FP),Y
   CMP   #$63
   BNE   .lb_skip@2
   JMP   .if_end@20
.lb_skip@2:
.pass_and_return_regs@jthread@3:
   LDA   #$02
   STA   __local_pass_and_return_regs__retval+15
   LDA   #$00
   STA   __local_pass_and_return_regs__retval+16
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_pass_and_return_regs__retval
   LDY   #$00
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+1
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+2
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+3
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+4
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+5
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+6
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+7
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+8
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+9
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+10
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+11
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+12
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+13
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+14
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+15
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+16
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2A
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
   RTS
.if_end@20:
   CLC
   LDA   FP
   ADC   #$25
   STA   __local_pass_and_return_regs__4
   LDA   FP+1
   ADC   #$00
   STA   __local_pass_and_return_regs__4+1
   LDA   __local_pass_and_return_regs__4
   STA   DPTR
   LDA   __local_pass_and_return_regs__5
   STA   DPTR+1
   LDY   #$00
   LDA   (__local_pass_and_return_regs__4),Y
   STA   __local_pass_and_return_regs__3
   INY
   LDA   (__local_pass_and_return_regs__4),Y
   STA   __local_pass_and_return_regs__2
   INY
   LDA   (__local_pass_and_return_regs__4),Y
   STA   __local_pass_and_return_regs__1
   INY
   LDA   (__local_pass_and_return_regs__4),Y
   STA   __local_pass_and_return_regs__0
   BNE   .jcmp_skip@0
   LDA   __local_pass_and_return_regs__1
   CMP   #$08
   BNE   .jcmp_skip@0
   LDA   __local_pass_and_return_regs__2
   CMP   #$AA
   BNE   .jcmp_skip@0
   LDA   __local_pass_and_return_regs__3
   CMP   #$52
   BNE   .lb_skip@1
   JMP   .if_end@21
.lb_skip@1:
.jcmp_skip@0:
   LDA   #$03
   STA   __local_pass_and_return_regs__retval+15
   LDA   #$00
   STA   __local_pass_and_return_regs__retval+16
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_pass_and_return_regs__retval
   LDY   #$00
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+1
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+2
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+3
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+4
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+5
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+6
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+7
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+8
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+9
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+10
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+11
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+12
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+13
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+14
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+15
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+16
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2A
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
   RTS
.if_end@21:
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDX   #>__local_pass_and_return_regs__stackbytes
   LDA   #<__local_pass_and_return_regs__stackbytes
   JSR   strcmp
   STA   __local_pass_and_return_regs__1
   STX   __local_pass_and_return_regs__0
   ORA   __local_pass_and_return_regs__0
   BNE   .lb_skip@0
   JMP   .if_end@22
.lb_skip@0:
   LDA   #$04
   STA   __local_pass_and_return_regs__retval+15
   LDA   #$00
   STA   __local_pass_and_return_regs__retval+16
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_pass_and_return_regs__retval
   LDY   #$00
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+1
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+2
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+3
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+4
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+5
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+6
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+7
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+8
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+9
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+10
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+11
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+12
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+13
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+14
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+15
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+16
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2A
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
   RTS
.if_end@22:
   LDA   #$64
   STA   __local_pass_and_return_regs__retval+11
   LDA   #$00
   STA   __local_pass_and_return_regs__retval+12
   LDY   #$00
   STY   __local_pass_and_return_regs__retval+13
   STA   __local_pass_and_return_regs__retval+14
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_pass_and_return_regs__retval
   LDY   #$00
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+1
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+2
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+3
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+4
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+5
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+6
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+7
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+8
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+9
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+10
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+11
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+12
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+13
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+14
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+15
   INY
   STA   (DPTR),Y
   LDA   __local_pass_and_return_regs__retval+16
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2A
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
   RTS

str@0:
   DC.B  $7A, $79, $78, $77, $76, $75, $74, $00
