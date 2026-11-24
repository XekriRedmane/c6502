SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__local_validate_array_of_structs__0	EQU	$82
__local_validate_full_initialization__0	EQU	$82
__local_validate_mixed_initialization__0	EQU	$82
__local_validate_partial_initialization__0	EQU	$82
__local_validate_array_of_structs__1	EQU	$83
__local_validate_full_initialization__1	EQU	$83
__local_validate_mixed_initialization__1	EQU	$83
__local_validate_partial_initialization__1	EQU	$83
__local_validate_array_of_structs__2	EQU	$84
__local_validate_full_initialization__2	EQU	$84
__local_validate_mixed_initialization__2	EQU	$84
__local_validate_partial_initialization__2	EQU	$84
__local_validate_array_of_structs__3	EQU	$85
__local_validate_full_initialization__3	EQU	$85
__local_validate_mixed_initialization__3	EQU	$85
__local_validate_partial_initialization__3	EQU	$85
__local_validate_array_of_structs__4	EQU	$86
__local_validate_full_initialization__4	EQU	$86
__local_validate_mixed_initialization__4	EQU	$86
__local_validate_partial_initialization__4	EQU	$86
__local_validate_array_of_structs__5	EQU	$87
__local_validate_full_initialization__5	EQU	$87
__local_validate_mixed_initialization__5	EQU	$87
__local_validate_partial_initialization__5	EQU	$87
__local_validate_array_of_structs__6	EQU	$88
__local_validate_full_initialization__6	EQU	$88
__local_validate_mixed_initialization__6	EQU	$88
__local_validate_partial_initialization__6	EQU	$88
__local_validate_array_of_structs__7	EQU	$89
__local_validate_full_initialization__7	EQU	$89
__local_validate_mixed_initialization__7	EQU	$89
__local_validate_array_of_structs__struct_array	EQU	$8A
__local_validate_full_initialization__ptr	EQU	$8A
__local_validate_mixed_initialization__8	EQU	$8A
__local_validate_partial_initialization__ptr	EQU	$8A
__local_validate_array_of_structs__8	EQU	$8B
__local_validate_full_initialization__8	EQU	$8B
__local_validate_mixed_initialization__9	EQU	$8B
__local_validate_partial_initialization__8	EQU	$8B
__local_validate_mixed_initialization__ptr	EQU	$8C
__local_validate_mixed_initialization__10	EQU	$8D

; @zp-link-meta-begin
; def validate_array_of_structs params=__zpabi_validate_array_of_structs__struct_array_0,__zpabi_validate_array_of_structs__struct_array_1 locals=__local_validate_array_of_structs__0,__local_validate_array_of_structs__1,__local_validate_array_of_structs__2,__local_validate_array_of_structs__3,__local_validate_array_of_structs__4,__local_validate_array_of_structs__5,__local_validate_array_of_structs__6,__local_validate_array_of_structs__7,__local_validate_array_of_structs__struct_array,__local_validate_array_of_structs__8 indirect=false in_cycle=false param_regs=A,X
; def validate_full_initialization params=__zpabi_validate_full_initialization__ptr_0,__zpabi_validate_full_initialization__ptr_1 locals=__local_validate_full_initialization__0,__local_validate_full_initialization__1,__local_validate_full_initialization__2,__local_validate_full_initialization__3,__local_validate_full_initialization__4,__local_validate_full_initialization__5,__local_validate_full_initialization__6,__local_validate_full_initialization__7,__local_validate_full_initialization__ptr,__local_validate_full_initialization__8 indirect=false in_cycle=false param_regs=A,X
; def validate_mixed_initialization params=__zpabi_validate_mixed_initialization__ptr_0,__zpabi_validate_mixed_initialization__ptr_1 locals=__local_validate_mixed_initialization__0,__local_validate_mixed_initialization__1,__local_validate_mixed_initialization__2,__local_validate_mixed_initialization__3,__local_validate_mixed_initialization__4,__local_validate_mixed_initialization__5,__local_validate_mixed_initialization__6,__local_validate_mixed_initialization__7,__local_validate_mixed_initialization__8,__local_validate_mixed_initialization__9,__local_validate_mixed_initialization__ptr,__local_validate_mixed_initialization__10 indirect=false in_cycle=false param_regs=A,X
; def validate_partial_initialization params=__zpabi_validate_partial_initialization__ptr_0,__zpabi_validate_partial_initialization__ptr_1 locals=__local_validate_partial_initialization__0,__local_validate_partial_initialization__1,__local_validate_partial_initialization__2,__local_validate_partial_initialization__3,__local_validate_partial_initialization__4,__local_validate_partial_initialization__5,__local_validate_partial_initialization__6,__local_validate_partial_initialization__7,__local_validate_partial_initialization__ptr,__local_validate_partial_initialization__8,__local_validate_partial_initialization__9,__local_validate_partial_initialization__10 indirect=false in_cycle=false param_regs=A,X
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call validate_array_of_structs -> strcmp
; call validate_full_initialization -> strcmp
; call validate_mixed_initialization -> strcmp
; call validate_partial_initialization -> strcmp
; item validate_full_initialization section=.text local=false zero=false
; item validate_full_initialization@tail0 section=.text local=true zero=false
; item validate_partial_initialization section=.text local=false zero=false
; item validate_mixed_initialization section=.text local=false zero=false
; item validate_array_of_structs section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item str@3 section=.rodata local=true zero=false
; item str@4 section=.rodata local=true zero=false
; item str@5 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

