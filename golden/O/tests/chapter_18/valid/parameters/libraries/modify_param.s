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
__local_modify_simple_struct__1	EQU	$8B
__zpabi_modify_nested_struct__s_11	EQU	$8B
__zpabi_modify_nested_struct__s_12	EQU	$8C
__zpabi_modify_nested_struct__s_13	EQU	$8D
__zpabi_modify_nested_struct__s_14	EQU	$8E
__zpabi_modify_nested_struct__s_15	EQU	$8F
__local_modify_nested_struct__0	EQU	$90
__local_modify_nested_struct__1	EQU	$91
__local_modify_nested_struct__2	EQU	$92
__local_modify_nested_struct__3	EQU	$93
__local_modify_nested_struct__4	EQU	$94
__local_modify_nested_struct__5	EQU	$95
__local_modify_nested_struct__6	EQU	$96
__local_modify_nested_struct__7	EQU	$97
__local_modify_nested_struct__copy_10	EQU	$98
__local_modify_nested_struct__copy_11	EQU	$99
__local_modify_nested_struct__8	EQU	$9A
__local_modify_nested_struct__9	EQU	$9B
__local_modify_simple_struct__copy_0	EQU	$9D
__local_modify_nested_struct__copy_0	EQU	$A9

; @zp-link-meta-begin
; def modify_nested_struct params=__zpabi_modify_nested_struct__s_0,__zpabi_modify_nested_struct__s_1,__zpabi_modify_nested_struct__s_2,__zpabi_modify_nested_struct__s_3,__zpabi_modify_nested_struct__s_4,__zpabi_modify_nested_struct__s_5,__zpabi_modify_nested_struct__s_6,__zpabi_modify_nested_struct__s_7,__zpabi_modify_nested_struct__s_8,__zpabi_modify_nested_struct__s_9,__zpabi_modify_nested_struct__s_10,__zpabi_modify_nested_struct__s_11,__zpabi_modify_nested_struct__s_12,__zpabi_modify_nested_struct__s_13,__zpabi_modify_nested_struct__s_14,__zpabi_modify_nested_struct__s_15 locals=__local_modify_nested_struct__0,__local_modify_nested_struct__1,__local_modify_nested_struct__2,__local_modify_nested_struct__3,__local_modify_nested_struct__4,__local_modify_nested_struct__5,__local_modify_nested_struct__6,__local_modify_nested_struct__7,__local_modify_nested_struct__copy_10,__local_modify_nested_struct__copy_11,__local_modify_nested_struct__8,__local_modify_nested_struct__9,__local_modify_nested_struct__copy_15,__local_modify_nested_struct__copy_14,__local_modify_nested_struct__copy_13,__local_modify_nested_struct__copy_12,__local_modify_nested_struct__copy_9,__local_modify_nested_struct__copy_8,__local_modify_nested_struct__copy_7,__local_modify_nested_struct__copy_6,__local_modify_nested_struct__copy_5,__local_modify_nested_struct__copy_4,__local_modify_nested_struct__copy_3,__local_modify_nested_struct__copy_2,__local_modify_nested_struct__copy_1,__local_modify_nested_struct__copy_0 indirect=false in_cycle=false changes=__zpabi_modify_nested_struct__s_0,__zpabi_modify_nested_struct__s_1,__zpabi_modify_nested_struct__s_2,__zpabi_modify_nested_struct__s_3,__zpabi_modify_nested_struct__s_4,__zpabi_modify_nested_struct__s_5,__zpabi_modify_nested_struct__s_6,__zpabi_modify_nested_struct__s_7,__zpabi_modify_nested_struct__s_8,__zpabi_modify_nested_struct__s_9,__zpabi_modify_nested_struct__s_10,__zpabi_modify_nested_struct__s_11,__zpabi_modify_nested_struct__s_12,__zpabi_modify_nested_struct__s_13,__zpabi_modify_nested_struct__s_14,__zpabi_modify_nested_struct__s_15
; def modify_simple_struct params=__zpabi_modify_simple_struct__s_0,__zpabi_modify_simple_struct__s_1,__zpabi_modify_simple_struct__s_2,__zpabi_modify_simple_struct__s_3,__zpabi_modify_simple_struct__s_4,__zpabi_modify_simple_struct__s_5,__zpabi_modify_simple_struct__s_6,__zpabi_modify_simple_struct__s_7,__zpabi_modify_simple_struct__s_8,__zpabi_modify_simple_struct__s_9 locals=__local_modify_simple_struct__0,__local_modify_simple_struct__1,__local_modify_simple_struct__2,__local_modify_simple_struct__3,__local_modify_simple_struct__4,__local_modify_simple_struct__5,__local_modify_simple_struct__6,__local_modify_simple_struct__7,__local_modify_simple_struct__8,__local_modify_simple_struct__9,__local_modify_simple_struct__copy_9,__local_modify_simple_struct__copy_8,__local_modify_simple_struct__copy_7,__local_modify_simple_struct__copy_6,__local_modify_simple_struct__copy_5,__local_modify_simple_struct__copy_4,__local_modify_simple_struct__copy_3,__local_modify_simple_struct__copy_2,__local_modify_simple_struct__copy_1,__local_modify_simple_struct__copy_0 indirect=false in_cycle=false changes=__zpabi_modify_simple_struct__s_0,__zpabi_modify_simple_struct__s_1,__zpabi_modify_simple_struct__s_2,__zpabi_modify_simple_struct__s_3,__zpabi_modify_simple_struct__s_4,__zpabi_modify_simple_struct__s_5,__zpabi_modify_simple_struct__s_6,__zpabi_modify_simple_struct__s_7,__zpabi_modify_simple_struct__s_8,__zpabi_modify_simple_struct__s_9
; item modify_simple_struct section=.text local=false zero=false
; item modify_nested_struct section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

