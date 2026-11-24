SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_get_nested_addr__i_0	EQU	$80
__zpabi_get_subarray__i_0	EQU	$80
__zpabi_read_nested__i_0	EQU	$80
__zpabi_read_nested_negated__i_0	EQU	$80
__zpabi_read_static_nested__j_0	EQU	$80
__zpabi_write_nested__i_0	EQU	$80
__zpabi_write_nested_complex__j_0	EQU	$80
__zpabi_get_nested_addr__i_1	EQU	$81
__zpabi_get_subarray__i_1	EQU	$81
__zpabi_read_nested__i_1	EQU	$81
__zpabi_read_nested_negated__i_1	EQU	$81
__zpabi_read_static_nested__j_1	EQU	$81
__zpabi_write_nested__i_1	EQU	$81
__zpabi_write_nested_complex__j_1	EQU	$81
__zpabi_get_nested_addr__j_0	EQU	$82
__zpabi_read_nested__j_0	EQU	$82
__zpabi_read_nested_negated__j_0	EQU	$82
__zpabi_read_static_nested__k_0	EQU	$82
__zpabi_write_nested__j_0	EQU	$82
__zpabi_write_nested_complex__k_0	EQU	$82
__zpabi_get_nested_addr__j_1	EQU	$83
__zpabi_read_nested__j_1	EQU	$83
__zpabi_read_nested_negated__j_1	EQU	$83
__zpabi_read_static_nested__k_1	EQU	$83
__zpabi_write_nested__j_1	EQU	$83
__zpabi_write_nested_complex__k_1	EQU	$83
__zpabi_get_nested_addr__expected_0	EQU	$84
__zpabi_read_nested__expected_0	EQU	$84
__zpabi_read_nested_negated__expected_0	EQU	$84
__zpabi_read_static_nested__expected_0	EQU	$84
__zpabi_write_nested__new_val_0	EQU	$84
__zpabi_write_nested_complex__val_0	EQU	$84
__zpabi_get_nested_addr__expected_1	EQU	$85
__zpabi_read_nested__expected_1	EQU	$85
__zpabi_read_nested_negated__expected_1	EQU	$85
__zpabi_read_static_nested__expected_1	EQU	$85
__zpabi_write_nested__new_val_1	EQU	$85
__zpabi_write_nested_complex__val_1	EQU	$85
__local_main__0	EQU	$86
__local_main__1	EQU	$87
__local_main__2	EQU	$88
__local_main__3	EQU	$89
__local_main__4	EQU	$8A
__local_main__5	EQU	$8B
__local_main__nested_arr	EQU	$8C
__local_get_nested_addr__0	EQU	$98
__local_get_subarray__0	EQU	$98
__local_read_nested__0	EQU	$98
__local_read_nested_negated__0	EQU	$98
__local_read_static_nested__i	EQU	$98
__local_write_nested__0	EQU	$98
__local_write_nested_complex__0	EQU	$98
__local_get_nested_addr__1	EQU	$99
__local_get_subarray__1	EQU	$99
__local_read_nested__1	EQU	$99
__local_read_nested_negated__1	EQU	$99
__local_read_static_nested__0	EQU	$99
__local_write_nested__1	EQU	$99
__local_write_nested_complex__1	EQU	$99
__local_get_nested_addr__2	EQU	$9A
__local_get_subarray__2	EQU	$9A
__local_read_nested__2	EQU	$9A
__local_read_nested_negated__2	EQU	$9A
__local_read_static_nested__1	EQU	$9A
__local_write_nested__2	EQU	$9A
__local_write_nested_complex__2	EQU	$9A
__local_get_nested_addr__3	EQU	$9B
__local_get_subarray__3	EQU	$9B
__local_read_nested__3	EQU	$9B
__local_read_nested_negated__3	EQU	$9B
__local_read_static_nested__2	EQU	$9B
__local_write_nested__3	EQU	$9B
__local_write_nested_complex__3	EQU	$9B
__local_get_nested_addr__4	EQU	$9C
__local_get_subarray__4	EQU	$9C
__local_read_nested__4	EQU	$9C
__local_read_nested_negated__4	EQU	$9C
__local_read_static_nested__3	EQU	$9C
__local_write_nested__4	EQU	$9C
__local_write_nested_complex__i_0	EQU	$9C
__local_get_nested_addr__5	EQU	$9D
__local_read_nested__5	EQU	$9D
__local_read_nested_negated__5	EQU	$9D
__local_write_nested__5	EQU	$9D
__local_write_nested_complex__i_1	EQU	$9D
__local_get_subarray__nested_0	EQU	$9E
__local_get_nested_addr__nested_arr_0	EQU	$9F
__local_read_nested__nested_arr_0	EQU	$9F
__local_read_nested_negated__nested_arr_0	EQU	$9F
__local_write_nested__nested_arr_0	EQU	$9F
__local_write_nested_complex__5	EQU	$9F

