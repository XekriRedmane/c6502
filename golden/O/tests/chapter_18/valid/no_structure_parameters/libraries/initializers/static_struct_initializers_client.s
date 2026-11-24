SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; ext test_implicit_conversion params=
; ext test_partial_inner_init params=
; ext test_partially_initialized params=
; ext test_uninitialized params=
; call main -> test_implicit_conversion
; call main -> test_partial_inner_init
; call main -> test_partially_initialized
; call main -> test_uninitialized
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item uninitialized section=.bss local=false zero=true
; item partial section=.data local=false zero=false
; item partial_with_array section=.data local=false zero=false
; item converted section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   JSR   test_uninitialized
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   test_partially_initialized
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   test_partial_inner_init
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   test_implicit_conversion
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $48, $65, $6C, $6C, $6F, $00

str@1:
   DC.B  $21, $00

uninitialized:
   DS.B  15

partial:
   DC.L  $00000000
   DC.L  $3FF00000
   DC.W  str@0
   DS.B  5

partial_with_array:
   DC.L  $00000000
   DC.L  $40080000
   DC.W  str@1
   DC.B  $01, $00, $00
   DC.W  $0002

converted:
   DC.L  $00000000
   DC.L  $43B00000
   DS.B  2
   DC.B  $61, $62, $63
   DC.W  $0005
