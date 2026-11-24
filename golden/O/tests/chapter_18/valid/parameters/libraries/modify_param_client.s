SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_modify_nested_struct__s_0	EQU	$80
__zpabi_modify_simple_struct__s_0	EQU	$80
__zpabi_modify_nested_struct__s_1	EQU	$81
__zpabi_modify_simple_struct__s_1	EQU	$81
__zpabi_modify_nested_struct__s_2	EQU	$82
__zpabi_modify_simple_struct__s_2	EQU	$82
__zpabi_modify_nested_struct__s_3	EQU	$83
__zpabi_modify_simple_struct__s_3	EQU	$83
__zpabi_modify_nested_struct__s_4	EQU	$84
__zpabi_modify_simple_struct__s_4	EQU	$84
__zpabi_modify_nested_struct__s_5	EQU	$85
__zpabi_modify_simple_struct__s_5	EQU	$85
__zpabi_modify_nested_struct__s_6	EQU	$86
__zpabi_modify_simple_struct__s_6	EQU	$86
__zpabi_modify_nested_struct__s_7	EQU	$87
__zpabi_modify_simple_struct__s_7	EQU	$87
__zpabi_modify_nested_struct__s_8	EQU	$88
__zpabi_modify_simple_struct__s_8	EQU	$88
__zpabi_modify_nested_struct__s_9	EQU	$89
__zpabi_modify_simple_struct__s_9	EQU	$89
__zpabi_modify_nested_struct__s_10	EQU	$8A
__zpabi_modify_nested_struct__s_11	EQU	$8B
__zpabi_modify_nested_struct__s_12	EQU	$8C
__zpabi_modify_nested_struct__s_13	EQU	$8D
__zpabi_modify_nested_struct__s_14	EQU	$8E
__zpabi_modify_nested_struct__s_15	EQU	$8F
__local_main__0	EQU	$90
__local_main__1	EQU	$91
__local_main__s_o_15	EQU	$92
__local_main__s_o_14	EQU	$93
__local_main__s_o_13	EQU	$94
__local_main__s_o_12	EQU	$95
__local_main__s_o_9	EQU	$96
__local_main__s_o_8	EQU	$97
__local_main__s_o_10	EQU	$98
__local_main__s_o_11	EQU	$99
__local_main__s_o_7	EQU	$9A
__local_main__s_o_6	EQU	$9B
__local_main__s_o_5	EQU	$9C
__local_main__s_o_4	EQU	$9D
__local_main__s_o_3	EQU	$9E
__local_main__s_o_2	EQU	$9F
__local_main__s_o_1	EQU	$A0
__local_main__s_o_0	EQU	$A1
__local_main__s_inner	EQU	$A2

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__s_o_15,__local_main__s_o_14,__local_main__s_o_13,__local_main__s_o_12,__local_main__s_o_9,__local_main__s_o_8,__local_main__s_o_10,__local_main__s_o_11,__local_main__s_o_7,__local_main__s_o_6,__local_main__s_o_5,__local_main__s_o_4,__local_main__s_o_3,__local_main__s_o_2,__local_main__s_o_1,__local_main__s_o_0,__local_main__s_inner,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17 indirect=false in_cycle=false
; ext modify_nested_struct params=__zpabi_modify_nested_struct__s_0,__zpabi_modify_nested_struct__s_1,__zpabi_modify_nested_struct__s_2,__zpabi_modify_nested_struct__s_3,__zpabi_modify_nested_struct__s_4,__zpabi_modify_nested_struct__s_5,__zpabi_modify_nested_struct__s_6,__zpabi_modify_nested_struct__s_7,__zpabi_modify_nested_struct__s_8,__zpabi_modify_nested_struct__s_9,__zpabi_modify_nested_struct__s_10,__zpabi_modify_nested_struct__s_11,__zpabi_modify_nested_struct__s_12,__zpabi_modify_nested_struct__s_13,__zpabi_modify_nested_struct__s_14,__zpabi_modify_nested_struct__s_15
; ext modify_simple_struct params=__zpabi_modify_simple_struct__s_0,__zpabi_modify_simple_struct__s_1,__zpabi_modify_simple_struct__s_2,__zpabi_modify_simple_struct__s_3,__zpabi_modify_simple_struct__s_4,__zpabi_modify_simple_struct__s_5,__zpabi_modify_simple_struct__s_6,__zpabi_modify_simple_struct__s_7,__zpabi_modify_simple_struct__s_8,__zpabi_modify_simple_struct__s_9
; call main -> modify_nested_struct
; call main -> modify_simple_struct
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__s_inner
   LDY   #$00
   STY   __local_main__s_inner+1
   STY   __local_main__s_inner+2
   STY   __local_main__s_inner+3
   STY   __local_main__s_inner+4
   STY   __local_main__s_inner+5
   STA   __local_main__s_inner+6
   LDA   #$40
   STA   __local_main__s_inner+7
   LDA   #$03
   STA   __local_main__s_inner+8
   TYA
   STA   __local_main__s_inner+9
   STY   __zpabi_modify_simple_struct__s_0
   STY   __zpabi_modify_simple_struct__s_1
   STY   __zpabi_modify_simple_struct__s_2
   STY   __zpabi_modify_simple_struct__s_3
   STY   __zpabi_modify_simple_struct__s_4
   STY   __zpabi_modify_simple_struct__s_5
   STA   __zpabi_modify_simple_struct__s_6
   LDA   #$40
   STA   __zpabi_modify_simple_struct__s_7
   LDA   #$03
   STA   __zpabi_modify_simple_struct__s_8
   STY   __zpabi_modify_simple_struct__s_9
   JSR   modify_simple_struct
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_main__s_inner
   STA   __local_main__s_o_8
   LDA   __local_main__s_inner+1
   STA   __local_main__s_o_9
   LDA   __local_main__s_inner+2
   STA   __local_main__s_o_12
   LDA   __local_main__s_inner+3
   STA   __local_main__s_o_13
   LDA   __local_main__s_inner+4
   STA   __local_main__s_o_14
   LDA   __local_main__s_inner+5
   STA   __local_main__s_o_15
   LDA   __local_main__s_inner+6
   STA   __local_main__1
   LDA   __local_main__s_inner+7
   CMP   #$40
   BNE   .main@jthread@0
   LDA   __local_main__1
   BNE   .main@jthread@0
   LDA   __local_main__s_o_15
   BNE   .main@jthread@0
   LDA   __local_main__s_o_14
   BNE   .main@jthread@0
   LDA   __local_main__s_o_13
   BNE   .main@jthread@0
   LDA   __local_main__s_o_12
   BNE   .main@jthread@0
   LDA   __local_main__s_o_9
   BNE   .main@jthread@0
   LDA   __local_main__s_o_8
   BNE   .main@jthread@0
   LDA   __local_main__s_inner+8
   STA   __local_main__1
   LDA   __local_main__s_inner+9
   BNE   .main@jthread@0
   LDA   __local_main__1
   CMP   #$03
   BEQ   .if_end@3