validate_full_initialization:
   SUBROUTINE

   STA   __local_validate_full_initialization__ptr
   STX   __local_validate_full_initialization__8
   LDY   #$00
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__3
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__2
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__1
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   CMP   #$FF
   BEQ   .lb_skip@11
   JMP   .validate_full_initialization@jthread@8
.lb_skip@11:
   LDA   __local_validate_full_initialization__1
   CMP   #$FF
   BEQ   .lb_skip@10
   JMP   .validate_full_initialization@jthread@8
.lb_skip@10:
   LDA   __local_validate_full_initialization__2
   CMP   #$FF
   BEQ   .lb_skip@9
   JMP   .validate_full_initialization@jthread@8
.lb_skip@9:
   LDA   __local_validate_full_initialization__3
   CMP   #$38
   BEQ   .lb_skip@8
   JMP   .validate_full_initialization@jthread@8
.lb_skip@8:
   LDY   #$04
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__1
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   CMP   #$FF
   BEQ   .lb_skip@7
   JMP   .validate_full_initialization@jthread@8
.lb_skip@7:
   LDA   __local_validate_full_initialization__1
   CMP   #$55
   BEQ   .lb_skip@6
   JMP   .validate_full_initialization@jthread@8
.lb_skip@6:
   LDY   #$06
   LDA   (__local_validate_full_initialization__ptr),Y
   CMP   #$C8
   BEQ   .lb_skip@5
   JMP   .validate_full_initialization@jthread@8
.lb_skip@5:
   LDY   #$07
   LDA   (__local_validate_full_initialization__ptr),Y
   CMP   #$CA
   BEQ   .lb_skip@4
   JMP   .validate_full_initialization@jthread@8
.lb_skip@4:
   LDY   #$08
   LDA   (__local_validate_full_initialization__ptr),Y
   CMP   #$CB
   BEQ   .lb_skip@3
   JMP   .validate_full_initialization@jthread@8
.lb_skip@3:
   LDY   #$09
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__1
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   BEQ   .lb_skip@2
   JMP   .validate_full_initialization@jthread@8
.lb_skip@2:
   LDA   __local_validate_full_initialization__1
   CMP   #$28
   BEQ   .lb_skip@1
   JMP   .validate_full_initialization@jthread@8
.lb_skip@1:
   LDY   #$0B
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__0
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   TAX
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDA   __local_validate_full_initialization__0
   JSR   strcmp
   STX   __local_validate_full_initialization__0
   ORA   __local_validate_full_initialization__0
   BEQ   .lb_skip@0
   JMP   .validate_full_initialization@jthread@8
.lb_skip@0:
   LDY   #$0D
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__7
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__6
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__5
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__4
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__3
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__2
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__1
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   CMP   #$C0
   BNE   .validate_full_initialization@jthread@8
   LDA   __local_validate_full_initialization__1
   CMP   #$36
   BNE   .validate_full_initialization@jthread@8
   LDA   __local_validate_full_initialization__2
   BNE   .validate_full_initialization@jthread@8
   LDA   __local_validate_full_initialization__3
   BNE   .validate_full_initialization@jthread@8
   LDA   __local_validate_full_initialization__4
   BNE   .validate_full_initialization@jthread@8
   LDA   __local_validate_full_initialization__5
   BNE   .validate_full_initialization@jthread@8
   LDA   __local_validate_full_initialization__6
   BNE   .validate_full_initialization@jthread@8
   LDA   __local_validate_full_initialization__7
   BNE   .validate_full_initialization@jthread@8
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__1
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   BNE   .validate_full_initialization@jthread@8
   LDA   __local_validate_full_initialization__1
   CMP   #$01
   BNE   .validate_full_initialization@jthread@8
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__1
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   BNE   .validate_full_initialization@jthread@8
   LDA   __local_validate_full_initialization__1
   CMP   #$02
   BEQ   .if_end@18
