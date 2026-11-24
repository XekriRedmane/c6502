SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_use_struct_pointers__param_1	EQU	$80
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_malloc__size_0	EQU	$80
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_malloc__size_1	EQU	$81
__zpabi_strcmp__s2_1	EQU	$81
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_malloc__size_2	EQU	$82
__local_validate_incomplete_var__0	EQU	$83
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_malloc__size_3	EQU	$83
__local_print_msg__param	EQU	$84
__local_test_deref_incomplete_var__0	EQU	$84
__zpabi_calloc__size_0	EQU	$84
__local_make_struct__0	EQU	$85
__local_print_msg__1	EQU	$85
__local_test_deref_incomplete_var__1	EQU	$85
__zpabi_calloc__size_1	EQU	$85
__local_make_struct__1	EQU	$86
__zpabi_calloc__size_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87
__local_main__0	EQU	$88
__local_test_use_incomplete_struct_pointers__0	EQU	$8A
__local_test_use_incomplete_struct_pointers__1	EQU	$8B
__local_validate_struct__1	EQU	$8B
__local_test_use_incomplete_struct_pointers__2	EQU	$8C
__local_validate_struct__2	EQU	$8C
__local_test_use_incomplete_struct_pointers__3	EQU	$8D
__local_validate_struct__3	EQU	$8D
__local_test_use_incomplete_struct_pointers__4	EQU	$8E
__local_validate_struct__ptr	EQU	$8E
__local_validate_struct__4	EQU	$8F

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def make_struct params= locals=__local_make_struct__0,__local_make_struct__1 indirect=false in_cycle=false
; def print_msg params=__zpabi_print_msg__param_0,__zpabi_print_msg__param_1 locals=__local_print_msg__0,__local_print_msg__param,__local_print_msg__1 indirect=false in_cycle=false param_regs=A,X
; def test_block_scope_forward_decl params= locals=__local_test_block_scope_forward_decl__val_3,__local_test_block_scope_forward_decl__val_2,__local_test_block_scope_forward_decl__val_1,__local_test_block_scope_forward_decl__val_0 indirect=false in_cycle=false
; def test_deref_incomplete_var params= locals=__local_test_deref_incomplete_var__0,__local_test_deref_incomplete_var__1 indirect=false in_cycle=false
; def test_file_scope_forward_decl params= locals=__local_test_file_scope_forward_decl__0 indirect=false in_cycle=false
; def test_incomplete_var params= locals=__local_test_incomplete_var__0 indirect=false in_cycle=false
; def test_use_incomplete_struct_pointers params= locals=__local_test_use_incomplete_struct_pointers__0,__local_test_use_incomplete_struct_pointers__1,__local_test_use_incomplete_struct_pointers__2,__local_test_use_incomplete_struct_pointers__3,__local_test_use_incomplete_struct_pointers__4 indirect=false in_cycle=false
; def use_struct_pointers params=__zpabi_use_struct_pointers__param_0,__zpabi_use_struct_pointers__param_1 locals=__local_use_struct_pointers__param_1,__local_use_struct_pointers__param_0 indirect=false in_cycle=false param_regs=A,X
; def validate_incomplete_var params= locals=__local_validate_incomplete_var__0,__local_validate_incomplete_var__1 indirect=false in_cycle=false
; def validate_struct params=__zpabi_validate_struct__ptr_0,__zpabi_validate_struct__ptr_1 locals=__local_validate_struct__0,__local_validate_struct__1,__local_validate_struct__2,__local_validate_struct__3,__local_validate_struct__ptr,__local_validate_struct__4 indirect=false in_cycle=false param_regs=A,X
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext puts params=__zpabi_puts__s_0,__zpabi_puts__s_1 param_regs=A,X
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call main -> test_block_scope_forward_decl
; call main -> test_deref_incomplete_var
; call main -> test_file_scope_forward_decl
; call main -> test_incomplete_var
; call main -> test_use_incomplete_struct_pointers
; call make_struct -> malloc
; call print_msg -> puts
; call test_deref_incomplete_var -> malloc
; call test_file_scope_forward_decl -> make_struct
; call test_file_scope_forward_decl -> validate_struct
; call test_incomplete_var -> print_msg
; call test_incomplete_var -> validate_incomplete_var
; call test_use_incomplete_struct_pointers -> calloc
; call test_use_incomplete_struct_pointers -> use_struct_pointers
; call use_struct_pointers -> puts
; call validate_incomplete_var -> strcmp
; item test_block_scope_forward_decl section=.text local=false zero=false
; item test_block_scope_forward_decl@tail1 section=.text local=true zero=false
; item test_block_scope_forward_decl@tail0 section=.text local=true zero=false
; item test_file_scope_forward_decl section=.text local=false zero=false
; item make_struct section=.text local=false zero=false
; item make_struct@run1 section=.text local=true zero=false
; item validate_struct section=.text local=false zero=false
; item test_incomplete_var section=.text local=false zero=false
; item validate_incomplete_var section=.text local=false zero=false
; item print_msg section=.text local=false zero=false
; item test_deref_incomplete_var section=.text local=false zero=false
; item use_struct_pointers section=.text local=false zero=false
; item test_use_incomplete_struct_pointers section=.text local=false zero=false
; item test_use_incomplete_struct_pointers@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item incomplete_var section=.data local=false zero=false
; item @22.flse section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_block_scope_forward_decl:
   SUBROUTINE

   LDA   #$00
   LDY   #$00
   CMP   #$00
   BNE   .test_block_scope_forward_decl@jthread@0
   LDA   #$01
   CMP   #$01
   BNE   .test_block_scope_forward_decl@jthread@0
   TYA
   BNE   .test_block_scope_forward_decl@jthread@0
   LDA   #$02
   CMP   #$02
   BEQ   .if_end@2
