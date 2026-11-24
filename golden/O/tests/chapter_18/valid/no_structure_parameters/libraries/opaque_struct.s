SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_increment_struct__0	EQU	$80
__zpabi_check_struct__expected_i_0	EQU	$80
__zpabi_create_struct__d_0	EQU	$80
__local_increment_struct__1	EQU	$81
__local_print_struct_msg__ptr	EQU	$81
__zpabi_check_struct__expected_i_1	EQU	$81
__zpabi_create_struct__d_1	EQU	$81
__local_increment_struct__2	EQU	$82
__local_print_struct_msg__1	EQU	$82
__zpabi_check_struct__expected_d_0	EQU	$82
__zpabi_create_struct__d_2	EQU	$82
__local_increment_struct__3	EQU	$83
__zpabi_check_struct__expected_d_1	EQU	$83
__zpabi_create_struct__d_3	EQU	$83
__local_increment_struct__4	EQU	$84
__zpabi_check_struct__expected_d_2	EQU	$84
__zpabi_create_struct__d_4	EQU	$84
__local_increment_struct__5	EQU	$85
__zpabi_check_struct__expected_d_3	EQU	$85
__zpabi_create_struct__d_5	EQU	$85
__local_increment_struct__6	EQU	$86
__zpabi_check_struct__expected_d_4	EQU	$86
__zpabi_create_struct__d_6	EQU	$86
__zpabi_check_struct__expected_d_5	EQU	$87
__zpabi_create_struct__d_7	EQU	$87
__local_increment_struct__ptr	EQU	$88
__zpabi_check_struct__expected_d_6	EQU	$88
__zpabi_create_struct__s_0	EQU	$88
__local_increment_struct__8	EQU	$89
__zpabi_check_struct__expected_d_7	EQU	$89
__zpabi_create_struct__s_1	EQU	$89
__zpabi_check_struct__expected_s_0	EQU	$8A
__zpabi_malloc__size_0	EQU	$8A
__zpabi_check_struct__expected_s_1	EQU	$8B
__zpabi_malloc__size_1	EQU	$8B
__zpabi_malloc__size_2	EQU	$8C
__zpabi_strcmp__s2_0	EQU	$8C
__zpabi_malloc__size_3	EQU	$8D
__zpabi_strcmp__s2_1	EQU	$8D
__local_check_struct__0	EQU	$8E
__local_create_struct__0	EQU	$8E
__local_check_struct__1	EQU	$8F
__local_create_struct__1	EQU	$8F
__local_check_struct__2	EQU	$90
__local_create_struct__i_1	EQU	$90
__local_check_struct__3	EQU	$91
__local_create_struct__i_0	EQU	$91
__local_check_struct__4	EQU	$92
__local_check_struct__5	EQU	$93
__local_check_struct__6	EQU	$94
__local_check_struct__7	EQU	$95
__local_check_struct__ptr	EQU	$96
__local_check_struct__8	EQU	$97

; @zp-link-meta-begin
; def check_struct params=__zpabi_check_struct__ptr_0,__zpabi_check_struct__ptr_1,__zpabi_check_struct__expected_i_0,__zpabi_check_struct__expected_i_1,__zpabi_check_struct__expected_d_0,__zpabi_check_struct__expected_d_1,__zpabi_check_struct__expected_d_2,__zpabi_check_struct__expected_d_3,__zpabi_check_struct__expected_d_4,__zpabi_check_struct__expected_d_5,__zpabi_check_struct__expected_d_6,__zpabi_check_struct__expected_d_7,__zpabi_check_struct__expected_s_0,__zpabi_check_struct__expected_s_1 locals=__local_check_struct__0,__local_check_struct__1,__local_check_struct__2,__local_check_struct__3,__local_check_struct__4,__local_check_struct__5,__local_check_struct__6,__local_check_struct__7,__local_check_struct__ptr,__local_check_struct__8 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,
; def create_struct params=__zpabi_create_struct__i_0,__zpabi_create_struct__i_1,__zpabi_create_struct__d_0,__zpabi_create_struct__d_1,__zpabi_create_struct__d_2,__zpabi_create_struct__d_3,__zpabi_create_struct__d_4,__zpabi_create_struct__d_5,__zpabi_create_struct__d_6,__zpabi_create_struct__d_7,__zpabi_create_struct__s_0,__zpabi_create_struct__s_1 locals=__local_create_struct__0,__local_create_struct__1,__local_create_struct__i_1,__local_create_struct__i_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; def get_internal_struct params= locals= indirect=false in_cycle=false
; def increment_struct params=__zpabi_increment_struct__ptr_0,__zpabi_increment_struct__ptr_1 locals=__local_increment_struct__0,__local_increment_struct__1,__local_increment_struct__2,__local_increment_struct__3,__local_increment_struct__4,__local_increment_struct__5,__local_increment_struct__6,__local_increment_struct__7,__local_increment_struct__ptr,__local_increment_struct__8 indirect=false in_cycle=false param_regs=A,X
; def print_struct_msg params=__zpabi_print_struct_msg__ptr_0,__zpabi_print_struct_msg__ptr_1 locals=__local_print_struct_msg__0,__local_print_struct_msg__ptr,__local_print_struct_msg__1 indirect=false in_cycle=false param_regs=A,X
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext puts params=__zpabi_puts__s_0,__zpabi_puts__s_1 param_regs=A,X
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call check_struct -> strcmp
; call create_struct -> malloc
; call print_struct_msg -> puts
; item create_struct section=.text local=false zero=false
; item increment_struct section=.text local=false zero=false
; item check_struct section=.text local=false zero=false
; item print_struct_msg section=.text local=false zero=false
; item get_internal_struct section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item internal section=.data local=true zero=false
; item incomplete_var section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