.validate_full_initialization@jthread@8:
   LDX   #$00
   TXA
   RTS
.if_end@18:
   JMP   validate_full_initialization@tail0

validate_full_initialization@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

validate_partial_initialization:
   SUBROUTINE

   STA   __local_validate_partial_initialization__ptr
   STX   __local_validate_partial_initialization__8
   LDY   #$00
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__3
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__2
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__1
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   BNE   .validate_partial_initialization@jthread@1
   LDA   __local_validate_partial_initialization__1
   BNE   .validate_partial_initialization@jthread@1
   LDA   __local_validate_partial_initialization__2
   CMP   #$03
   BNE   .validate_partial_initialization@jthread@1
   LDA   __local_validate_partial_initialization__3
   CMP   #$E8
   BNE   .validate_partial_initialization@jthread@1
   LDA   __local_validate_partial_initialization__ptr
   CLC
   ADC   #$04
   STA   __local_validate_partial_initialization__2
   TXA
   ADC   #$00
   STA   __local_validate_partial_initialization__3
   LDY   #$00
   LDA   (__local_validate_partial_initialization__2),Y
   STA   __local_validate_partial_initialization__1
   INY
   LDA   (__local_validate_partial_initialization__2),Y
   BNE   .validate_partial_initialization@jthread@1
   LDA   __local_validate_partial_initialization__1
   CMP   #$01
   BNE   .validate_partial_initialization@jthread@1
   LDY   #$0B
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__0
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   TAX
   LDA   #<str@1
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@1
   STA   __zpabi_strcmp__s2_1
   LDA   __local_validate_partial_initialization__0
   JSR   strcmp
   STX   __local_validate_partial_initialization__0
   ORA   __local_validate_partial_initialization__0
   BEQ   .if_end@23
.validate_partial_initialization@jthread@1:
   LDX   #$00
   TXA
   RTS
.if_end@23:
   LDY   #$02
   LDA   (__local_validate_partial_initialization__2),Y
   BNE   .validate_partial_initialization@jthread@7
   INY
   LDA   (__local_validate_partial_initialization__2),Y
   BNE   .validate_partial_initialization@jthread@7
   INY
   LDA   (__local_validate_partial_initialization__2),Y
   BNE   .validate_partial_initialization@jthread@7
   LDY   #$06
   LDA   (__local_validate_partial_initialization__2),Y
   DEY
   ORA   (__local_validate_partial_initialization__2),Y
   BNE   .validate_partial_initialization@jthread@7
   LDY   #$0E
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__6
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__5
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__4
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__3
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__2
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__1
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__0
   LDY   #$0D
   LDA   (__local_validate_partial_initialization__ptr),Y
   ORA   __local_validate_partial_initialization__6
   ORA   __local_validate_partial_initialization__5
   ORA   __local_validate_partial_initialization__4
   ORA   __local_validate_partial_initialization__3
   ORA   __local_validate_partial_initialization__2
   ORA   __local_validate_partial_initialization__1
   ORA   __local_validate_partial_initialization__0
   BNE   .validate_partial_initialization@jthread@7
   LDY   #$16
   LDA   (__local_validate_partial_initialization__ptr),Y
   DEY
   ORA   (__local_validate_partial_initialization__ptr),Y
   BNE   .validate_partial_initialization@jthread@7
   LDY   #$18
   LDA   (__local_validate_partial_initialization__ptr),Y
   DEY
   ORA   (__local_validate_partial_initialization__ptr),Y
   BEQ   .if_end@36
.validate_partial_initialization@jthread@7:
   LDX   #$00
   TXA
   RTS
.if_end@36:
   JMP   validate_full_initialization@tail0