.test_block_scope_forward_decl@jthread@0:
   JMP   test_block_scope_forward_decl@tail0
.if_end@2:
   JMP   test_block_scope_forward_decl@tail1

test_block_scope_forward_decl@tail1:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_block_scope_forward_decl@tail0:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_file_scope_forward_decl:
   SUBROUTINE

   JSR   make_struct
   JMP   validate_struct

make_struct:
   SUBROUTINE

   LDA   #$08
   STA   __zpabi_malloc__size_0
   JSR   make_struct@run1
   STA   __local_make_struct__0
   STX   __local_make_struct__1
   STA   DPTR
   STX   DPTR+1
   LDA   #$64
   LDY   #$00
   STA   (__local_make_struct__0),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   __local_make_struct__0
   STA   DPTR
   STX   DPTR+1
   LDA   #$C8
   INY
   STA   (__local_make_struct__0),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   __local_make_struct__0
   RTS

make_struct@run1:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JMP   malloc

validate_struct:
   SUBROUTINE

   STA   __local_validate_struct__ptr
   STX   __local_validate_struct__4
   LDY   #$00
   LDA   (__local_validate_struct__ptr),Y
   STA   __local_validate_struct__3
   INY
   LDA   (__local_validate_struct__ptr),Y
   STA   __local_validate_struct__2
   INY
   LDA   (__local_validate_struct__ptr),Y
   STA   __local_validate_struct__1
   INY
   LDA   (__local_validate_struct__ptr),Y
   BNE   .and_false@3
   LDA   __local_validate_struct__1
   BNE   .and_false@3
   LDA   __local_validate_struct__2
   BNE   .and_false@3
   LDA   __local_validate_struct__3
   CMP   #$64
   BNE   .and_false@3
   INY
   LDA   (__local_validate_struct__ptr),Y
   STA   __local_validate_struct__3
   INY
   LDA   (__local_validate_struct__ptr),Y
   STA   __local_validate_struct__2
   INY
   LDA   (__local_validate_struct__ptr),Y
   STA   __local_validate_struct__1
   INY
   LDA   (__local_validate_struct__ptr),Y
   BNE   .and_false@3
   LDA   __local_validate_struct__1
   BNE   .and_false@3
   LDA   __local_validate_struct__2
   BNE   .and_false@3
   LDA   __local_validate_struct__3
   CMP   #$C8
   BNE   .and_false@3
   LDA   #$01
   LDX   #$00
   RTS
.and_false@3:
   LDA   #$00
   LDX   #$00
   RTS

test_incomplete_var:
   SUBROUTINE

   LDX   #>incomplete_var
   LDA   #<incomplete_var
   JSR   print_msg
   JMP   validate_incomplete_var

validate_incomplete_var:
   SUBROUTINE

   LDX   incomplete_var+1
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDA   incomplete_var
   JSR   strcmp
   STX   __local_validate_incomplete_var__0
   ORA   __local_validate_incomplete_var__0
   BEQ   .if_end@5
   JMP   test_block_scope_forward_decl@tail0
.if_end@5:
   JMP   test_block_scope_forward_decl@tail1

print_msg:
   SUBROUTINE

   STA   __local_print_msg__param
   STX   __local_print_msg__1
   LDY   #$01
   LDA   (__local_print_msg__param),Y
   TAX
   DEY
   LDA   (__local_print_msg__param),Y
   JMP   puts