; @zp-link-meta-begin
; def get_array params= locals= indirect=false in_cycle=false
; def get_nested_addr params=__zpabi_get_nested_addr__nested_arr_0,__zpabi_get_nested_addr__nested_arr_1,__zpabi_get_nested_addr__i_0,__zpabi_get_nested_addr__i_1,__zpabi_get_nested_addr__j_0,__zpabi_get_nested_addr__j_1,__zpabi_get_nested_addr__expected_0,__zpabi_get_nested_addr__expected_1 locals=__local_get_nested_addr__0,__local_get_nested_addr__1,__local_get_nested_addr__2,__local_get_nested_addr__3,__local_get_nested_addr__4,__local_get_nested_addr__5,__local_get_nested_addr__nested_arr_1,__local_get_nested_addr__nested_arr_0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def get_subarray params=__zpabi_get_subarray__nested_0,__zpabi_get_subarray__nested_1,__zpabi_get_subarray__i_0,__zpabi_get_subarray__i_1 locals=__local_get_subarray__0,__local_get_subarray__1,__local_get_subarray__2,__local_get_subarray__3,__local_get_subarray__4,__local_get_subarray__nested_1,__local_get_subarray__nested_0 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__nested_arr,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17 indirect=false in_cycle=false
; def read_nested params=__zpabi_read_nested__nested_arr_0,__zpabi_read_nested__nested_arr_1,__zpabi_read_nested__i_0,__zpabi_read_nested__i_1,__zpabi_read_nested__j_0,__zpabi_read_nested__j_1,__zpabi_read_nested__expected_0,__zpabi_read_nested__expected_1 locals=__local_read_nested__0,__local_read_nested__1,__local_read_nested__2,__local_read_nested__3,__local_read_nested__4,__local_read_nested__5,__local_read_nested__nested_arr_1,__local_read_nested__nested_arr_0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def read_nested_negated params=__zpabi_read_nested_negated__nested_arr_0,__zpabi_read_nested_negated__nested_arr_1,__zpabi_read_nested_negated__i_0,__zpabi_read_nested_negated__i_1,__zpabi_read_nested_negated__j_0,__zpabi_read_nested_negated__j_1,__zpabi_read_nested_negated__expected_0,__zpabi_read_nested_negated__expected_1 locals=__local_read_nested_negated__0,__local_read_nested_negated__1,__local_read_nested_negated__2,__local_read_nested_negated__3,__local_read_nested_negated__4,__local_read_nested_negated__5,__local_read_nested_negated__nested_arr_1,__local_read_nested_negated__nested_arr_0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def read_static_nested params=__zpabi_read_static_nested__i_0,__zpabi_read_static_nested__i_1,__zpabi_read_static_nested__j_0,__zpabi_read_static_nested__j_1,__zpabi_read_static_nested__k_0,__zpabi_read_static_nested__k_1,__zpabi_read_static_nested__expected_0,__zpabi_read_static_nested__expected_1 locals=__local_read_static_nested__i,__local_read_static_nested__0,__local_read_static_nested__1,__local_read_static_nested__2,__local_read_static_nested__3 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def write_nested params=__zpabi_write_nested__nested_arr_0,__zpabi_write_nested__nested_arr_1,__zpabi_write_nested__i_0,__zpabi_write_nested__i_1,__zpabi_write_nested__j_0,__zpabi_write_nested__j_1,__zpabi_write_nested__new_val_0,__zpabi_write_nested__new_val_1 locals=__local_write_nested__0,__local_write_nested__1,__local_write_nested__2,__local_write_nested__3,__local_write_nested__4,__local_write_nested__5,__local_write_nested__nested_arr_1,__local_write_nested__nested_arr_0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def write_nested_complex params=__zpabi_write_nested_complex__i_0,__zpabi_write_nested_complex__i_1,__zpabi_write_nested_complex__j_0,__zpabi_write_nested_complex__j_1,__zpabi_write_nested_complex__k_0,__zpabi_write_nested_complex__k_1,__zpabi_write_nested_complex__val_0,__zpabi_write_nested_complex__val_1 locals=__local_write_nested_complex__0,__local_write_nested_complex__1,__local_write_nested_complex__2,__local_write_nested_complex__3,__local_write_nested_complex__i_0,__local_write_nested_complex__i_1,__local_write_nested_complex__4,__local_write_nested_complex__5 indirect=false in_cycle=false param_regs=A,X,,,,,,
; call main -> get_array
; call main -> get_nested_addr
; call main -> get_subarray
; call main -> read_nested
; call main -> read_nested_negated
; call main -> read_static_nested
; call main -> write_nested
; call main -> write_nested_complex
; call write_nested_complex -> get_array
; item read_nested section=.text local=false zero=false
; item write_nested section=.text local=false zero=false
; item read_nested_negated section=.text local=false zero=false
; item get_nested_addr section=.text local=false zero=false
; item read_static_nested section=.text local=false zero=false
; item get_array section=.text local=false zero=false
; item write_nested_complex section=.text local=false zero=false
; item get_subarray section=.text local=false zero=false
; item main section=.text local=false zero=false
; item nested_arr section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