validate_mixed_initialization:
   SUBROUTINE

   STA   __local_validate_mixed_initialization__ptr
   STX   __local_validate_mixed_initialization__10
   LDY   #$00
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__3
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__2
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__1
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   BEQ   .lb_skip@21
   JMP   .validate_mixed_initialization@jthread@6
.lb_skip@21:
   LDA   __local_validate_mixed_initialization__1
   BEQ   .lb_skip@20
   JMP   .validate_mixed_initialization@jthread@6
.lb_skip@20:
   LDA   __local_validate_mixed_initialization__2
   BEQ   .lb_skip@19
   JMP   .validate_mixed_initialization@jthread@6
.lb_skip@19:
   LDA   __local_validate_mixed_initialization__3
   CMP   #$C8
   BEQ   .lb_skip@18
   JMP   .validate_mixed_initialization@jthread@6
.lb_skip@18:
   LDA   __local_validate_mixed_initialization__ptr
   CLC
   ADC   #$04
   STA   __local_validate_mixed_initialization__8
   TXA
   ADC   #$00
   STA   __local_validate_mixed_initialization__9
   LDY   #$00
   LDA   (__local_validate_mixed_initialization__8),Y
   STA   __local_validate_mixed_initialization__1
   INY
   LDA   (__local_validate_mixed_initialization__8),Y
   BEQ   .lb_skip@17
   JMP   .validate_mixed_initialization@jthread@6
.lb_skip@17:
   LDA   __local_validate_mixed_initialization__1
   CMP   #$14
   BEQ   .lb_skip@16
   JMP   .validate_mixed_initialization@jthread@6
.lb_skip@16:
   LDY   #$06
   LDA   (__local_validate_mixed_initialization__ptr),Y
   CMP   #$15
   BEQ   .lb_skip@15
   JMP   .validate_mixed_initialization@jthread@6
.lb_skip@15:
   LDY   #$09
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__1
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   BEQ   .lb_skip@14
   JMP   .validate_mixed_initialization@jthread@6
.lb_skip@14:
   LDA   __local_validate_mixed_initialization__1
   CMP   #$16
   BEQ   .lb_skip@13
   JMP   .validate_mixed_initialization@jthread@6
.lb_skip@13:
   LDY   #$0B
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__0
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   TAX
   LDA   #<str@2
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@2
   STA   __zpabi_strcmp__s2_1
   LDA   __local_validate_mixed_initialization__0
   JSR   strcmp
   STX   __local_validate_mixed_initialization__0
   ORA   __local_validate_mixed_initialization__0
   BEQ   .lb_skip@12
   JMP   .validate_mixed_initialization@jthread@6
.lb_skip@12:
   LDY   #$0D
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__7
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__6
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__5
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__4
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__3
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__2
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__1
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   CMP   #$40
   BNE   .validate_mixed_initialization@jthread@7
   LDA   __local_validate_mixed_initialization__1
   CMP   #$24
   BNE   .validate_mixed_initialization@jthread@7
   LDA   __local_validate_mixed_initialization__2
   BNE   .validate_mixed_initialization@jthread@7
   LDA   __local_validate_mixed_initialization__3
   BNE   .validate_mixed_initialization@jthread@7
   LDA   __local_validate_mixed_initialization__4
   BNE   .validate_mixed_initialization@jthread@7
   LDA   __local_validate_mixed_initialization__5
   BNE   .validate_mixed_initialization@jthread@7
   LDA   __local_validate_mixed_initialization__6
   BNE   .validate_mixed_initialization@jthread@7
   LDA   __local_validate_mixed_initialization__7
   BNE   .validate_mixed_initialization@jthread@7
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__1
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   BNE   .validate_mixed_initialization@jthread@7
   LDA   __local_validate_mixed_initialization__1
   CMP   #$63
   BNE   .validate_mixed_initialization@jthread@7
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   STA   __local_validate_mixed_initialization__1
   INY
   LDA   (__local_validate_mixed_initialization__ptr),Y
   BNE   .validate_mixed_initialization@jthread@7
   LDA   __local_validate_mixed_initialization__1
   CMP   #$64
   BEQ   .if_end@51
.validate_mixed_initialization@jthread@6:
   LDX   #$00
   TXA
   RTS
.if_end@51:
   LDY   #$03
   LDA   (__local_validate_mixed_initialization__8),Y
   BNE   .validate_mixed_initialization@jthread@7
   INY
   LDA   (__local_validate_mixed_initialization__8),Y
   BEQ   .if_end@54