test_deref_incomplete_var:
   SUBROUTINE

   LDA   #$04
   STA   __zpabi_malloc__size_0
   JSR   make_struct@run1
   STA   __local_test_deref_incomplete_var__1
   STX   __local_test_deref_incomplete_var__0
   CPX   __local_test_deref_incomplete_var__0
   BNE   .thread@0
   CMP   __local_test_deref_incomplete_var__1
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

use_struct_pointers:
   SUBROUTINE

   STX   __local_use_struct_pointers__param_1
   ORA   __local_use_struct_pointers__param_1
   BNE   .if_end@6
   LDX   #>str@2
   LDA   #<str@2
   JSR   puts
.if_end@6:
   JMP   test_block_scope_forward_decl@tail0

test_use_incomplete_struct_pointers:
   SUBROUTINE

   JSR   test_use_incomplete_struct_pointers@run0
   STA   __local_test_use_incomplete_struct_pointers__3
   STX   __local_test_use_incomplete_struct_pointers__4
   JSR   test_use_incomplete_struct_pointers@run0
   STA   __local_test_use_incomplete_struct_pointers__2
   STX   __local_test_use_incomplete_struct_pointers__1
   LDY   #$00
   LDA   (__local_test_use_incomplete_struct_pointers__3),Y
   BNE   .tail@0
   INY
   LDA   (__local_test_use_incomplete_struct_pointers__3),Y
   BNE   .tail@0
   LDA   __local_test_use_incomplete_struct_pointers__3
   ORA   __local_test_use_incomplete_struct_pointers__4
   BEQ   .tail@0
   TXA
   ORA   __local_test_use_incomplete_struct_pointers__2
   BEQ   .tail@0
   LDA   __local_test_use_incomplete_struct_pointers__4
   CMP   __local_test_use_incomplete_struct_pointers__1
   BNE   .if_end@14
   LDA   __local_test_use_incomplete_struct_pointers__3
   CMP   __local_test_use_incomplete_struct_pointers__2
   BEQ   .tail@0
.if_end@14:
   LDA   @22.flse
   ORA   @22.flse+1
   BNE   .cond_end@16
   LDA   __local_test_use_incomplete_struct_pointers__2
   STA   __local_test_use_incomplete_struct_pointers__3
   STX   __local_test_use_incomplete_struct_pointers__4
.cond_end@16:
   LDA   __local_test_use_incomplete_struct_pointers__4
   CMP   __local_test_use_incomplete_struct_pointers__1
   BNE   .tail@0
   LDA   __local_test_use_incomplete_struct_pointers__3
   CMP   __local_test_use_incomplete_struct_pointers__2
   BNE   .tail@0
   LDX   __local_test_use_incomplete_struct_pointers__4
   JSR   use_struct_pointers
   STX   __local_test_use_incomplete_struct_pointers__0
   ORA   __local_test_use_incomplete_struct_pointers__0
   BEQ   .if_end@18
.tail@0:
   JMP   test_block_scope_forward_decl@tail0
.if_end@18:
   JMP   test_block_scope_forward_decl@tail1

test_use_incomplete_struct_pointers@run0:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_calloc__nmemb_0
   LDA   #$04
   STA   __zpabi_calloc__size_0
   LDA   #$00
   STA   __zpabi_calloc__nmemb_1
   STA   __zpabi_calloc__nmemb_2
   STA   __zpabi_calloc__nmemb_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   JMP   calloc

main:
   SUBROUTINE

   JSR   test_block_scope_forward_decl
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@19
   LDX   #$00
   LDA   #$02
   RTS
.if_end@19:
   JSR   test_file_scope_forward_decl
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@20
   LDX   #$00
   LDA   #$03
   RTS
.if_end@20:
   JSR   test_incomplete_var
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@21
   LDX   #$00
   LDA   #$04
   RTS
.if_end@21:
   JSR   test_deref_incomplete_var
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@22
   LDX   #$00
   LDA   #$05
   RTS
.if_end@22:
   JSR   test_use_incomplete_struct_pointers
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@23
   LDX   #$00
   LDA   #$06
   RTS
.if_end@23:
   JMP   test_block_scope_forward_decl@tail0

str@0:
   DC.B  $49, $27, $6D, $20, $61, $20, $73, $74, $72, $75, $63, $74, $21, $00

str@1:
   DC.B  $49, $27, $6D, $20, $61, $20, $73, $74, $72, $75, $63, $74, $21, $00

str@2:
   DC.B  $65, $6D, $70, $74, $79, $20, $70, $6F, $69, $6E, $74, $65, $72, $21, $00

incomplete_var:
   DC.W  str@1

@22.flse:
   DS.B  2