read_nested:
   SUBROUTINE

   STA   __local_read_nested__nested_arr_0
   LDA   __zpabi_read_nested__i_0
   ASL
   STA   __local_read_nested__4
   LDA   __zpabi_read_nested__i_1
   ROL
   STA   __local_read_nested__5
   LDA   __local_read_nested__4
   ASL
   STA   __local_read_nested__2
   LDA   __local_read_nested__5
   ROL
   STA   __local_read_nested__3
   LDA   __local_read_nested__2
   CLC
   ADC   __local_read_nested__4
   STA   __local_read_nested__1
   LDA   __local_read_nested__3
   ADC   __local_read_nested__5
   STA   __local_read_nested__0
   LDA   __local_read_nested__nested_arr_0
   CLC
   ADC   __local_read_nested__1
   STA   __local_read_nested__1
   TXA
   ADC   __local_read_nested__0
   STA   __local_read_nested__3
   LDA   __zpabi_read_nested__j_0
   ASL
   CLC
   ADC   __local_read_nested__1
   STA   __local_read_nested__2
   BCC   .carry_inc@0
   INC   __local_read_nested__3
.carry_inc@0:
   LDY   #$00
   LDA   (__local_read_nested__2),Y
   STA   __local_read_nested__1
   INY
   LDA   (__local_read_nested__2),Y
   CMP   __zpabi_read_nested__expected_1
   BNE   .thread@0
   LDA   __local_read_nested__1
   CMP   __zpabi_read_nested__expected_0
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

write_nested:
   SUBROUTINE

   STA   __local_write_nested__nested_arr_0
   LDA   __zpabi_write_nested__i_0
   ASL
   STA   __local_write_nested__4
   LDA   __zpabi_write_nested__i_1
   ROL
   STA   __local_write_nested__5
   LDA   __local_write_nested__4
   ASL
   STA   __local_write_nested__2
   LDA   __local_write_nested__5
   ROL
   STA   __local_write_nested__3
   LDA   __local_write_nested__2
   CLC
   ADC   __local_write_nested__4
   STA   __local_write_nested__1
   LDA   __local_write_nested__3
   ADC   __local_write_nested__5
   STA   __local_write_nested__0
   LDA   __local_write_nested__nested_arr_0
   CLC
   ADC   __local_write_nested__1
   STA   __local_write_nested__3
   TXA
   ADC   __local_write_nested__0
   STA   __local_write_nested__1
   LDA   __zpabi_write_nested__j_0
   ASL
   CLC
   ADC   __local_write_nested__3
   STA   __local_write_nested__0
   BCC   .carry_inc@1
   INC   __local_write_nested__1
.carry_inc@1:
   LDA   __zpabi_write_nested__new_val_0
   LDY   #$00
   STA   (__local_write_nested__0),Y
   LDA   __zpabi_write_nested__new_val_1
   INY
   STA   (__local_write_nested__0),Y
   LDX   #$00
   TXA
   RTS