modify_simple_struct:
   SUBROUTINE

   LDA   __zpabi_modify_simple_struct__s_0
   STA   __local_modify_simple_struct__copy_0
   LDY   #$00
   BNE   .modify_simple_struct@jthread@1
   LDA   __zpabi_modify_simple_struct__s_0+8
   STA   __local_modify_simple_struct__1
   LDA   __zpabi_modify_simple_struct__s_0+9
   BNE   .modify_simple_struct@jthread@1
   LDA   __local_modify_simple_struct__1
   CMP   #$03
   BNE   .modify_simple_struct@jthread@1
   LDA   __zpabi_modify_simple_struct__s_7
   CMP   #$40
   BNE   .modify_simple_struct@jthread@1
   LDA   __zpabi_modify_simple_struct__s_6
   BNE   .modify_simple_struct@jthread@1
   LDA   __zpabi_modify_simple_struct__s_5
   BNE   .modify_simple_struct@jthread@1
   LDA   __zpabi_modify_simple_struct__s_4
   BNE   .modify_simple_struct@jthread@1
   LDA   __zpabi_modify_simple_struct__s_3
   BNE   .modify_simple_struct@jthread@1
   LDA   __zpabi_modify_simple_struct__s_2
   BNE   .modify_simple_struct@jthread@1
   LDA   __zpabi_modify_simple_struct__s_1
   BNE   .modify_simple_struct@jthread@1
   LDA   __local_modify_simple_struct__copy_0
   BNE   .modify_simple_struct@jthread@1
   LDA   __zpabi_modify_simple_struct__s_9
   BNE   .modify_simple_struct@jthread@1
   LDA   __zpabi_modify_simple_struct__s_8
   CMP   #$03
   BEQ   .if_end@5
.modify_simple_struct@jthread@1:
   LDX   #$00
   TXA
   RTS
.if_end@5:
   LDX   #$00
   LDA   #$01
   RTS