.main@jthread@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   LDA   #$00
   STA   __local_main__s_o_0
   LDY   #$00
   STY   __local_main__s_o_1
   STY   __local_main__s_o_2
   STY   __local_main__s_o_3
   STY   __local_main__s_o_4
   STA   __local_main__s_o_5
   LDA   #$10
   STA   __local_main__s_o_6
   LDA   #$40
   STA   __local_main__s_o_7
   LDA   #$05
   STA   __local_main__s_o_8
   STY   __local_main__s_o_9
   LDA   #<__local_main__s_inner
   STA   __local_main__s_o_10
   LDA   #>__local_main__s_inner
   STA   __local_main__s_o_11
   LDA   #$E8
   STA   __local_main__s_o_12
   LDA   #$03
   STA   __local_main__s_o_13
   TYA
   STA   __local_main__s_o_14
   STY   __local_main__s_o_15
   STY   __zpabi_modify_nested_struct__s_0
   STY   __zpabi_modify_nested_struct__s_1
   STY   __zpabi_modify_nested_struct__s_2
   STY   __zpabi_modify_nested_struct__s_3
   STY   __zpabi_modify_nested_struct__s_4
   STA   __zpabi_modify_nested_struct__s_5
   LDA   #$10
   STA   __zpabi_modify_nested_struct__s_6
   LDA   #$40
   STA   __zpabi_modify_nested_struct__s_7
   LDA   #$05
   STA   __zpabi_modify_nested_struct__s_8
   STY   __zpabi_modify_nested_struct__s_9
   LDA   #<__local_main__s_inner
   STA   __zpabi_modify_nested_struct__s_10
   LDA   #>__local_main__s_inner
   STA   __zpabi_modify_nested_struct__s_11
   LDA   #$E8
   STA   __zpabi_modify_nested_struct__s_12
   LDA   #$03
   STA   __zpabi_modify_nested_struct__s_13
   TYA
   STA   __zpabi_modify_nested_struct__s_14
   STA   __zpabi_modify_nested_struct__s_15
   JSR   modify_nested_struct
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$03
   RTS
