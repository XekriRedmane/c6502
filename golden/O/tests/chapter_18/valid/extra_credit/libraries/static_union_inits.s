SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__local_validate_has_union_array__2	EQU	$82
__local_validate_padded_union_array__0	EQU	$82
__local_validate_has_union_array__3	EQU	$83
__local_validate_has_union_array__i_1	EQU	$84
__local_validate_has_union_array__i_0	EQU	$85

; @zp-link-meta-begin
; def validate_has_union params= locals=__local_validate_has_union__0,__local_validate_has_union__1 indirect=false in_cycle=false
; def validate_has_union_array params= locals=__local_validate_has_union_array__0,__local_validate_has_union_array__1,__local_validate_has_union_array__2,__local_validate_has_union_array__3,__local_validate_has_union_array__i_1,__local_validate_has_union_array__i_0,__local_validate_has_union_array__4,__local_validate_has_union_array__5 indirect=false in_cycle=false
; def validate_padded_union_array params= locals=__local_validate_padded_union_array__0,__local_validate_padded_union_array__1 indirect=false in_cycle=false
; def validate_simple params= locals=__local_validate_simple__0,__local_validate_simple__1 indirect=false in_cycle=false
; def validate_uninitialized params= locals=__local_validate_uninitialized__0,__local_validate_uninitialized__1,__local_validate_uninitialized__2,__local_validate_uninitialized__3,__local_validate_uninitialized__4,__local_validate_uninitialized__5,__local_validate_uninitialized__6,__local_validate_uninitialized__7 indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call validate_padded_union_array -> strcmp
; item validate_simple section=.text local=false zero=false
; item validate_has_union section=.text local=false zero=false
; item validate_has_union_array section=.text local=false zero=false
; item validate_has_union_array@tail0 section=.text local=true zero=false
; item validate_uninitialized section=.text local=false zero=false
; item validate_padded_union_array section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

validate_simple:
   SUBROUTINE

   LDA   #$00
   CMP   #$FF
   BNE   .vthread@0
   LDA   s
   CMP   #$D9
   BNE   .and_false@0
   LDA   s+1
   BNE   .and_false@0
   LDA   s
   CMP   #$D9
   BNE   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   LDA   #$00
.vthread@0:
   LDX   #$00
   RTS

validate_has_union:
   SUBROUTINE

   LDA   h
   CMP   #$4D
   BNE   .and_false@2
   LDA   h+1
   BNE   .and_false@2
   LDA   h
   CMP   #$4D
   BNE   .and_false@2
   LDA   #$01
   LDX   #$00
   RTS
.and_false@2:
   LDA   #$00
   LDX   #$00
   RTS

validate_has_union_array:
   SUBROUTINE

   LDA   #$00
   STA   __local_validate_has_union_array__i_0
   LDY   #$00
   STY   __local_validate_has_union_array__i_1
.loop@0_start:
   LDA   __local_validate_has_union_array__i_0
   SEC
   SBC   #$03
   LDA   __local_validate_has_union_array__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   #$61
   CLC
   ADC   __local_validate_has_union_array__i_0
   STA   __local_validate_has_union_array__3
   TYA
   ADC   __local_validate_has_union_array__i_1
   STA   __local_validate_has_union_array__2
   LDA   __local_validate_has_union_array__i_0
   ASL
   ASL
   ASL
   TAX
   CPY   __local_validate_has_union_array__2
   BNE   .validate_has_union_array@jthread@2
   LDA   my_struct,X
   CMP   __local_validate_has_union_array__3
   BNE   .validate_has_union_array@jthread@2
   CPY   __local_validate_has_union_array__2
   BNE   .validate_has_union_array@jthread@2
   CMP   __local_validate_has_union_array__3
   BNE   .validate_has_union_array@jthread@2
   LDA   my_struct+1,X
   CMP   __local_validate_has_union_array__2
   BNE   .validate_has_union_array@jthread@2
   LDA   my_struct,X
   CMP   __local_validate_has_union_array__3
   BNE   .validate_has_union_array@jthread@2
   INC   __local_validate_has_union_array__i_0
   BNE   .loop@0_start
   INC   __local_validate_has_union_array__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   my_struct+24
   ORA   my_struct+25
   ORA   my_struct+26
   ORA   my_struct+27
   ORA   my_struct+28
   ORA   my_struct+29
   ORA   my_struct+30
   ORA   my_struct+31
   BNE   .validate_has_union_array@jthread@2
   LDA   my_struct+32
   CMP   #$23
   BNE   .validate_has_union_array@jthread@2
   LDA   my_struct+33
   CMP   #$21
   BNE   .validate_has_union_array@jthread@2
   LDA   my_struct+34
   BNE   .validate_has_union_array@jthread@2
   LDA   my_struct+33
   CMP   #$21
   BEQ   .if_end@15
.validate_has_union_array@jthread@2:
   LDX   #$00
   TXA
   RTS
.if_end@15:
   JMP   validate_has_union_array@tail0

validate_has_union_array@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

validate_uninitialized:
   SUBROUTINE

   LDA   all_zeros
   ORA   all_zeros+1
   ORA   all_zeros+2
   ORA   all_zeros+3
   ORA   all_zeros+4
   ORA   all_zeros+5
   ORA   all_zeros+6
   ORA   all_zeros+7
   BEQ   .if_end@16
   LDX   #$00
   TXA
   RTS
.if_end@16:
   JMP   validate_has_union_array@tail0

validate_padded_union_array:
   SUBROUTINE

   LDX   #>padded_union_array
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDA   #<padded_union_array
   JSR   strcmp
   STX   __local_validate_padded_union_array__0
   ORA   __local_validate_padded_union_array__0
   BNE   .tail@0
   LDX   #>(padded_union_array+13)
   LDA   #<str@1
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@1
   STA   __zpabi_strcmp__s2_1
   LDA   #<(padded_union_array+13)
   JSR   strcmp
   STX   __local_validate_padded_union_array__0
   ORA   __local_validate_padded_union_array__0
   BNE   .tail@0
   LDX   #>(padded_union_array+26)
   LDA   #<str@2
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@2
   STA   __zpabi_strcmp__s2_1
   LDA   #<(padded_union_array+26)
   JSR   strcmp
   STX   __local_validate_padded_union_array__0
   ORA   __local_validate_padded_union_array__0
   BEQ   .if_end@19
.tail@0:
   LDX   #$00
   TXA
   RTS
.if_end@19:
   JMP   validate_has_union_array@tail0

str@0:
   DC.B  $66, $69, $72, $73, $74, $20, $73, $74, $72, $69, $6E, $67, $00

str@1:
   DC.B  $73, $74, $72, $69, $6E, $67, $20, $23, $32, $00

str@2:
   DC.B  $73, $74, $72, $69, $6E, $67, $20, $23, $33, $00
