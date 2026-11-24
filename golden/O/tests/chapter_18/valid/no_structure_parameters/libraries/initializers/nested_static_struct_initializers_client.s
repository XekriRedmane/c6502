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
; ext test_array_of_structs params=
; ext test_fully_intialized params=
; ext test_implicit_conversions params=
; ext test_partially_initialized params=
; ext test_uninitialized params=
; call main -> test_array_of_structs
; call main -> test_fully_intialized
; call main -> test_implicit_conversions
; call main -> test_partially_initialized
; call main -> test_uninitialized
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item all_zeros section=.bss local=false zero=true
; item partial section=.data local=false zero=false
; item full section=.data local=false zero=false
; item converted section=.data local=false zero=false
; item struct_array section=.data local=false zero=false
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
   JSR   test_fully_intialized
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   test_implicit_conversions
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   test_array_of_structs
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $48, $65, $6C, $6C, $6F, $21, $00

str@1:
   DC.B  $41, $6E, $6F, $74, $68, $65, $72, $20, $6D, $65, $73, $73, $61, $67, $65, $00

str@2:
   DC.B  $4D, $65, $73, $73, $61, $67, $65, $00

all_zeros:
   DS.B  21

partial:
   DC.L  $00000064
   DC.W  $000A
   DC.B  $0A, $00, $00
   DS.B  2
   DC.W  str@0
   DS.B  8

full:
   DC.L  $FFFFFFFB
   DC.W  $03E8
   DC.B  $6F, $6B, $00
   DS.B  2
   DC.W  str@1
   DC.L  $A2000000
   DC.L  $427D1A94

converted:
   DC.L  $0000000A
   DC.W  $0002
   DC.B  $0F, $03, $81
   DS.B  4
   DC.L  $00000001
   DC.L  $43E00000

struct_array:
   DC.L  $00000001
   DC.W  $0002
   DC.B  $61, $62, $00
   DC.W  $0003
   DS.B  2
   DC.L  $00000000
   DC.L  $40140000
   DC.L  $00000006
   DC.W  $0007
   DC.B  $63, $64, $00
   DC.W  $0008
   DC.W  str@2
   DC.L  $00000000
   DC.L  $40220000
   DS.B  21