read_nested_negated:
   SUBROUTINE

   STA   __local_read_nested_negated__nested_arr_0
   SEC
   LDA   #$00
   SBC   __zpabi_read_nested_negated__i_0
   STA   __local_read_nested_negated__4
   LDA   #$00
   SBC   __zpabi_read_nested_negated__i_1
   ASL   __local_read_nested_negated__4
   ROL
   STA   __local_read_nested_negated__5
   LDA   __local_read_nested_negated__4
   ASL
   STA   __local_read_nested_negated__2
   LDA   __local_read_nested_negated__5
   ROL
   STA   __local_read_nested_negated__3
   LDA   __local_read_nested_negated__2
   CLC
   ADC   __local_read_nested_negated__4
   STA   __local_read_nested_negated__1
   LDA   __local_read_nested_negated__3
   ADC   __local_read_nested_negated__5
   STA   __local_read_nested_negated__0
   LDA   __local_read_nested_negated__nested_arr_0
   CLC
   ADC   __local_read_nested_negated__1
   STA   __local_read_nested_negated__1
   TXA
   ADC   __local_read_nested_negated__0
   STA   __local_read_nested_negated__3
   LDA   __zpabi_read_nested_negated__j_0
   ASL
   CLC
   ADC   __local_read_nested_negated__1
   STA   __local_read_nested_negated__2
   BCC   .carry_inc@2
   INC   __local_read_nested_negated__3
.carry_inc@2:
   LDY   #$00
   LDA   (__local_read_nested_negated__2),Y
   STA   __local_read_nested_negated__1
   INY
   LDA   (__local_read_nested_negated__2),Y
   CMP   __zpabi_read_nested_negated__expected_1
   BNE   .thread@1
   LDA   __local_read_nested_negated__1
   CMP   __zpabi_read_nested_negated__expected_0
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

get_nested_addr:
   SUBROUTINE

   STA   __local_get_nested_addr__nested_arr_0
   LDA   __zpabi_get_nested_addr__i_0
   ASL
   STA   __local_get_nested_addr__4
   LDA   __zpabi_get_nested_addr__i_1
   ROL
   STA   __local_get_nested_addr__5
   LDA   __local_get_nested_addr__4
   ASL
   STA   __local_get_nested_addr__2
   LDA   __local_get_nested_addr__5
   ROL
   STA   __local_get_nested_addr__3
   LDA   __local_get_nested_addr__2
   CLC
   ADC   __local_get_nested_addr__4
   STA   __local_get_nested_addr__1
   LDA   __local_get_nested_addr__3
   ADC   __local_get_nested_addr__5
   STA   __local_get_nested_addr__0
   LDA   __local_get_nested_addr__nested_arr_0
   CLC
   ADC   __local_get_nested_addr__1
   STA   __local_get_nested_addr__2
   TXA
   ADC   __local_get_nested_addr__0
   STA   __local_get_nested_addr__1
   LDA   __zpabi_get_nested_addr__j_0
   ASL
   CLC
   ADC   __local_get_nested_addr__2
   STA   __local_get_nested_addr__0
   BCC   .carry_inc@3
   INC   __local_get_nested_addr__1
.carry_inc@3:
   LDA   __local_get_nested_addr__1
   CMP   __zpabi_get_nested_addr__expected_1
   BNE   .thread@2
   LDA   __local_get_nested_addr__0
   CMP   __zpabi_get_nested_addr__expected_0
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDX   #$00
   RTS

read_static_nested:
   SUBROUTINE

   ASL
   STA   __local_read_static_nested__0
   ASL
   ASL
   ASL
   ASL
   SEC
   SBC   __local_read_static_nested__0
   STA   __local_read_static_nested__i
   LDA   #<nested_arr
   CLC
   ADC   __local_read_static_nested__i
   STA   __local_read_static_nested__3
   LDA   #>nested_arr
   ADC   #$00
   STA   __local_read_static_nested__2
   LDA   __zpabi_read_static_nested__j_0
   ASL
   STA   __local_read_static_nested__0
   ASL
   ASL
   CLC
   ADC   __local_read_static_nested__0
   CLC
   ADC   __local_read_static_nested__3
   STA   __local_read_static_nested__0
   BCC   .carry_inc@4
   INC   __local_read_static_nested__2
.carry_inc@4:
   LDA   __zpabi_read_static_nested__k_0
   ASL
   CLC
   ADC   __local_read_static_nested__0
   STA   __local_read_static_nested__1
   BCC   .carry_inc@5
   INC   __local_read_static_nested__2