create_struct:
   SUBROUTINE

   STA   __local_create_struct__i_0
   STX   __local_create_struct__i_1
   LDA   #$0C
   STA   __zpabi_malloc__size_0
   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JSR   malloc
   STA   __local_create_struct__0
   STX   __local_create_struct__1
   STA   DPTR
   STX   DPTR+1
   LDA   __local_create_struct__i_0
   LDY   #$00
   STA   (__local_create_struct__0),Y
   LDA   __local_create_struct__i_1
   INY
   STA   (DPTR),Y
   LDA   __local_create_struct__0
   STA   DPTR
   STX   DPTR+1
   LDA   __zpabi_create_struct__d_0
   INY
   STA   (__local_create_struct__0),Y
   LDA   __zpabi_create_struct__d_1
   INY
   STA   (DPTR),Y
   LDA   __zpabi_create_struct__d_2
   INY
   STA   (DPTR),Y
   LDA   __zpabi_create_struct__d_3
   INY
   STA   (DPTR),Y
   LDA   __zpabi_create_struct__d_4
   INY
   STA   (DPTR),Y
   LDA   __zpabi_create_struct__d_5
   INY
   STA   (DPTR),Y
   LDA   __zpabi_create_struct__d_6
   INY
   STA   (DPTR),Y
   LDA   __zpabi_create_struct__d_7
   INY
   STA   (DPTR),Y
   LDA   __local_create_struct__0
   STA   DPTR
   STX   DPTR+1
   LDA   __zpabi_create_struct__s_0
   INY
   STA   (__local_create_struct__0),Y
   LDA   __zpabi_create_struct__s_1
   INY
   STA   (DPTR),Y
   LDA   __local_create_struct__0
   RTS

increment_struct:
   SUBROUTINE

   STA   __local_increment_struct__ptr
   STX   __local_increment_struct__8
   LDY   #$00
   LDA   (__local_increment_struct__ptr),Y
   STA   __local_increment_struct__0
   INY
   LDA   (__local_increment_struct__ptr),Y
   STA   __local_increment_struct__1
   INC   __local_increment_struct__0
   BNE   .inc_done@0
   INC   __local_increment_struct__1
.inc_done@0:
   LDA   __local_increment_struct__0
   LDY   #$00
   STA   (__local_increment_struct__ptr),Y
   LDA   __local_increment_struct__1
   INY
   STA   (__local_increment_struct__ptr),Y
   INY
   LDA   (__local_increment_struct__ptr),Y
   STA   HARGS
   INY
   LDA   (__local_increment_struct__ptr),Y
   STA   HARGS+1
   INY
   LDA   (__local_increment_struct__ptr),Y
   STA   HARGS+2
   INY
   LDA   (__local_increment_struct__ptr),Y
   STA   HARGS+3
   INY
   LDA   (__local_increment_struct__ptr),Y
   STA   HARGS+4
   INY
   LDA   (__local_increment_struct__ptr),Y
   STA   HARGS+5
   INY
   LDA   (__local_increment_struct__ptr),Y
   STA   HARGS+6
   INY
   LDA   (__local_increment_struct__ptr),Y
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+17
   STA   __local_increment_struct__6
   LDA   HARGS+18
   STA   __local_increment_struct__5
   LDA   HARGS+19
   STA   __local_increment_struct__4
   LDA   HARGS+20
   STA   __local_increment_struct__3
   LDA   HARGS+21
   STA   __local_increment_struct__2
   LDA   HARGS+22
   STA   __local_increment_struct__1
   LDA   HARGS+23
   STA   __local_increment_struct__0
   LDA   HARGS+16
   LDY   #$02
   STA   (__local_increment_struct__ptr),Y
   LDA   __local_increment_struct__6
   INY
   STA   (__local_increment_struct__ptr),Y
   LDA   __local_increment_struct__5
   INY
   STA   (__local_increment_struct__ptr),Y
   LDA   __local_increment_struct__4
   INY
   STA   (__local_increment_struct__ptr),Y
   LDA   __local_increment_struct__3
   INY
   STA   (__local_increment_struct__ptr),Y
   LDA   __local_increment_struct__2
   INY
   STA   (__local_increment_struct__ptr),Y
   LDA   __local_increment_struct__1
   INY
   STA   (__local_increment_struct__ptr),Y
   LDA   __local_increment_struct__0
   INY
   STA   (__local_increment_struct__ptr),Y
   RTS