modify_nested_struct:
   SUBROUTINE

   LDA   __zpabi_modify_nested_struct__s_0
   STA   __local_modify_nested_struct__copy_0
   LDA   __zpabi_modify_nested_struct__s_10
   STA   __local_modify_nested_struct__copy_10
   LDA   __zpabi_modify_nested_struct__s_11
   STA   __local_modify_nested_struct__copy_11
   LDA   #$0A
   STA   __zpabi_modify_nested_struct__s_0+12
   LDA   #$00
   STA   __zpabi_modify_nested_struct__s_0+13
   LDY   #$00
   STY   __zpabi_modify_nested_struct__s_0+14
   STA   __zpabi_modify_nested_struct__s_0+15
   LDY   #$C8
   STY   __zpabi_modify_nested_struct__s_0+8
   STA   __zpabi_modify_nested_struct__s_0+9
   LDA   __zpabi_modify_nested_struct__s_0+10
   STA   __local_modify_nested_struct__0
   LDA   __zpabi_modify_nested_struct__s_0+11
   STA   __local_modify_nested_struct__1
   LDA   #$00
   LDY   #$00
   STA   (__local_modify_nested_struct__0),Y
   INY
   STA   (__local_modify_nested_struct__0),Y
   INY
   STA   (__local_modify_nested_struct__0),Y
   INY
   STA   (__local_modify_nested_struct__0),Y
   INY
   STA   (__local_modify_nested_struct__0),Y
   INY
   STA   (__local_modify_nested_struct__0),Y
   LDA   #$24
   INY
   STA   (__local_modify_nested_struct__0),Y
   LDA   #$40
   INY
   STA   (__local_modify_nested_struct__0),Y
   LDA   #$0B
   INY
   STA   (__local_modify_nested_struct__0),Y
   LDA   #$00
   INY
   STA   (__local_modify_nested_struct__0),Y
   LDA   __zpabi_modify_nested_struct__s_0+8
   STA   __local_modify_nested_struct__1
   LDA   __zpabi_modify_nested_struct__s_0+9
   BEQ   .lb_skip@25
   JMP   .modify_nested_struct@jthread@3
.lb_skip@25:
   LDA   __local_modify_nested_struct__1
   CMP   #$C8
   BEQ   .lb_skip@24
   JMP   .modify_nested_struct@jthread@3
.lb_skip@24:
   LDA   __zpabi_modify_nested_struct__s_0
   STA   __local_modify_nested_struct__7
   LDA   __zpabi_modify_nested_struct__s_0+1
   STA   __local_modify_nested_struct__6
   LDA   __zpabi_modify_nested_struct__s_0+2
   STA   __local_modify_nested_struct__5
   LDA   __zpabi_modify_nested_struct__s_0+3
   STA   __local_modify_nested_struct__4
   LDA   __zpabi_modify_nested_struct__s_0+4
   STA   __local_modify_nested_struct__3
   LDA   __zpabi_modify_nested_struct__s_0+5
   STA   __local_modify_nested_struct__2
   LDA   __zpabi_modify_nested_struct__s_0+6
   STA   __local_modify_nested_struct__1
   LDA   __zpabi_modify_nested_struct__s_0+7
   CMP   #$40
   BEQ   .lb_skip@23
   JMP   .modify_nested_struct@jthread@3
.lb_skip@23:
   LDA   __local_modify_nested_struct__1
   CMP   #$10
   BEQ   .lb_skip@22
   JMP   .modify_nested_struct@jthread@3
.lb_skip@22:
   LDA   __local_modify_nested_struct__2
   BEQ   .lb_skip@21
   JMP   .modify_nested_struct@jthread@3
.lb_skip@21:
   LDA   __local_modify_nested_struct__3
   BEQ   .lb_skip@20
   JMP   .modify_nested_struct@jthread@3
.lb_skip@20:
   LDA   __local_modify_nested_struct__4
   BEQ   .lb_skip@19
   JMP   .modify_nested_struct@jthread@3
.lb_skip@19:
   LDA   __local_modify_nested_struct__5
   BEQ   .lb_skip@18
   JMP   .modify_nested_struct@jthread@3
.lb_skip@18:
   LDA   __local_modify_nested_struct__6
   BEQ   .lb_skip@17
   JMP   .modify_nested_struct@jthread@3
.lb_skip@17:
   LDA   __local_modify_nested_struct__7
   BEQ   .lb_skip@16
   JMP   .modify_nested_struct@jthread@3
