SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_validate_struct_param__p_0	EQU	$80
__zpabi_validate_struct_param__p_1	EQU	$81
__zpabi_validate_struct_param__p_2	EQU	$82
__zpabi_validate_struct_param__p_3	EQU	$83
__local_main__0	EQU	$84

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; ext validate_struct_param params=__zpabi_validate_struct_param__p_0,__zpabi_validate_struct_param__p_1,__zpabi_validate_struct_param__p_2,__zpabi_validate_struct_param__p_3
; call main -> validate_struct_param
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_validate_struct_param__p_0
   LDA   #$00
   STA   __zpabi_validate_struct_param__p_1
   LDY   #$02
   STY   __zpabi_validate_struct_param__p_2
   STA   __zpabi_validate_struct_param__p_3
   JSR   validate_struct_param
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$00
   TXA
   RTS
