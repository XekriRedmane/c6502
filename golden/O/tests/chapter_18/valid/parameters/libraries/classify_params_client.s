SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_test_flattened_ints__s_0	EQU	$80
__zpabi_test_large__s_0	EQU	$80
__zpabi_test_nested_double__s_0	EQU	$80
__zpabi_test_nested_ints__s_0	EQU	$80
__zpabi_test_pass_in_memory__s_0	EQU	$80
__zpabi_test_twelve_bytes__s_0	EQU	$80
__zpabi_test_two_eightbytes__s_0	EQU	$80
__zpabi_test_two_ints__s_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__zpabi_test_flattened_ints__s_1	EQU	$81
__zpabi_test_large__s_1	EQU	$81
__zpabi_test_nested_double__s_1	EQU	$81
__zpabi_test_nested_ints__s_1	EQU	$81
__zpabi_test_pass_in_memory__s_1	EQU	$81
__zpabi_test_twelve_bytes__s_1	EQU	$81
__zpabi_test_two_eightbytes__s_1	EQU	$81
__zpabi_test_two_ints__s_1	EQU	$81
__zpabi_test_flattened_ints__s_2	EQU	$82
__zpabi_test_large__s_2	EQU	$82
__zpabi_test_nested_double__s_2	EQU	$82
__zpabi_test_nested_ints__s_2	EQU	$82
__zpabi_test_pass_in_memory__s_2	EQU	$82
__zpabi_test_twelve_bytes__s_2	EQU	$82
__zpabi_test_two_eightbytes__s_2	EQU	$82
__zpabi_test_two_ints__s_2	EQU	$82
__zpabi_test_flattened_ints__s_3	EQU	$83
__zpabi_test_large__s_3	EQU	$83
__zpabi_test_nested_double__s_3	EQU	$83
__zpabi_test_nested_ints__s_3	EQU	$83
__zpabi_test_pass_in_memory__s_3	EQU	$83
__zpabi_test_twelve_bytes__s_3	EQU	$83
__zpabi_test_two_eightbytes__s_3	EQU	$83
__zpabi_test_two_ints__s_3	EQU	$83
__zpabi_test_large__s_4	EQU	$84
__zpabi_test_nested_double__s_4	EQU	$84
__zpabi_test_pass_in_memory__s_4	EQU	$84
__zpabi_test_twelve_bytes__s_4	EQU	$84
__zpabi_test_two_eightbytes__s_4	EQU	$84
__zpabi_test_large__s_5	EQU	$85
__zpabi_test_nested_double__s_5	EQU	$85
__zpabi_test_pass_in_memory__s_5	EQU	$85
__zpabi_test_twelve_bytes__s_5	EQU	$85
__zpabi_test_two_eightbytes__s_5	EQU	$85
__zpabi_test_large__s_6	EQU	$86
__zpabi_test_nested_double__s_6	EQU	$86
__zpabi_test_pass_in_memory__s_6	EQU	$86
__zpabi_test_twelve_bytes__s_6	EQU	$86
__zpabi_test_two_eightbytes__s_6	EQU	$86
__zpabi_test_large__s_7	EQU	$87
__zpabi_test_nested_double__s_7	EQU	$87
__zpabi_test_pass_in_memory__s_7	EQU	$87
__zpabi_test_twelve_bytes__s_7	EQU	$87
__zpabi_test_two_eightbytes__s_7	EQU	$87
__zpabi_test_large__s_8	EQU	$88
__zpabi_test_pass_in_memory__s_8	EQU	$88
__zpabi_test_twelve_bytes__s_8	EQU	$88
__zpabi_test_two_eightbytes__s_8	EQU	$88
__zpabi_test_large__s_9	EQU	$89
__zpabi_test_pass_in_memory__s_9	EQU	$89
__zpabi_test_twelve_bytes__s_9	EQU	$89
__zpabi_test_large__s_10	EQU	$8A
__zpabi_test_pass_in_memory__s_10	EQU	$8A
__zpabi_test_large__s_11	EQU	$8B
__zpabi_test_pass_in_memory__s_11	EQU	$8B
__zpabi_test_large__s_12	EQU	$8C
__zpabi_test_pass_in_memory__s_12	EQU	$8C
__zpabi_test_large__s_13	EQU	$8D
__zpabi_test_pass_in_memory__s_13	EQU	$8D
__zpabi_test_large__s_14	EQU	$8E
__zpabi_test_pass_in_memory__s_14	EQU	$8E
__zpabi_test_large__s_15	EQU	$8F
__zpabi_test_pass_in_memory__s_15	EQU	$8F
__zpabi_test_large__s_16	EQU	$90
__zpabi_test_pass_in_memory__s_16	EQU	$90
__zpabi_test_large__s_17	EQU	$91
__zpabi_test_pass_in_memory__s_17	EQU	$91
__zpabi_test_large__s_18	EQU	$92
__zpabi_test_pass_in_memory__s_18	EQU	$92
__zpabi_test_large__s_19	EQU	$93
__zpabi_test_pass_in_memory__s_19	EQU	$93
__zpabi_test_pass_in_memory__s_20	EQU	$94
__zpabi_test_pass_in_memory__s_21	EQU	$95
__local_main__0	EQU	$96

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; ext test_flattened_ints params=__zpabi_test_flattened_ints__s_0,__zpabi_test_flattened_ints__s_1,__zpabi_test_flattened_ints__s_2,__zpabi_test_flattened_ints__s_3
; ext test_large params=__zpabi_test_large__s_0,__zpabi_test_large__s_1,__zpabi_test_large__s_2,__zpabi_test_large__s_3,__zpabi_test_large__s_4,__zpabi_test_large__s_5,__zpabi_test_large__s_6,__zpabi_test_large__s_7,__zpabi_test_large__s_8,__zpabi_test_large__s_9,__zpabi_test_large__s_10,__zpabi_test_large__s_11,__zpabi_test_large__s_12,__zpabi_test_large__s_13,__zpabi_test_large__s_14,__zpabi_test_large__s_15,__zpabi_test_large__s_16,__zpabi_test_large__s_17,__zpabi_test_large__s_18,__zpabi_test_large__s_19
; ext test_nested_double params=__zpabi_test_nested_double__s_0,__zpabi_test_nested_double__s_1,__zpabi_test_nested_double__s_2,__zpabi_test_nested_double__s_3,__zpabi_test_nested_double__s_4,__zpabi_test_nested_double__s_5,__zpabi_test_nested_double__s_6,__zpabi_test_nested_double__s_7
; ext test_nested_ints params=__zpabi_test_nested_ints__s_0,__zpabi_test_nested_ints__s_1,__zpabi_test_nested_ints__s_2,__zpabi_test_nested_ints__s_3
; ext test_pass_in_memory params=__zpabi_test_pass_in_memory__s_0,__zpabi_test_pass_in_memory__s_1,__zpabi_test_pass_in_memory__s_2,__zpabi_test_pass_in_memory__s_3,__zpabi_test_pass_in_memory__s_4,__zpabi_test_pass_in_memory__s_5,__zpabi_test_pass_in_memory__s_6,__zpabi_test_pass_in_memory__s_7,__zpabi_test_pass_in_memory__s_8,__zpabi_test_pass_in_memory__s_9,__zpabi_test_pass_in_memory__s_10,__zpabi_test_pass_in_memory__s_11,__zpabi_test_pass_in_memory__s_12,__zpabi_test_pass_in_memory__s_13,__zpabi_test_pass_in_memory__s_14,__zpabi_test_pass_in_memory__s_15,__zpabi_test_pass_in_memory__s_16,__zpabi_test_pass_in_memory__s_17,__zpabi_test_pass_in_memory__s_18,__zpabi_test_pass_in_memory__s_19,__zpabi_test_pass_in_memory__s_20,__zpabi_test_pass_in_memory__s_21
; ext test_twelve_bytes params=__zpabi_test_twelve_bytes__s_0,__zpabi_test_twelve_bytes__s_1,__zpabi_test_twelve_bytes__s_2,__zpabi_test_twelve_bytes__s_3,__zpabi_test_twelve_bytes__s_4,__zpabi_test_twelve_bytes__s_5,__zpabi_test_twelve_bytes__s_6,__zpabi_test_twelve_bytes__s_7,__zpabi_test_twelve_bytes__s_8,__zpabi_test_twelve_bytes__s_9
; ext test_two_eightbytes params=__zpabi_test_two_eightbytes__s_0,__zpabi_test_two_eightbytes__s_1,__zpabi_test_two_eightbytes__s_2,__zpabi_test_two_eightbytes__s_3,__zpabi_test_two_eightbytes__s_4,__zpabi_test_two_eightbytes__s_5,__zpabi_test_two_eightbytes__s_6,__zpabi_test_two_eightbytes__s_7,__zpabi_test_two_eightbytes__s_8
; ext test_two_ints params=__zpabi_test_two_ints__s_0,__zpabi_test_two_ints__s_1,__zpabi_test_two_ints__s_2,__zpabi_test_two_ints__s_3
; call main -> test_flattened_ints
; call main -> test_large
; call main -> test_nested_double
; call main -> test_nested_ints
; call main -> test_pass_in_memory
; call main -> test_twelve_bytes
; call main -> test_two_eightbytes
; call main -> test_two_ints
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$6C
   STA   __zpabi_test_twelve_bytes__s_2
   LDA   #$6D
   STA   __zpabi_test_twelve_bytes__s_3
   LDA   #$6E
   STA   __zpabi_test_twelve_bytes__s_4
   LDA   #$6F
   STA   __zpabi_test_twelve_bytes__s_5
   LDA   #$70
   STA   __zpabi_test_twelve_bytes__s_6
   LDA   #$71
   STA   __zpabi_test_twelve_bytes__s_7
   LDA   #$72
   STA   __zpabi_test_twelve_bytes__s_8
   LDA   #$00
   STA   __zpabi_test_twelve_bytes__s_0
   STA   __zpabi_test_twelve_bytes__s_1
   STA   __zpabi_test_twelve_bytes__s_9
   JSR   test_twelve_bytes
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$7F
   STA   __zpabi_test_nested_ints__s_0
   LDA   #$FF
   STA   __zpabi_test_nested_ints__s_1
   STA   __zpabi_test_nested_ints__s_2
   LDA   #$80
   STA   __zpabi_test_nested_ints__s_3
   JSR   test_nested_ints
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$7F
   STA   __zpabi_test_flattened_ints__s_0
   LDA   #$FF
   STA   __zpabi_test_flattened_ints__s_1
   STA   __zpabi_test_flattened_ints__s_2
   LDA   #$80
   STA   __zpabi_test_flattened_ints__s_3
   JSR   test_flattened_ints
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$40
   STA   __zpabi_test_large__s_0
   LDA   #$0D
   STA   __zpabi_test_large__s_1
   LDA   #$00
   STA   __zpabi_test_large__s_2
   LDY   #$00
   STY   __zpabi_test_large__s_3
   STY   __zpabi_test_large__s_4
   STY   __zpabi_test_large__s_5
   STA   __zpabi_test_large__s_6
   LDA   #$40
   STA   __zpabi_test_large__s_7
   LDY   #$37
   STY   __zpabi_test_large__s_8
   STA   __zpabi_test_large__s_9
   LDA   #$61
   STA   __zpabi_test_large__s_10
   LDA   #$62
   STA   __zpabi_test_large__s_11
   LDA   #$63
   STA   __zpabi_test_large__s_12
   LDA   #$64
   STA   __zpabi_test_large__s_13
   LDA   #$65
   STA   __zpabi_test_large__s_14
   LDA   #$66
   STA   __zpabi_test_large__s_15
   LDA   #$67
   STA   __zpabi_test_large__s_16
   LDA   #$68
   STA   __zpabi_test_large__s_17
   LDA   #$69
   STA   __zpabi_test_large__s_18
   LDA   #$00
   STA   __zpabi_test_large__s_19
   JSR   test_large
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #$E7
   STA   __zpabi_test_two_ints__s_0
   LDA   #$03
   STA   __zpabi_test_two_ints__s_1
   LDY   #$78
   STY   __zpabi_test_two_ints__s_2
   STA   __zpabi_test_two_ints__s_3
   JSR   test_two_ints
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #$00
   STA   __zpabi_test_nested_double__s_0
   LDY   #$00
   STY   __zpabi_test_nested_double__s_1
   STY   __zpabi_test_nested_double__s_2
   STA   __zpabi_test_nested_double__s_3
   LDA   #$89
   STA   __zpabi_test_nested_double__s_5
   LDA   #$D8
   STA   __zpabi_test_nested_double__s_6
   LDA   #$40
   STA   __zpabi_test_nested_double__s_4
   STA   __zpabi_test_nested_double__s_7
   JSR   test_nested_double
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   #$00
   STA   __zpabi_test_two_eightbytes__s_0
   LDY   #$00
   STY   __zpabi_test_two_eightbytes__s_1
   STY   __zpabi_test_two_eightbytes__s_2
   STY   __zpabi_test_two_eightbytes__s_3
   STA   __zpabi_test_two_eightbytes__s_4
   LDA   #$40
   STA   __zpabi_test_two_eightbytes__s_5
   LDY   #$8F
   STY   __zpabi_test_two_eightbytes__s_6
   STA   __zpabi_test_two_eightbytes__s_7
   LDA   #$78
   STA   __zpabi_test_two_eightbytes__s_8
   JSR   test_two_eightbytes
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   #$76
   STA   __zpabi_test_pass_in_memory__s_0
   STA   __zpabi_test_pass_in_memory__s_8
   LDA   #$3B
   STA   __zpabi_test_pass_in_memory__s_1
   STA   __zpabi_test_pass_in_memory__s_9
   LDA   #$77
   STA   __zpabi_test_pass_in_memory__s_2
   STA   __zpabi_test_pass_in_memory__s_10
   LDA   #$30
   STA   __zpabi_test_pass_in_memory__s_3
   STA   __zpabi_test_pass_in_memory__s_11
   LDA   #$D1
   STA   __zpabi_test_pass_in_memory__s_4
   STA   __zpabi_test_pass_in_memory__s_12
   LDA   #$42
   STA   __zpabi_test_pass_in_memory__s_5
   STA   __zpabi_test_pass_in_memory__s_13
   LDA   #$EE
   STA   __zpabi_test_pass_in_memory__s_6
   STA   __zpabi_test_pass_in_memory__s_14
   LDA   #$7F
   STA   __zpabi_test_pass_in_memory__s_7
   LDA   #$FF
   STA   __zpabi_test_pass_in_memory__s_15
   LDA   #$01
   STA   __zpabi_test_pass_in_memory__s_16
   LDY   #$00
   STY   __zpabi_test_pass_in_memory__s_17
   STA   __zpabi_test_pass_in_memory__s_18
   TYA
   STA   __zpabi_test_pass_in_memory__s_19
   STY   __zpabi_test_pass_in_memory__s_20
   STA   __zpabi_test_pass_in_memory__s_21
   JSR   test_pass_in_memory
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS
