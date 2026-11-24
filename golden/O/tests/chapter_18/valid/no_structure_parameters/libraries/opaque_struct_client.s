SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check_struct__expected_i_0	EQU	$80
__zpabi_create_struct__d_0	EQU	$80
__zpabi_check_struct__expected_i_1	EQU	$81
__zpabi_create_struct__d_1	EQU	$81
__zpabi_check_struct__expected_d_0	EQU	$82
__zpabi_create_struct__d_2	EQU	$82
__zpabi_check_struct__expected_d_1	EQU	$83
__zpabi_create_struct__d_3	EQU	$83
__zpabi_check_struct__expected_d_2	EQU	$84
__zpabi_create_struct__d_4	EQU	$84
__zpabi_check_struct__expected_d_3	EQU	$85
__zpabi_create_struct__d_5	EQU	$85
__zpabi_check_struct__expected_d_4	EQU	$86
__zpabi_create_struct__d_6	EQU	$86
__zpabi_check_struct__expected_d_5	EQU	$87
__zpabi_create_struct__d_7	EQU	$87
__zpabi_check_struct__expected_d_6	EQU	$88
__zpabi_create_struct__s_0	EQU	$88
__zpabi_check_struct__expected_d_7	EQU	$89
__zpabi_create_struct__s_1	EQU	$89
__zpabi_check_struct__expected_s_0	EQU	$8A
__zpabi_check_struct__expected_s_1	EQU	$8B
__local_main__0	EQU	$8C
__local_main__1	EQU	$8D
__local_main__2	EQU	$8E
__local_main__3	EQU	$8F

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5 indirect=false in_cycle=false
; ext check_struct params=__zpabi_check_struct__ptr_0,__zpabi_check_struct__ptr_1,__zpabi_check_struct__expected_i_0,__zpabi_check_struct__expected_i_1,__zpabi_check_struct__expected_d_0,__zpabi_check_struct__expected_d_1,__zpabi_check_struct__expected_d_2,__zpabi_check_struct__expected_d_3,__zpabi_check_struct__expected_d_4,__zpabi_check_struct__expected_d_5,__zpabi_check_struct__expected_d_6,__zpabi_check_struct__expected_d_7,__zpabi_check_struct__expected_s_0,__zpabi_check_struct__expected_s_1 param_regs=A,X,,,,,,,,,,,,
; ext create_struct params=__zpabi_create_struct__i_0,__zpabi_create_struct__i_1,__zpabi_create_struct__d_0,__zpabi_create_struct__d_1,__zpabi_create_struct__d_2,__zpabi_create_struct__d_3,__zpabi_create_struct__d_4,__zpabi_create_struct__d_5,__zpabi_create_struct__d_6,__zpabi_create_struct__d_7,__zpabi_create_struct__s_0,__zpabi_create_struct__s_1 param_regs=A,X,,,,,,,,,,
; ext get_internal_struct params=
; ext increment_struct params=__zpabi_increment_struct__src_ptr_0,__zpabi_increment_struct__src_ptr_1 param_regs=A,X
; ext print_struct_msg params=__zpabi_print_struct_msg__ptr_0,__zpabi_print_struct_msg__ptr_1 param_regs=A,X
; call main -> check_struct
; call main -> create_struct
; call main -> get_internal_struct
; call main -> increment_struct
; call main -> print_struct_msg
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_create_struct__d_0
   STA   __zpabi_create_struct__d_1
   STA   __zpabi_create_struct__d_2
   STA   __zpabi_create_struct__d_3
   STA   __zpabi_create_struct__d_4
   LDA   #$80
   STA   __zpabi_create_struct__d_5
   LDA   #$59
   STA   __zpabi_create_struct__d_6
   LDA   #$40
   STA   __zpabi_create_struct__d_7
   LDA   #<str@0
   STA   __zpabi_create_struct__s_0
   LDA   #>str@0
   STA   __zpabi_create_struct__s_1
   LDX   #$00
   LDA   #$65
   JSR   create_struct
   STA   __local_main__3
   STX   __local_main__2
   JSR   get_internal_struct
   STA   __local_main__1
   STX   __local_main__0
   LDX   __local_main__2
   LDA   __local_main__3
   JSR   print_struct_msg
   LDX   __local_main__0
   LDA   __local_main__1
   JSR   print_struct_msg
   LDX   #>incomplete_var
   LDA   #<incomplete_var
   JSR   print_struct_msg
   LDX   __local_main__2
   LDA   __local_main__3
   JSR   increment_struct
   LDX   #>incomplete_var
   LDA   #<incomplete_var
   JSR   increment_struct
   LDA   #$66
   STA   __zpabi_check_struct__expected_i_0
   JSR   main@run0
   LDA   #$C0
   STA   __zpabi_check_struct__expected_d_5
   LDA   #$59
   STA   __zpabi_check_struct__expected_d_6
   LDA   #$40
   STA   __zpabi_check_struct__expected_d_7
   LDA   #<str@1
   STA   __zpabi_check_struct__expected_s_0
   LDA   #>str@1
   STA   __zpabi_check_struct__expected_s_1
   LDX   __local_main__2
   LDA   __local_main__3
   JSR   check_struct
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$04
   STA   __zpabi_check_struct__expected_i_0
   JSR   main@run0
   STA   __zpabi_check_struct__expected_d_5
   LDA   #$14
   STA   __zpabi_check_struct__expected_d_6
   LDA   #$40
   STA   __zpabi_check_struct__expected_d_7
   LDA   #<str@2
   STA   __zpabi_check_struct__expected_s_0
   LDA   #>str@2
   STA   __zpabi_check_struct__expected_s_1
   LDX   #>incomplete_var
   LDA   #<incomplete_var
   JSR   check_struct
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_check_struct__expected_i_1
   STA   __zpabi_check_struct__expected_d_0
   STA   __zpabi_check_struct__expected_d_1
   STA   __zpabi_check_struct__expected_d_2
   STA   __zpabi_check_struct__expected_d_3
   STA   __zpabi_check_struct__expected_d_4
   RTS

str@0:
   DC.B  $6E, $65, $77, $20, $73, $74, $72, $75, $63, $74, $00

str@1:
   DC.B  $6E, $65, $77, $20, $73, $74, $72, $75, $63, $74, $00

str@2:
   DC.B  $67, $6C, $6F, $62, $61, $6C, $20, $73, $74, $72, $75, $63, $74, $00