.carry_inc@5:
   LDY   #$00
   LDA   (__local_read_static_nested__1),Y
   STA   __local_read_static_nested__0
   INY
   LDA   (__local_read_static_nested__1),Y
   CMP   __zpabi_read_static_nested__expected_1
   BNE   .thread@3
   LDA   __local_read_static_nested__0
   CMP   __zpabi_read_static_nested__expected_0
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDX   #$00
   RTS

get_array:
   SUBROUTINE

   LDX   #>nested_arr
   LDA   #<nested_arr
   RTS

write_nested_complex:
   SUBROUTINE

   STA   __local_write_nested_complex__i_0
   STX   __local_write_nested_complex__i_1
   JSR   get_array
   STA   __local_write_nested_complex__5
   ASL   __local_write_nested_complex__i_0
   ROL   __local_write_nested_complex__i_1
   LDA   __local_write_nested_complex__i_0
   ASL
   STA   __local_write_nested_complex__2
   LDA   __local_write_nested_complex__i_1
   ROL
   ASL   __local_write_nested_complex__2
   ROL
   ASL   __local_write_nested_complex__2
   ROL
   ASL   __local_write_nested_complex__2
   ROL
   STA   __local_write_nested_complex__3
   LDA   __local_write_nested_complex__2
   SEC
   SBC   __local_write_nested_complex__i_0
   STA   __local_write_nested_complex__1
   LDA   __local_write_nested_complex__3
   SBC   __local_write_nested_complex__i_1
   STA   __local_write_nested_complex__0
   LDA   __local_write_nested_complex__5
   CLC
   ADC   __local_write_nested_complex__1
   STA   __local_write_nested_complex__i_0
   TXA
   ADC   __local_write_nested_complex__0
   STA   __local_write_nested_complex__1
   LDA   __zpabi_write_nested_complex__j_0
   ASL
   STA   __local_write_nested_complex__3
   ASL
   ASL
   CLC
   ADC   __local_write_nested_complex__3
   CLC
   ADC   __local_write_nested_complex__i_0
   STA   __local_write_nested_complex__3
   BCC   .carry_inc@6
   INC   __local_write_nested_complex__1
.carry_inc@6:
   LDA   __zpabi_write_nested_complex__k_0
   ASL
   CLC
   ADC   __local_write_nested_complex__3
   STA   __local_write_nested_complex__0
   BCC   .carry_inc@7
   INC   __local_write_nested_complex__1
.carry_inc@7:
   LDA   __zpabi_write_nested_complex__val_0
   LDY   #$00
   STA   (__local_write_nested_complex__0),Y
   LDA   __zpabi_write_nested_complex__val_1
   INY
   STA   (__local_write_nested_complex__0),Y
   LDX   #$00
   TXA
   RTS

get_subarray:
   SUBROUTINE

   STA   __local_get_subarray__nested_0
   LDA   __zpabi_get_subarray__i_0
   ASL
   STA   __local_get_subarray__3
   LDA   __zpabi_get_subarray__i_1
   ROL
   STA   __local_get_subarray__4
   LDA   __local_get_subarray__3
   ASL
   STA   __local_get_subarray__1
   LDA   __local_get_subarray__4
   ROL
   STA   __local_get_subarray__2
   LDA   __local_get_subarray__1
   CLC
   ADC   __local_get_subarray__3
   STA   __local_get_subarray__0
   LDA   __local_get_subarray__2
   ADC   __local_get_subarray__4
   STA   __local_get_subarray__1
   LDA   __local_get_subarray__nested_0
   CLC
   ADC   __local_get_subarray__0
   STA   __local_get_subarray__0
   TXA
   ADC   __local_get_subarray__1
   TAX
   LDA   __local_get_subarray__0
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__nested_arr
   LDA   #$00
   STA   __local_main__nested_arr+1
   LDY   #$02
   STY   __local_main__nested_arr+2
   STA   __local_main__nested_arr+3
   INY
   STY   __local_main__nested_arr+4
   STA   __local_main__nested_arr+5
   INY
   STY   __local_main__nested_arr+6
   STA   __local_main__nested_arr+7
   INY
   STY   __local_main__nested_arr+8
   STA   __local_main__nested_arr+9
   INY
   STY   __local_main__nested_arr+10
   STA   __local_main__nested_arr+11
   LDA   #$01
   STA   __zpabi_read_nested__i_0
   LDA   #$02
   STA   __zpabi_read_nested__j_0
   STY   __zpabi_read_nested__expected_0
   LDA   #$00
   STA   __zpabi_read_nested__i_1
   STA   __zpabi_read_nested__j_1
   STA   __zpabi_read_nested__expected_1
   LDX   #>__local_main__nested_arr
   LDA   #<__local_main__nested_arr
   JSR   read_nested
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$01
   STA   __zpabi_write_nested__i_0
   LDA   #$00
   STA   __zpabi_write_nested__i_1
   STA   __zpabi_write_nested__j_1
   LDA   #$02
   STA   __zpabi_write_nested__j_0
   LDA   #$FF
   STA   __zpabi_write_nested__new_val_0
   STA   __zpabi_write_nested__new_val_1
   LDX   #>__local_main__nested_arr
   LDA   #<__local_main__nested_arr
   JSR   write_nested
   LDA   __local_main__nested_arr+10
   STA   __local_main__1
   LDA   __local_main__nested_arr+11
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   __local_main__1
   CMP   #$FF
   BEQ   .if_end@1