.if_end@4:
   LDA   __local_main__s_o_7
   CMP   #$40
   BNE   .main@jthread@2
   LDA   __local_main__s_o_6
   CMP   #$10
   BNE   .main@jthread@2
   LDA   __local_main__s_o_5
   BNE   .main@jthread@2
   LDA   __local_main__s_o_4
   BNE   .main@jthread@2
   LDA   __local_main__s_o_3
   BNE   .main@jthread@2
   LDA   __local_main__s_o_2
   BNE   .main@jthread@2
   LDA   __local_main__s_o_1
   BNE   .main@jthread@2
   LDA   __local_main__s_o_0
   BNE   .main@jthread@2
   LDA   __local_main__s_o_9
   BNE   .main@jthread@2
   LDA   __local_main__s_o_8
   CMP   #$05
   BNE   .main@jthread@2
   LDA   __local_main__s_o_15
   BNE   .main@jthread@2
   LDA   __local_main__s_o_14
   BNE   .main@jthread@2
   LDA   __local_main__s_o_13
   CMP   #$03
   BNE   .main@jthread@2
   LDA   __local_main__s_o_12
   CMP   #$E8
   BEQ   .if_end@9
.main@jthread@2:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@9:
   LDA   __local_main__s_o_11
   CMP   #>__local_main__s_inner
   BNE   .jcmp_skip@0
   LDA   __local_main__s_o_10
   CMP   #<__local_main__s_inner
   BEQ   .if_end@10
.jcmp_skip@0:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@10:
   LDY   #$00
   LDA   (__local_main__s_o_10),Y
   STA   __local_main__s_o_8
   INY
   LDA   (__local_main__s_o_10),Y
   STA   __local_main__s_o_9
   INY
   LDA   (__local_main__s_o_10),Y
   STA   __local_main__s_o_12
   INY
   LDA   (__local_main__s_o_10),Y
   STA   __local_main__s_o_13
   INY
   LDA   (__local_main__s_o_10),Y
   STA   __local_main__s_o_14
   INY
   LDA   (__local_main__s_o_10),Y
   STA   __local_main__s_o_15
   INY
   LDA   (__local_main__s_o_10),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__s_o_10),Y
   CMP   #$40
   BNE   .main@jthread@3
   LDA   __local_main__1
   CMP   #$24
   BNE   .main@jthread@3
   LDA   __local_main__s_o_15
   BNE   .main@jthread@3
   LDA   __local_main__s_o_14
   BNE   .main@jthread@3
   LDA   __local_main__s_o_13
   BNE   .main@jthread@3
   LDA   __local_main__s_o_12
   BNE   .main@jthread@3
   LDA   __local_main__s_o_9
   BNE   .main@jthread@3
   LDA   __local_main__s_o_8
   BNE   .main@jthread@3
   INY
   LDA   (__local_main__s_o_10),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__s_o_10),Y
   BNE   .main@jthread@3
   LDA   __local_main__1
   CMP   #$0B
   BEQ   .if_end@13
.main@jthread@3:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@13:
   LDX   #$00
   TXA
   RTS