.validate_mixed_initialization@jthread@7:
   LDX   #$00
   TXA
   RTS
.if_end@54:
   JMP   validate_full_initialization@tail0

validate_array_of_structs:
   SUBROUTINE

   STA   __local_validate_array_of_structs__struct_array
   STX   __local_validate_array_of_structs__8
   LDY   #$00
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__3
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__2
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@61
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@61:
   LDA   __local_validate_array_of_structs__1
   BEQ   .lb_skip@60
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@60:
   LDA   __local_validate_array_of_structs__2
   BEQ   .lb_skip@59
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@59:
   LDA   __local_validate_array_of_structs__3
   CMP   #$01
   BEQ   .lb_skip@58
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@58:
   LDY   #$04
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@57
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@57:
   LDA   __local_validate_array_of_structs__1
   CMP   #$02
   BEQ   .lb_skip@56
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@56:
   LDY   #$06
   LDA   (__local_validate_array_of_structs__struct_array),Y
   CMP   #$03
   BEQ   .lb_skip@55
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@55:
   LDY   #$07
   LDA   (__local_validate_array_of_structs__struct_array),Y
   CMP   #$04
   BEQ   .lb_skip@54
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@54:
   LDY   #$08
   LDA   (__local_validate_array_of_structs__struct_array),Y
   CMP   #$05
   BEQ   .lb_skip@53
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@53:
   LDY   #$09
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@52
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@52:
   LDA   __local_validate_array_of_structs__1
   CMP   #$06
   BEQ   .lb_skip@51
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@51:
   LDY   #$0B
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__0
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   TAX
   LDA   #<str@3
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@3
   STA   __zpabi_strcmp__s2_1
   LDA   __local_validate_array_of_structs__0
   JSR   strcmp
   STX   __local_validate_array_of_structs__0
   ORA   __local_validate_array_of_structs__0
   BEQ   .lb_skip@50
   JMP   .validate_array_of_structs@jthread@8
.lb_skip@50:
   LDY   #$0D
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__7
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__6
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__5
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__4
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__3
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__2
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   CMP   #$40
   BNE   .validate_array_of_structs@jthread@8
   LDA   __local_validate_array_of_structs__1
   CMP   #$20
   BNE   .validate_array_of_structs@jthread@8
   LDA   __local_validate_array_of_structs__2
   BNE   .validate_array_of_structs@jthread@8
   LDA   __local_validate_array_of_structs__3
   BNE   .validate_array_of_structs@jthread@8
   LDA   __local_validate_array_of_structs__4
   BNE   .validate_array_of_structs@jthread@8
   LDA   __local_validate_array_of_structs__5
   BNE   .validate_array_of_structs@jthread@8
   LDA   __local_validate_array_of_structs__6
   BNE   .validate_array_of_structs@jthread@8
   LDA   __local_validate_array_of_structs__7
   BNE   .validate_array_of_structs@jthread@8
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BNE   .validate_array_of_structs@jthread@8
   LDA   __local_validate_array_of_structs__1
   CMP   #$09
   BNE   .validate_array_of_structs@jthread@8
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BNE   .validate_array_of_structs@jthread@8
   LDA   __local_validate_array_of_structs__1
   CMP   #$0A
   BEQ   .if_end@73
.validate_array_of_structs@jthread@8:
   LDX   #$00
   TXA
   RTS
.if_end@73:
   LDY   #$19
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__3
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__2
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@49
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@49:
   LDA   __local_validate_array_of_structs__1
   BEQ   .lb_skip@48
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@48:
   LDA   __local_validate_array_of_structs__2
   BEQ   .lb_skip@47
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@47:
   LDA   __local_validate_array_of_structs__3
   CMP   #$65
   BEQ   .lb_skip@46
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@46:
   LDY   #$1D
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@45
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@45:
   LDA   __local_validate_array_of_structs__1
   CMP   #$66
   BEQ   .lb_skip@44
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@44:
   LDY   #$1F
   LDA   (__local_validate_array_of_structs__struct_array),Y
   CMP   #$67
   BEQ   .lb_skip@43
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@43:
   LDY   #$20
   LDA   (__local_validate_array_of_structs__struct_array),Y
   CMP   #$68
   BEQ   .lb_skip@42
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@42:
   LDY   #$21
   LDA   (__local_validate_array_of_structs__struct_array),Y
   CMP   #$69
   BEQ   .lb_skip@41
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@41:
   LDY   #$22
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@40
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@40:
   LDA   __local_validate_array_of_structs__1
   CMP   #$6A
   BEQ   .lb_skip@39
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@39:
   LDY   #$24
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__0
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   TAX
   LDA   #<str@4
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@4
   STA   __zpabi_strcmp__s2_1
   LDA   __local_validate_array_of_structs__0
   JSR   strcmp
   STX   __local_validate_array_of_structs__0
   ORA   __local_validate_array_of_structs__0
   BEQ   .lb_skip@38
   JMP   .validate_array_of_structs@jthread@17