.jcmp_skip@12:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #>__local_main__nested_arr
   STA   __local_main__2
   LDA   #<__local_main__nested_arr
   CLC
   ADC   #$0C
   STA   __local_main__0
   BCC   .carry_inc@8
   INC   __local_main__2
.carry_inc@8:
   LDA   #$02
   STA   __zpabi_read_nested_negated__i_0
   LDA   #$01
   STA   __zpabi_read_nested_negated__expected_0
   LDA   #$00
   STA   __zpabi_read_nested_negated__i_1
   STA   __zpabi_read_nested_negated__j_0
   STA   __zpabi_read_nested_negated__j_1
   STA   __zpabi_read_nested_negated__expected_1
   LDX   __local_main__2
   LDA   __local_main__0
   JSR   read_nested_negated
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #<(__local_main__nested_arr+2)
   STA   __zpabi_get_nested_addr__expected_0
   LDA   #>(__local_main__nested_arr+2)
   STA   __zpabi_get_nested_addr__expected_1
   LDA   #$01
   STA   __zpabi_get_nested_addr__j_0
   LDA   #$00
   STA   __zpabi_get_nested_addr__i_0
   STA   __zpabi_get_nested_addr__i_1
   STA   __zpabi_get_nested_addr__j_1
   LDX   #>__local_main__nested_arr
   LDA   #<__local_main__nested_arr
   JSR   get_nested_addr
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #$01
   STA   __zpabi_read_static_nested__j_0
   LDA   #$05
   STA   __zpabi_read_static_nested__expected_0
   LDA   #$00
   STA   __zpabi_read_static_nested__j_1
   STA   __zpabi_read_static_nested__k_0
   STA   __zpabi_read_static_nested__k_1
   STA   __zpabi_read_static_nested__expected_1
   LDX   #$00
   LDA   #$01
   JSR   read_static_nested
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #$02
   STA   __zpabi_write_nested_complex__j_0
   LDA   #$03
   STA   __zpabi_write_nested_complex__k_0
   LDA   #$6F
   STA   __zpabi_write_nested_complex__val_0
   LDA   #$00
   STA   __zpabi_write_nested_complex__j_1
   STA   __zpabi_write_nested_complex__k_1
   STA   __zpabi_write_nested_complex__val_1
   LDX   #$00
   JSR   write_nested_complex
   JSR   get_array
   STA   __local_main__2
   STX   __local_main__3
   LDY   #$1A
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@13
   LDA   __local_main__1
   CMP   #$6F
   BEQ   .if_end@5
.jcmp_skip@13:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   #$01
   STA   __zpabi_get_subarray__i_0
   LDA   #$00
   STA   __zpabi_get_subarray__i_1
   LDX   #>__local_main__nested_arr
   LDA   #<__local_main__nested_arr
   JSR   get_subarray
   STX   __local_main__5
   CLC
   ADC   #$02
   STA   __local_main__4
   BCC   .carry_inc@9
   INC   __local_main__5
.carry_inc@9:
   LDA   __local_main__5
   CMP   #>(__local_main__nested_arr+8)
   BNE   .jcmp_skip@14
   LDA   __local_main__4
   CMP   #<(__local_main__nested_arr+8)
   BEQ   .if_end@6
.jcmp_skip@14:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDX   #$00
   TXA
   RTS

nested_arr:
   DC.W  $0001
   DC.W  $0002
   DS.B  6
   DC.W  $0003
   DS.B  18
   DC.W  $0004
   DS.B  8
   DC.W  $0005
   DS.B  78