check_struct:
   SUBROUTINE

   STA   __local_check_struct__ptr
   STX   __local_check_struct__8
   LDY   #$00
   LDA   (__local_check_struct__ptr),Y
   STA   __local_check_struct__1
   INY
   LDA   (__local_check_struct__ptr),Y
   CMP   __zpabi_check_struct__expected_i_1
   BNE   .jcmp_skip@0
   LDA   __local_check_struct__1
   CMP   __zpabi_check_struct__expected_i_0
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   TXA
   RTS
.if_end@0:
   LDY   #$02
   LDA   (__local_check_struct__ptr),Y
   STA   __local_check_struct__7
   INY
   LDA   (__local_check_struct__ptr),Y
   STA   __local_check_struct__6
   INY
   LDA   (__local_check_struct__ptr),Y
   STA   __local_check_struct__5
   INY
   LDA   (__local_check_struct__ptr),Y
   STA   __local_check_struct__4
   INY
   LDA   (__local_check_struct__ptr),Y
   STA   __local_check_struct__3
   INY
   LDA   (__local_check_struct__ptr),Y
   STA   __local_check_struct__2
   INY
   LDA   (__local_check_struct__ptr),Y
   STA   __local_check_struct__1
   INY
   LDA   (__local_check_struct__ptr),Y
   CMP   __zpabi_check_struct__expected_d_7
   BNE   .tail@0
   LDA   __local_check_struct__1
   CMP   __zpabi_check_struct__expected_d_6
   BNE   .tail@0
   LDA   __local_check_struct__2
   CMP   __zpabi_check_struct__expected_d_5
   BNE   .tail@0
   LDA   __local_check_struct__3
   CMP   __zpabi_check_struct__expected_d_4
   BNE   .tail@0
   LDA   __local_check_struct__4
   CMP   __zpabi_check_struct__expected_d_3
   BNE   .tail@0
   LDA   __local_check_struct__5
   CMP   __zpabi_check_struct__expected_d_2
   BNE   .tail@0
   LDA   __local_check_struct__6
   CMP   __zpabi_check_struct__expected_d_1
   BNE   .tail@0
   LDA   __local_check_struct__7
   CMP   __zpabi_check_struct__expected_d_0
   BNE   .tail@0
   INY
   LDA   (__local_check_struct__ptr),Y
   STA   __local_check_struct__0
   INY
   LDA   (__local_check_struct__ptr),Y
   TAX
   LDA   __zpabi_check_struct__expected_s_0
   STA   __zpabi_strcmp__s2_0
   LDA   __zpabi_check_struct__expected_s_1
   STA   __zpabi_strcmp__s2_1
   LDA   __local_check_struct__0
   JSR   strcmp
   STX   __local_check_struct__0
   ORA   __local_check_struct__0
   BEQ   .if_end@2
.tail@0:
   LDX   #$00
   TXA
   RTS
.if_end@2:
   LDX   #$00
   LDA   #$01
   RTS

print_struct_msg:
   SUBROUTINE

   STA   __local_print_struct_msg__ptr
   STX   __local_print_struct_msg__1
   LDY   #$0B
   LDA   (__local_print_struct_msg__ptr),Y
   TAX
   DEY
   LDA   (__local_print_struct_msg__ptr),Y
   JMP   puts

get_internal_struct:
   SUBROUTINE

   LDX   #>internal
   LDA   #<internal
   RTS

str@0:
   DC.B  $73, $74, $61, $74, $69, $63, $20, $73, $74, $72, $75, $63, $74, $00

str@1:
   DC.B  $67, $6C, $6F, $62, $61, $6C, $20, $73, $74, $72, $75, $63, $74, $00

internal:
   DC.W  $0001
   DC.L  $00000000
   DC.L  $40000000
   DC.W  str@0

incomplete_var:
   DC.W  $0003
   DC.L  $00000000
   DC.L  $40100000
   DC.W  str@1
