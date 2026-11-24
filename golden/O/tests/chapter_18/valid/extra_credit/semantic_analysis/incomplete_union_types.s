SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_use_union_pointers__param_1	EQU	$80
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_calloc__size_0	EQU	$84
__zpabi_calloc__size_1	EQU	$85
__zpabi_calloc__size_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87
__local_main__0	EQU	$88
__local_test_use_incomplete_union_pointers__0	EQU	$8A
__local_test_use_incomplete_union_pointers__1	EQU	$8B
__local_test_use_incomplete_union_pointers__2	EQU	$8C
__local_test_use_incomplete_union_pointers__3	EQU	$8D
__local_test_use_incomplete_union_pointers__4	EQU	$8E

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_block_scope_forward_decl params= locals=__local_test_block_scope_forward_decl__val_7,__local_test_block_scope_forward_decl__val_6,__local_test_block_scope_forward_decl__val_5,__local_test_block_scope_forward_decl__val_4,__local_test_block_scope_forward_decl__val_3,__local_test_block_scope_forward_decl__val_2,__local_test_block_scope_forward_decl__val_1,__local_test_block_scope_forward_decl__val_0 indirect=false in_cycle=false
; def test_use_incomplete_union_pointers params= locals=__local_test_use_incomplete_union_pointers__0,__local_test_use_incomplete_union_pointers__1,__local_test_use_incomplete_union_pointers__2,__local_test_use_incomplete_union_pointers__3,__local_test_use_incomplete_union_pointers__4 indirect=false in_cycle=false
; def use_union_pointers params=__zpabi_use_union_pointers__param_0,__zpabi_use_union_pointers__param_1 locals=__local_use_union_pointers__param_1,__local_use_union_pointers__param_0 indirect=false in_cycle=false param_regs=A,X
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext puts params=__zpabi_puts__s_0,__zpabi_puts__s_1 param_regs=A,X
; call main -> test_block_scope_forward_decl
; call main -> test_use_incomplete_union_pointers
; call test_use_incomplete_union_pointers -> calloc
; call test_use_incomplete_union_pointers -> use_union_pointers
; call use_union_pointers -> puts
; item test_block_scope_forward_decl section=.text local=false zero=false
; item test_block_scope_forward_decl@tail0 section=.text local=true zero=false
; item use_union_pointers section=.text local=false zero=false
; item test_use_incomplete_union_pointers section=.text local=false zero=false
; item test_use_incomplete_union_pointers@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item @11.flse section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_block_scope_forward_decl:
   SUBROUTINE

   LDA   #$FF
   CMP   #$FF
   BNE   .test_block_scope_forward_decl@jthread@0
   LDA   #$FA
   CMP   #$FA
   BNE   .test_block_scope_forward_decl@jthread@0
   LDA   #$0A
   CMP   #$0A
   BNE   .test_block_scope_forward_decl@jthread@0
   LDA   #$1F
   CMP   #$1F
   BNE   .test_block_scope_forward_decl@jthread@0
   LDA   #$00
   BNE   .test_block_scope_forward_decl@jthread@0
   BEQ   .if_end@2
.test_block_scope_forward_decl@jthread@0:
   LDX   #$00
   TXA
   RTS
.if_end@2:
   JMP   test_block_scope_forward_decl@tail0

test_block_scope_forward_decl@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

use_union_pointers:
   SUBROUTINE

   STX   __local_use_union_pointers__param_1
   ORA   __local_use_union_pointers__param_1
   BNE   .if_end@3
   LDX   #>str@0
   LDA   #<str@0
   JSR   puts
.if_end@3:
   LDX   #$00
   TXA
   RTS

test_use_incomplete_union_pointers:
   SUBROUTINE

   JSR   test_use_incomplete_union_pointers@run0
   STA   __local_test_use_incomplete_union_pointers__3
   STX   __local_test_use_incomplete_union_pointers__4
   JSR   test_use_incomplete_union_pointers@run0
   STA   __local_test_use_incomplete_union_pointers__2
   STX   __local_test_use_incomplete_union_pointers__1
   LDY   #$00
   LDA   (__local_test_use_incomplete_union_pointers__3),Y
   BNE   .tail@0
   INY
   LDA   (__local_test_use_incomplete_union_pointers__3),Y
   BNE   .tail@0
   LDA   __local_test_use_incomplete_union_pointers__3
   ORA   __local_test_use_incomplete_union_pointers__4
   BEQ   .tail@0
   TXA
   ORA   __local_test_use_incomplete_union_pointers__2
   BEQ   .tail@0
   LDA   __local_test_use_incomplete_union_pointers__4
   CMP   __local_test_use_incomplete_union_pointers__1
   BNE   .if_end@11
   LDA   __local_test_use_incomplete_union_pointers__3
   CMP   __local_test_use_incomplete_union_pointers__2
   BEQ   .tail@0
.if_end@11:
   LDA   @11.flse
   ORA   @11.flse+1
   BNE   .cond_end@13
   LDA   __local_test_use_incomplete_union_pointers__2
   STA   __local_test_use_incomplete_union_pointers__3
   STX   __local_test_use_incomplete_union_pointers__4
.cond_end@13:
   LDA   __local_test_use_incomplete_union_pointers__4
   CMP   __local_test_use_incomplete_union_pointers__1
   BNE   .tail@0
   LDA   __local_test_use_incomplete_union_pointers__3
   CMP   __local_test_use_incomplete_union_pointers__2
   BNE   .tail@0
   LDX   __local_test_use_incomplete_union_pointers__4
   JSR   use_union_pointers
   STX   __local_test_use_incomplete_union_pointers__0
   ORA   __local_test_use_incomplete_union_pointers__0
   BEQ   .if_end@15
.tail@0:
   LDX   #$00
   TXA
   RTS
.if_end@15:
   JMP   test_block_scope_forward_decl@tail0

test_use_incomplete_union_pointers@run0:
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
   BNE   .if_end@16
   JMP   test_block_scope_forward_decl@tail0
.if_end@16:
   JSR   test_use_incomplete_union_pointers
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@17
   LDX   #$00
   LDA   #$02
   RTS
.if_end@17:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $6E, $75, $6C, $6C, $20, $70, $6F, $69, $6E, $74, $65, $72, $00

@11.flse:
   DS.B  2