.lb_skip@16:
   LDA   __zpabi_modify_nested_struct__s_0+12
   STA   __local_modify_nested_struct__3
   LDA   __zpabi_modify_nested_struct__s_0+13
   STA   __local_modify_nested_struct__2
   LDA   __zpabi_modify_nested_struct__s_0+14
   STA   __local_modify_nested_struct__1
   LDA   __zpabi_modify_nested_struct__s_0+15
   BEQ   .lb_skip@15
   JMP   .modify_nested_struct@jthread@3
.lb_skip@15:
   LDA   __local_modify_nested_struct__1
   BEQ   .lb_skip@14
   JMP   .modify_nested_struct@jthread@3
.lb_skip@14:
   LDA   __local_modify_nested_struct__2
   BEQ   .lb_skip@13
   JMP   .modify_nested_struct@jthread@3
.lb_skip@13:
   LDA   __local_modify_nested_struct__3
   CMP   #$0A
   BEQ   .lb_skip@12
   JMP   .modify_nested_struct@jthread@3
.lb_skip@12:
   LDA   __zpabi_modify_nested_struct__s_0+10
   STA   __local_modify_nested_struct__8
   LDA   __zpabi_modify_nested_struct__s_0+11
   STA   __local_modify_nested_struct__9
   LDY   #$00
   LDA   (__local_modify_nested_struct__8),Y
   STA   __local_modify_nested_struct__7
   INY
   LDA   (__local_modify_nested_struct__8),Y
   STA   __local_modify_nested_struct__6
   INY
   LDA   (__local_modify_nested_struct__8),Y
   STA   __local_modify_nested_struct__5
   INY
   LDA   (__local_modify_nested_struct__8),Y
   STA   __local_modify_nested_struct__4
   INY
   LDA   (__local_modify_nested_struct__8),Y
   STA   __local_modify_nested_struct__3
   INY
   LDA   (__local_modify_nested_struct__8),Y
   STA   __local_modify_nested_struct__2
   INY
   LDA   (__local_modify_nested_struct__8),Y
   STA   __local_modify_nested_struct__1
   INY
   LDA   (__local_modify_nested_struct__8),Y
   CMP   #$40
   BNE   .modify_nested_struct@jthread@3
   LDA   __local_modify_nested_struct__1
   CMP   #$24
   BNE   .modify_nested_struct@jthread@3
   LDA   __local_modify_nested_struct__2
   BNE   .modify_nested_struct@jthread@3
   LDA   __local_modify_nested_struct__3
   BNE   .modify_nested_struct@jthread@3
   LDA   __local_modify_nested_struct__4
   BNE   .modify_nested_struct@jthread@3
   LDA   __local_modify_nested_struct__5
   BNE   .modify_nested_struct@jthread@3
   LDA   __local_modify_nested_struct__6
   BNE   .modify_nested_struct@jthread@3
   LDA   __local_modify_nested_struct__7
   BNE   .modify_nested_struct@jthread@3
   LDA   __zpabi_modify_nested_struct__s_0+10
   STA   __local_modify_nested_struct__2
   LDA   __zpabi_modify_nested_struct__s_0+11
   STA   __local_modify_nested_struct__3
   INY
   LDA   (__local_modify_nested_struct__2),Y
   STA   __local_modify_nested_struct__1
   INY
   LDA   (__local_modify_nested_struct__2),Y
   BNE   .modify_nested_struct@jthread@3
   LDA   __local_modify_nested_struct__1
   CMP   #$0B
   BEQ   .if_end@14
.modify_nested_struct@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@14:
   LDA   __zpabi_modify_nested_struct__s_9
   BEQ   .lb_skip@11
   JMP   .modify_nested_struct@jthread@7
.lb_skip@11:
   LDA   __zpabi_modify_nested_struct__s_8
   CMP   #$05
   BEQ   .lb_skip@10
   JMP   .modify_nested_struct@jthread@7
.lb_skip@10:
   LDA   __zpabi_modify_nested_struct__s_7
   CMP   #$40
   BEQ   .lb_skip@9
   JMP   .modify_nested_struct@jthread@7
