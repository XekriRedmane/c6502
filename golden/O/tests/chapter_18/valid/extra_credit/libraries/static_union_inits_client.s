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
; ext validate_has_union params=
; ext validate_has_union_array params=
; ext validate_padded_union_array params=
; ext validate_simple params=
; ext validate_uninitialized params=
; call main -> validate_has_union
; call main -> validate_has_union_array
; call main -> validate_padded_union_array
; call main -> validate_simple
; call main -> validate_uninitialized
; item main section=.text local=false zero=false
; item s section=.data local=false zero=false
; item h section=.data local=false zero=false
; item my_struct section=.data local=false zero=false
; item all_zeros section=.bss local=false zero=true
; item padded_union_array section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   JSR   validate_simple
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   validate_has_union
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   validate_has_union_array
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   validate_uninitialized
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   validate_padded_union_array
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

s:
   DC.W  $00D9
   DS.B  6

h:
   DC.W  $004D
   DS.B  6

my_struct:
   DC.W  $0061
   DS.B  6
   DC.W  $0062
   DS.B  6
   DC.W  $0063
   DS.B  14
   DC.B  $23
   DC.W  $0021
   DS.B  6

all_zeros:
   DS.B  8

padded_union_array:
   DC.B  $66, $69, $72, $73, $74, $20, $73, $74, $72, $69, $6E, $67, $00
   DC.B  $73, $74, $72, $69, $6E, $67, $20, $23, $32, $00, $00, $00, $00
   DC.B  $73, $74, $72, $69, $6E, $67, $20, $23, $33, $00, $00, $00, $00