.lb_skip@38:
   LDY   #$26
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__7
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__6
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__5
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__4
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__3
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__2
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   CMP   #$40
   BNE   .validate_array_of_structs@jthread@17
   LDA   __local_validate_array_of_structs__1
   CMP   #$5B
   BNE   .validate_array_of_structs@jthread@17
   LDA   __local_validate_array_of_structs__2
   BNE   .validate_array_of_structs@jthread@17
   LDA   __local_validate_array_of_structs__3
   BNE   .validate_array_of_structs@jthread@17
   LDA   __local_validate_array_of_structs__4
   BNE   .validate_array_of_structs@jthread@17
   LDA   __local_validate_array_of_structs__5
   BNE   .validate_array_of_structs@jthread@17
   LDA   __local_validate_array_of_structs__6
   BNE   .validate_array_of_structs@jthread@17
   LDA   __local_validate_array_of_structs__7
   BNE   .validate_array_of_structs@jthread@17
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BNE   .validate_array_of_structs@jthread@17
   LDA   __local_validate_array_of_structs__1
   CMP   #$6D
   BNE   .validate_array_of_structs@jthread@17
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BNE   .validate_array_of_structs@jthread@17
   LDA   __local_validate_array_of_structs__1
   CMP   #$6E
   BEQ   .if_end@92
.validate_array_of_structs@jthread@17:
   LDX   #$00
   TXA
   RTS
.if_end@92:
   LDY   #$32
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__3
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__2
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@37
   JMP   .validate_array_of_structs@jthread@26
.lb_skip@37:
   LDA   __local_validate_array_of_structs__1
   BEQ   .lb_skip@36
   JMP   .validate_array_of_structs@jthread@26
.lb_skip@36:
   LDA   __local_validate_array_of_structs__2
   BEQ   .lb_skip@35
   JMP   .validate_array_of_structs@jthread@26
.lb_skip@35:
   LDA   __local_validate_array_of_structs__3
   CMP   #$C9
   BEQ   .lb_skip@34
   JMP   .validate_array_of_structs@jthread@26
.lb_skip@34:
   LDY   #$36
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@33
   JMP   .validate_array_of_structs@jthread@26
.lb_skip@33:
   LDA   __local_validate_array_of_structs__1
   CMP   #$CA
   BEQ   .lb_skip@32
   JMP   .validate_array_of_structs@jthread@26
.lb_skip@32:
   LDY   #$38
   LDA   (__local_validate_array_of_structs__struct_array),Y
   CMP   #$CB
   BEQ   .lb_skip@31
   JMP   .validate_array_of_structs@jthread@26
.lb_skip@31:
   LDY   #$39
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@30
   JMP   .validate_array_of_structs@jthread@26
.lb_skip@30:
   LDY   #$3A
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@29
   JMP   .validate_array_of_structs@jthread@26
.lb_skip@29:
   LDY   #$3C
   LDA   (__local_validate_array_of_structs__struct_array),Y
   DEY
   ORA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@28
   JMP   .validate_array_of_structs@jthread@26