.lb_skip@9:
   LDA   __zpabi_modify_nested_struct__s_6
   CMP   #$10
   BEQ   .lb_skip@8
   JMP   .modify_nested_struct@jthread@7
.lb_skip@8:
   LDA   __zpabi_modify_nested_struct__s_5
   BEQ   .lb_skip@7
   JMP   .modify_nested_struct@jthread@7
.lb_skip@7:
   LDA   __zpabi_modify_nested_struct__s_4
   BEQ   .lb_skip@6
   JMP   .modify_nested_struct@jthread@7
.lb_skip@6:
   LDA   __zpabi_modify_nested_struct__s_3
   BEQ   .lb_skip@5
   JMP   .modify_nested_struct@jthread@7
.lb_skip@5:
   LDA   __zpabi_modify_nested_struct__s_2
   BEQ   .lb_skip@4
   JMP   .modify_nested_struct@jthread@7
.lb_skip@4:
   LDA   __zpabi_modify_nested_struct__s_1
   BEQ   .lb_skip@3
   JMP   .modify_nested_struct@jthread@7
.lb_skip@3:
   LDA   __local_modify_nested_struct__copy_0
   BEQ   .lb_skip@2
   JMP   .modify_nested_struct@jthread@7
.lb_skip@2:
   LDA   __zpabi_modify_nested_struct__s_15
   BEQ   .lb_skip@1
   JMP   .modify_nested_struct@jthread@7
.lb_skip@1:
   LDA   __zpabi_modify_nested_struct__s_14
   BEQ   .lb_skip@0
   JMP   .modify_nested_struct@jthread@7
.lb_skip@0:
   LDA   __zpabi_modify_nested_struct__s_13
   CMP   #$03
   BNE   .modify_nested_struct@jthread@7
   LDA   __zpabi_modify_nested_struct__s_12
   CMP   #$E8
   BNE   .modify_nested_struct@jthread@7
   LDY   #$00
   LDA   (__local_modify_nested_struct__copy_10),Y
   STA   __local_modify_nested_struct__7
   INY
   LDA   (__local_modify_nested_struct__copy_10),Y
   STA   __local_modify_nested_struct__6
   INY
   LDA   (__local_modify_nested_struct__copy_10),Y
   STA   __local_modify_nested_struct__5
   INY
   LDA   (__local_modify_nested_struct__copy_10),Y
   STA   __local_modify_nested_struct__4
   INY
   LDA   (__local_modify_nested_struct__copy_10),Y
   STA   __local_modify_nested_struct__3
   INY
   LDA   (__local_modify_nested_struct__copy_10),Y
   STA   __local_modify_nested_struct__2
   INY
   LDA   (__local_modify_nested_struct__copy_10),Y
   STA   __local_modify_nested_struct__1
   INY
   LDA   (__local_modify_nested_struct__copy_10),Y
   CMP   #$40
   BNE   .modify_nested_struct@jthread@7
   LDA   __local_modify_nested_struct__1
   CMP   #$24
   BNE   .modify_nested_struct@jthread@7
   LDA   __local_modify_nested_struct__2
   BNE   .modify_nested_struct@jthread@7
   LDA   __local_modify_nested_struct__3
   BNE   .modify_nested_struct@jthread@7
   LDA   __local_modify_nested_struct__4
   BNE   .modify_nested_struct@jthread@7
   LDA   __local_modify_nested_struct__5
   BNE   .modify_nested_struct@jthread@7
   LDA   __local_modify_nested_struct__6
   BNE   .modify_nested_struct@jthread@7
   LDA   __local_modify_nested_struct__7
   BNE   .modify_nested_struct@jthread@7
   INY
   LDA   (__local_modify_nested_struct__copy_10),Y
   STA   __local_modify_nested_struct__1
   INY
   LDA   (__local_modify_nested_struct__copy_10),Y
   BNE   .modify_nested_struct@jthread@7
   LDA   __local_modify_nested_struct__1
   CMP   #$0B
   BEQ   .if_end@23
.modify_nested_struct@jthread@7:
   LDX   #$00
   TXA
   RTS
.if_end@23:
   LDX   #$00
   LDA   #$01
   RTS
