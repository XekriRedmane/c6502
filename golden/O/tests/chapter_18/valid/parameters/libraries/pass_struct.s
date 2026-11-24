SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_validate_struct_param__p_0	EQU	$80
__zpabi_validate_struct_param__p_1	EQU	$81
__zpabi_validate_struct_param__p_2	EQU	$82
__zpabi_validate_struct_param__p_3	EQU	$83
__local_validate_struct_param__1	EQU	$85

; @zp-link-meta-begin
; def validate_struct_param params=__zpabi_validate_struct_param__p_0,__zpabi_validate_struct_param__p_1,__zpabi_validate_struct_param__p_2,__zpabi_validate_struct_param__p_3 locals=__local_validate_struct_param__0,__local_validate_struct_param__1,__local_validate_struct_param__2,__local_validate_struct_param__3 indirect=false in_cycle=false changes=__zpabi_validate_struct_param__p_0,__zpabi_validate_struct_param__p_1,__zpabi_validate_struct_param__p_2,__zpabi_validate_struct_param__p_3
; item validate_struct_param section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

validate_struct_param:
   SUBROUTINE

   LDA   __zpabi_validate_struct_param__p_0
   STA   __local_validate_struct_param__1
   LDA   __zpabi_validate_struct_param__p_0+1
   BNE   .validate_struct_param@jthread@0
   LDA   __local_validate_struct_param__1
   CMP   #$01
   BNE   .validate_struct_param@jthread@0
   LDA   __zpabi_validate_struct_param__p_0+2
   STA   __local_validate_struct_param__1
   LDA   __zpabi_validate_struct_param__p_0+3
   BNE   .validate_struct_param@jthread@0
   LDA   __local_validate_struct_param__1
   CMP   #$02
   BEQ   .if_end@2
.validate_struct_param@jthread@0:
   LDX   #$00
   TXA
   RTS
.if_end@2:
   LDX   #$00
   LDA   #$01
   RTS