.lb_skip@28:
   LDY   #$3D
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__0
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   TAX
   LDA   #<str@5
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@5
   STA   __zpabi_strcmp__s2_1
   LDA   __local_validate_array_of_structs__0
   JSR   strcmp
   STX   __local_validate_array_of_structs__0
   ORA   __local_validate_array_of_structs__0
   BNE   .validate_array_of_structs@jthread@26
   LDY   #$3F
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__7
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__6
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__5
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__4
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__3
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__2
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   CMP   #$40
   BNE   .validate_array_of_structs@jthread@26
   LDA   __local_validate_array_of_structs__1
   CMP   #$6A
   BNE   .validate_array_of_structs@jthread@26
   LDA   __local_validate_array_of_structs__2
   BNE   .validate_array_of_structs@jthread@26
   LDA   __local_validate_array_of_structs__3
   BNE   .validate_array_of_structs@jthread@26
   LDA   __local_validate_array_of_structs__4
   BNE   .validate_array_of_structs@jthread@26
   LDA   __local_validate_array_of_structs__5
   BNE   .validate_array_of_structs@jthread@26
   LDA   __local_validate_array_of_structs__6
   BNE   .validate_array_of_structs@jthread@26
   LDA   __local_validate_array_of_structs__7
   BNE   .validate_array_of_structs@jthread@26
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BNE   .validate_array_of_structs@jthread@26
   LDA   __local_validate_array_of_structs__1
   CMP   #$D1
   BNE   .validate_array_of_structs@jthread@26
   LDY   #$4A
   LDA   (__local_validate_array_of_structs__struct_array),Y
   DEY
   ORA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .if_end@111
.validate_array_of_structs@jthread@26:
   LDX   #$00
   TXA
   RTS
.if_end@111:
   LDY   #$4B
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__3
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__2
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@27
   JMP   .validate_array_of_structs@jthread@35
.lb_skip@27:
   LDA   __local_validate_array_of_structs__1
   BEQ   .lb_skip@26
   JMP   .validate_array_of_structs@jthread@35
.lb_skip@26:
   LDA   __local_validate_array_of_structs__2
   CMP   #$01
   BEQ   .lb_skip@25
   JMP   .validate_array_of_structs@jthread@35
.lb_skip@25:
   LDA   __local_validate_array_of_structs__3
   CMP   #$2D
   BEQ   .lb_skip@24
   JMP   .validate_array_of_structs@jthread@35
.lb_skip@24:
   LDY   #$50
   LDA   (__local_validate_array_of_structs__struct_array),Y
   DEY
   ORA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@23
   JMP   .validate_array_of_structs@jthread@35
.lb_skip@23:
   LDY   #$51
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .lb_skip@22
   JMP   .validate_array_of_structs@jthread@35
.lb_skip@22:
   LDY   #$52
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BNE   .validate_array_of_structs@jthread@35
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   BNE   .validate_array_of_structs@jthread@35
   LDY   #$55
   LDA   (__local_validate_array_of_structs__struct_array),Y
   DEY
   ORA   (__local_validate_array_of_structs__struct_array),Y
   BNE   .validate_array_of_structs@jthread@35
   LDY   #$57
   LDA   (__local_validate_array_of_structs__struct_array),Y
   DEY
   ORA   (__local_validate_array_of_structs__struct_array),Y
   BNE   .validate_array_of_structs@jthread@35
   LDY   #$59
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__6
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__5
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__4
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__3
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__2
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__1
   INY
   LDA   (__local_validate_array_of_structs__struct_array),Y
   STA   __local_validate_array_of_structs__0
   LDY   #$58
   LDA   (__local_validate_array_of_structs__struct_array),Y
   ORA   __local_validate_array_of_structs__6
   ORA   __local_validate_array_of_structs__5
   ORA   __local_validate_array_of_structs__4
   ORA   __local_validate_array_of_structs__3
   ORA   __local_validate_array_of_structs__2
   ORA   __local_validate_array_of_structs__1
   ORA   __local_validate_array_of_structs__0
   BNE   .validate_array_of_structs@jthread@35
   LDY   #$61
   LDA   (__local_validate_array_of_structs__struct_array),Y
   DEY
   ORA   (__local_validate_array_of_structs__struct_array),Y
   BNE   .validate_array_of_structs@jthread@35
   LDY   #$63
   LDA   (__local_validate_array_of_structs__struct_array),Y
   DEY
   ORA   (__local_validate_array_of_structs__struct_array),Y
   BEQ   .if_end@130
.validate_array_of_structs@jthread@35:
   LDX   #$00
   TXA
   RTS
.if_end@130:
   JMP   validate_full_initialization@tail0

str@0:
   DC.B  $49, $6D, $70, $6F, $72, $74, $61, $6E, $74, $20, $6D, $65, $73, $73, $61, $67
   DC.B  $65, $21, $00

str@1:
   DC.B  $50, $61, $72, $74, $69, $61, $6C, $00

str@2:
   DC.B  $6D, $69, $78, $65, $64, $00

str@3:
   DC.B  $37, $00

str@4:
   DC.B  $31, $30, $37, $00

str@5:
   DC.B  $32, $30, $37, $00
