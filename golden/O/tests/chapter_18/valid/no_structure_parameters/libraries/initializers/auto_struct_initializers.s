SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_validate_converted__0	EQU	$80
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_malloc__size_0	EQU	$80
__zpabi_validate_partial_initialization__expected_msg_0	EQU	$80
__zpabi_validate_two_structs__ptr2_0	EQU	$80
__local_validate_converted__1	EQU	$81
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_malloc__size_1	EQU	$81
__zpabi_validate_partial_initialization__expected_msg_1	EQU	$81
__zpabi_validate_two_structs__ptr2_1	EQU	$81
__local_validate_converted__2	EQU	$82
__local_validate_partial_initialization__0	EQU	$82
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_malloc__size_2	EQU	$82
__zpabi_strcmp__s2_0	EQU	$82
__local_validate_converted__3	EQU	$83
__local_validate_partial_initialization__1	EQU	$83
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_malloc__size_3	EQU	$83
__zpabi_strcmp__s2_1	EQU	$83
__local_validate_converted__4	EQU	$84
__local_validate_full_initialization__0	EQU	$84
__local_validate_partial_initialization__2	EQU	$84
__local_validate_two_structs__0	EQU	$84
__zpabi_calloc__size_0	EQU	$84
__local_validate_converted__5	EQU	$85
__local_validate_full_initialization__1	EQU	$85
__local_validate_partial_initialization__3	EQU	$85
__local_validate_two_structs__1	EQU	$85
__zpabi_calloc__size_1	EQU	$85
__local_validate_converted__6	EQU	$86
__local_validate_full_initialization__2	EQU	$86
__local_validate_partial_initialization__4	EQU	$86
__local_validate_two_structs__2	EQU	$86
__zpabi_calloc__size_2	EQU	$86
__local_validate_converted__7	EQU	$87
__local_validate_full_initialization__3	EQU	$87
__local_validate_partial_initialization__5	EQU	$87
__local_validate_two_structs__3	EQU	$87
__zpabi_calloc__size_3	EQU	$87
__local_validate_converted__8	EQU	$88
__local_validate_full_initialization__4	EQU	$88
__local_validate_partial_initialization__6	EQU	$88
__local_validate_two_structs__4	EQU	$88
__local_validate_converted__9	EQU	$89
__local_validate_full_initialization__5	EQU	$89
__local_validate_two_structs__5	EQU	$89
__local_validate_converted__ptr	EQU	$8A
__local_validate_full_initialization__6	EQU	$8A
__local_validate_partial_initialization__ptr	EQU	$8A
__local_validate_two_structs__6	EQU	$8A
__local_validate_converted__10	EQU	$8B
__local_validate_full_initialization__7	EQU	$8B
__local_validate_partial_initialization__8	EQU	$8B
__local_validate_two_structs__7	EQU	$8B
__local_validate_full_initialization__8	EQU	$8C
__local_validate_two_structs__8	EQU	$8C
__local_validate_full_initialization__9	EQU	$8D
__local_validate_two_structs__9	EQU	$8D
__local_validate_full_initialization__ptr	EQU	$8E
__local_validate_two_structs__ptr1	EQU	$8E
__local_validate_full_initialization__10	EQU	$8F
__local_validate_two_structs__10	EQU	$8F

; @zp-link-meta-begin
; def validate_converted params=__zpabi_validate_converted__ptr_0,__zpabi_validate_converted__ptr_1 locals=__local_validate_converted__0,__local_validate_converted__1,__local_validate_converted__2,__local_validate_converted__3,__local_validate_converted__4,__local_validate_converted__5,__local_validate_converted__6,__local_validate_converted__7,__local_validate_converted__8,__local_validate_converted__9,__local_validate_converted__ptr,__local_validate_converted__10 indirect=false in_cycle=false param_regs=A,X
; def validate_full_initialization params=__zpabi_validate_full_initialization__ptr_0,__zpabi_validate_full_initialization__ptr_1 locals=__local_validate_full_initialization__0,__local_validate_full_initialization__1,__local_validate_full_initialization__2,__local_validate_full_initialization__3,__local_validate_full_initialization__4,__local_validate_full_initialization__5,__local_validate_full_initialization__6,__local_validate_full_initialization__7,__local_validate_full_initialization__8,__local_validate_full_initialization__9,__local_validate_full_initialization__ptr,__local_validate_full_initialization__10 indirect=false in_cycle=false param_regs=A,X
; def validate_partial_initialization params=__zpabi_validate_partial_initialization__ptr_0,__zpabi_validate_partial_initialization__ptr_1,__zpabi_validate_partial_initialization__expected_msg_0,__zpabi_validate_partial_initialization__expected_msg_1 locals=__local_validate_partial_initialization__0,__local_validate_partial_initialization__1,__local_validate_partial_initialization__2,__local_validate_partial_initialization__3,__local_validate_partial_initialization__4,__local_validate_partial_initialization__5,__local_validate_partial_initialization__6,__local_validate_partial_initialization__7,__local_validate_partial_initialization__ptr,__local_validate_partial_initialization__8 indirect=false in_cycle=false param_regs=A,X,,
; def validate_two_structs params=__zpabi_validate_two_structs__ptr1_0,__zpabi_validate_two_structs__ptr1_1,__zpabi_validate_two_structs__ptr2_0,__zpabi_validate_two_structs__ptr2_1 locals=__local_validate_two_structs__0,__local_validate_two_structs__1,__local_validate_two_structs__2,__local_validate_two_structs__3,__local_validate_two_structs__4,__local_validate_two_structs__5,__local_validate_two_structs__6,__local_validate_two_structs__7,__local_validate_two_structs__8,__local_validate_two_structs__9,__local_validate_two_structs__ptr1,__local_validate_two_structs__10 indirect=false in_cycle=false param_regs=A,X,,
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call validate_full_initialization -> strcmp
; call validate_two_structs -> strcmp
; item validate_full_initialization section=.text local=false zero=false
; item validate_full_initialization@tail0 section=.text local=true zero=false
; item validate_partial_initialization section=.text local=false zero=false
; item validate_converted section=.text local=false zero=false
; item validate_two_structs section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

validate_full_initialization:
   SUBROUTINE

   STA   __local_validate_full_initialization__ptr
   STX   __local_validate_full_initialization__10
   LDY   #$00
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
   BEQ   .lb_skip@10
   JMP   .validate_full_initialization@jthread@5
.lb_skip@10:
   LDY   #$02
   LDA   (__local_validate_full_initialization__ptr),Y
   CMP   #$73
   BEQ   .lb_skip@9
   JMP   .validate_full_initialization@jthread@5
.lb_skip@9:
   LDY   #$03
   LDA   (__local_validate_full_initialization__ptr),Y
   CMP   #$75
   BEQ   .lb_skip@8
   JMP   .validate_full_initialization@jthread@5
.lb_skip@8:
   LDY   #$04
   LDA   (__local_validate_full_initialization__ptr),Y
   CMP   #$70
   BEQ   .lb_skip@7
   JMP   .validate_full_initialization@jthread@5
.lb_skip@7:
   LDY   #$05
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__1
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   CMP   __local_validate_full_initialization__10
   BEQ   .lb_skip@6
   JMP   .validate_full_initialization@jthread@5
.lb_skip@6:
   LDA   __local_validate_full_initialization__1
   CMP   __local_validate_full_initialization__ptr
   BEQ   .lb_skip@5
   JMP   .validate_full_initialization@jthread@5
.lb_skip@5:
   LDY   #$07
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
   CMP   #$42
   BEQ   .lb_skip@4
   JMP   .validate_full_initialization@jthread@5
.lb_skip@4:
   LDA   __local_validate_full_initialization__1
   CMP   #$7D
   BEQ   .lb_skip@3
   JMP   .validate_full_initialization@jthread@5
.lb_skip@3:
   LDA   __local_validate_full_initialization__2
   CMP   #$1A
   BEQ   .lb_skip@2
   JMP   .validate_full_initialization@jthread@5
.lb_skip@2:
   LDA   __local_validate_full_initialization__3
   CMP   #$94
   BEQ   .lb_skip@1
   JMP   .validate_full_initialization@jthread@5
.lb_skip@1:
   LDA   __local_validate_full_initialization__4
   CMP   #$A2
   BEQ   .lb_skip@0
   JMP   .validate_full_initialization@jthread@5
.lb_skip@0:
   LDA   __local_validate_full_initialization__5
   BNE   .validate_full_initialization@jthread@5
   LDA   __local_validate_full_initialization__6
   BNE   .validate_full_initialization@jthread@5
   LDA   __local_validate_full_initialization__7
   BNE   .validate_full_initialization@jthread@5
   LDY   #$0F
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__8
   INY
   LDA   (__local_validate_full_initialization__ptr),Y
   STA   __local_validate_full_initialization__9
   LDY   #$00
   LDA   (__local_validate_full_initialization__8),Y
   STA   __local_validate_full_initialization__7
   INY
   LDA   (__local_validate_full_initialization__8),Y
   STA   __local_validate_full_initialization__6
   INY
   LDA   (__local_validate_full_initialization__8),Y
   STA   __local_validate_full_initialization__5
   INY
   LDA   (__local_validate_full_initialization__8),Y
   STA   __local_validate_full_initialization__4
   INY
   LDA   (__local_validate_full_initialization__8),Y
   STA   __local_validate_full_initialization__3
   INY
   LDA   (__local_validate_full_initialization__8),Y
   STA   __local_validate_full_initialization__2
   INY
   LDA   (__local_validate_full_initialization__8),Y
   STA   __local_validate_full_initialization__1
   INY
   LDA   (__local_validate_full_initialization__8),Y
   CMP   #$42
   BNE   .validate_full_initialization@jthread@5
   LDA   __local_validate_full_initialization__1
   CMP   #$7D
   BNE   .validate_full_initialization@jthread@5
   LDA   __local_validate_full_initialization__2
   CMP   #$1A
   BNE   .validate_full_initialization@jthread@5
   LDA   __local_validate_full_initialization__3
   CMP   #$94
   BNE   .validate_full_initialization@jthread@5
   LDA   __local_validate_full_initialization__4
   CMP   #$A2
   BNE   .validate_full_initialization@jthread@5
   LDA   __local_validate_full_initialization__5
   BNE   .validate_full_initialization@jthread@5
   LDA   __local_validate_full_initialization__6
   BNE   .validate_full_initialization@jthread@5
   LDA   __local_validate_full_initialization__7
   BEQ   .if_end@12
.validate_full_initialization@jthread@5:
   LDX   #$00
   TXA
   RTS
.if_end@12:
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
   STA   __local_validate_partial_initialization__1
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   CMP   __zpabi_validate_partial_initialization__expected_msg_1
   BNE   .validate_partial_initialization@jthread@1
   LDA   __local_validate_partial_initialization__1
   CMP   __zpabi_validate_partial_initialization__expected_msg_0
   BNE   .validate_partial_initialization@jthread@1
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   CMP   #$61
   BNE   .validate_partial_initialization@jthread@1
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   CMP   #$62
   BEQ   .if_end@17
.validate_partial_initialization@jthread@1:
   LDX   #$00
   TXA
   RTS
.if_end@17:
   LDY   #$05
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__2
   INY
   LDA   (__local_validate_partial_initialization__ptr),Y
   STA   __local_validate_partial_initialization__3
   LDY   #$01
   LDA   (__local_validate_partial_initialization__2),Y
   DEY
   ORA   (__local_validate_partial_initialization__2),Y
   BNE   .validate_partial_initialization@jthread@3
   LDY   #$04
   LDA   (__local_validate_partial_initialization__ptr),Y
   BNE   .validate_partial_initialization@jthread@3
   LDY   #$08
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
   LDY   #$07
   LDA   (__local_validate_partial_initialization__ptr),Y
   ORA   __local_validate_partial_initialization__6
   ORA   __local_validate_partial_initialization__5
   ORA   __local_validate_partial_initialization__4
   ORA   __local_validate_partial_initialization__3
   ORA   __local_validate_partial_initialization__2
   ORA   __local_validate_partial_initialization__1
   ORA   __local_validate_partial_initialization__0
   BNE   .validate_partial_initialization@jthread@3
   LDY   #$10
   LDA   (__local_validate_partial_initialization__ptr),Y
   DEY
   ORA   (__local_validate_partial_initialization__ptr),Y
   BEQ   .if_end@23
.validate_partial_initialization@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@23:
   JMP   validate_full_initialization@tail0

validate_converted:
   SUBROUTINE

   STA   __local_validate_converted__ptr
   STX   __local_validate_converted__10
   LDY   #$01
   LDA   (__local_validate_converted__ptr),Y
   DEY
   ORA   (__local_validate_converted__ptr),Y
   BNE   .lb_skip@16
   JMP   .validate_converted@jthread@5
.lb_skip@16:
   LDY   #$02
   LDA   (__local_validate_converted__ptr),Y
   CMP   #$DC
   BEQ   .lb_skip@15
   JMP   .validate_converted@jthread@5
.lb_skip@15:
   LDY   #$03
   LDA   (__local_validate_converted__ptr),Y
   CMP   #$E8
   BEQ   .lb_skip@14
   JMP   .validate_converted@jthread@5
.lb_skip@14:
   LDY   #$04
   LDA   (__local_validate_converted__ptr),Y
   CMP   #$E0
   BEQ   .lb_skip@13
   JMP   .validate_converted@jthread@5
.lb_skip@13:
   LDY   #$06
   LDA   (__local_validate_converted__ptr),Y
   DEY
   ORA   (__local_validate_converted__ptr),Y
   BEQ   .lb_skip@12
   JMP   .validate_converted@jthread@5
.lb_skip@12:
   LDY   #$07
   LDA   (__local_validate_converted__ptr),Y
   STA   __local_validate_converted__7
   INY
   LDA   (__local_validate_converted__ptr),Y
   STA   __local_validate_converted__6
   INY
   LDA   (__local_validate_converted__ptr),Y
   STA   __local_validate_converted__5
   INY
   LDA   (__local_validate_converted__ptr),Y
   STA   __local_validate_converted__4
   INY
   LDA   (__local_validate_converted__ptr),Y
   STA   __local_validate_converted__3
   INY
   LDA   (__local_validate_converted__ptr),Y
   STA   __local_validate_converted__2
   INY
   LDA   (__local_validate_converted__ptr),Y
   STA   __local_validate_converted__1
   INY
   LDA   (__local_validate_converted__ptr),Y
   CMP   #$40
   BEQ   .lb_skip@11
   JMP   .validate_converted@jthread@5
.lb_skip@11:
   LDA   __local_validate_converted__1
   CMP   #$A7
   BNE   .validate_converted@jthread@5
   LDA   __local_validate_converted__2
   CMP   #$6E
   BNE   .validate_converted@jthread@5
   LDA   __local_validate_converted__3
   BNE   .validate_converted@jthread@5
   LDA   __local_validate_converted__4
   BNE   .validate_converted@jthread@5
   LDA   __local_validate_converted__5
   BNE   .validate_converted@jthread@5
   LDA   __local_validate_converted__6
   BNE   .validate_converted@jthread@5
   LDA   __local_validate_converted__7
   BNE   .validate_converted@jthread@5
   LDY   #$0F
   LDA   (__local_validate_converted__ptr),Y
   STA   __local_validate_converted__8
   INY
   LDA   (__local_validate_converted__ptr),Y
   STA   __local_validate_converted__9
   LDY   #$01
   LDA   (__local_validate_converted__8),Y
   STA   __local_validate_converted__6
   INY
   LDA   (__local_validate_converted__8),Y
   STA   __local_validate_converted__5
   INY
   LDA   (__local_validate_converted__8),Y
   STA   __local_validate_converted__4
   INY
   LDA   (__local_validate_converted__8),Y
   STA   __local_validate_converted__3
   INY
   LDA   (__local_validate_converted__8),Y
   STA   __local_validate_converted__2
   INY
   LDA   (__local_validate_converted__8),Y
   STA   __local_validate_converted__1
   INY
   LDA   (__local_validate_converted__8),Y
   STA   __local_validate_converted__0
   LDY   #$00
   LDA   (__local_validate_converted__8),Y
   ORA   __local_validate_converted__6
   ORA   __local_validate_converted__5
   ORA   __local_validate_converted__4
   ORA   __local_validate_converted__3
   ORA   __local_validate_converted__2
   ORA   __local_validate_converted__1
   ORA   __local_validate_converted__0
   BEQ   .if_end@36
.validate_converted@jthread@5:
   LDX   #$00
   TXA
   RTS
.if_end@36:
   JMP   validate_full_initialization@tail0

validate_two_structs:
   SUBROUTINE

   STA   __local_validate_two_structs__ptr1
   STX   __local_validate_two_structs__10
   LDY   #$00
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   STA   __local_validate_two_structs__0
   INY
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   TAX
   LDA   #<str@1
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@1
   STA   __zpabi_strcmp__s2_1
   LDA   __local_validate_two_structs__0
   JSR   strcmp
   STX   __local_validate_two_structs__0
   ORA   __local_validate_two_structs__0
   BEQ   .lb_skip@29
   JMP   .validate_two_structs@jthread@5
.lb_skip@29:
   LDY   #$00
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   STA   __local_validate_two_structs__3
   LDA   (__local_validate_two_structs__ptr1),Y
   STA   __local_validate_two_structs__1
   INY
   LDA   (__local_validate_two_structs__ptr1),Y
   STA   __local_validate_two_structs__0
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   CMP   __local_validate_two_structs__0
   BEQ   .lb_skip@28
   JMP   .validate_two_structs@jthread@5
.lb_skip@28:
   LDA   __local_validate_two_structs__3
   CMP   __local_validate_two_structs__1
   BEQ   .lb_skip@27
   JMP   .validate_two_structs@jthread@5
.lb_skip@27:
   LDY   #$02
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   CMP   #$78
   BEQ   .lb_skip@26
   JMP   .validate_two_structs@jthread@5
.lb_skip@26:
   LDY   #$03
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   CMP   #$79
   BEQ   .lb_skip@25
   JMP   .validate_two_structs@jthread@5
.lb_skip@25:
   LDY   #$05
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   STA   __local_validate_two_structs__1
   INY
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   CMP   __local_validate_two_structs__10
   BEQ   .lb_skip@24
   JMP   .validate_two_structs@jthread@5
.lb_skip@24:
   LDA   __local_validate_two_structs__1
   CMP   __local_validate_two_structs__ptr1
   BEQ   .lb_skip@23
   JMP   .validate_two_structs@jthread@5
.lb_skip@23:
   LDY   #$07
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   STA   __local_validate_two_structs__7
   INY
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   STA   __local_validate_two_structs__6
   INY
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   STA   __local_validate_two_structs__5
   INY
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   STA   __local_validate_two_structs__4
   INY
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   STA   __local_validate_two_structs__3
   INY
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   STA   __local_validate_two_structs__2
   INY
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   STA   __local_validate_two_structs__1
   INY
   LDA   (__zpabi_validate_two_structs__ptr2_0),Y
   CMP   #$40
   BEQ   .lb_skip@22
   JMP   .validate_two_structs@jthread@5
.lb_skip@22:
   LDA   __local_validate_two_structs__1
   CMP   #$62
   BEQ   .lb_skip@21
   JMP   .validate_two_structs@jthread@5
.lb_skip@21:
   LDA   __local_validate_two_structs__2
   CMP   #$C0
   BEQ   .lb_skip@20
   JMP   .validate_two_structs@jthread@5
.lb_skip@20:
   LDA   __local_validate_two_structs__3
   BEQ   .lb_skip@19
   JMP   .validate_two_structs@jthread@5
.lb_skip@19:
   LDA   __local_validate_two_structs__4
   BEQ   .lb_skip@18
   JMP   .validate_two_structs@jthread@5
.lb_skip@18:
   LDA   __local_validate_two_structs__5
   BEQ   .lb_skip@17
   JMP   .validate_two_structs@jthread@5
.lb_skip@17:
   LDA   __local_validate_two_structs__6
   BNE   .validate_two_structs@jthread@5
   LDA   __local_validate_two_structs__7
   BNE   .validate_two_structs@jthread@5
   LDY   #$0F
   LDA   (__local_validate_two_structs__ptr1),Y
   STA   __local_validate_two_structs__8
   INY
   LDA   (__local_validate_two_structs__ptr1),Y
   STA   __local_validate_two_structs__9
   LDY   #$00
   LDA   (__local_validate_two_structs__8),Y
   STA   __local_validate_two_structs__7
   INY
   LDA   (__local_validate_two_structs__8),Y
   STA   __local_validate_two_structs__6
   INY
   LDA   (__local_validate_two_structs__8),Y
   STA   __local_validate_two_structs__5
   INY
   LDA   (__local_validate_two_structs__8),Y
   STA   __local_validate_two_structs__4
   INY
   LDA   (__local_validate_two_structs__8),Y
   STA   __local_validate_two_structs__3
   INY
   LDA   (__local_validate_two_structs__8),Y
   STA   __local_validate_two_structs__2
   INY
   LDA   (__local_validate_two_structs__8),Y
   STA   __local_validate_two_structs__1
   INY
   LDA   (__local_validate_two_structs__8),Y
   CMP   #$40
   BNE   .tail@0
   LDA   __local_validate_two_structs__1
   CMP   #$5E
   BNE   .tail@0
   LDA   __local_validate_two_structs__2
   CMP   #$D9
   BNE   .tail@0
   LDA   __local_validate_two_structs__3
   CMP   #$99
   BNE   .tail@0
   LDA   __local_validate_two_structs__4
   CMP   #$99
   BNE   .tail@0
   LDA   __local_validate_two_structs__5
   CMP   #$99
   BNE   .tail@0
   LDA   __local_validate_two_structs__6
   CMP   #$99
   BNE   .tail@0
   LDA   __local_validate_two_structs__7
   CMP   #$9A
   BEQ   .if_end@49
.validate_two_structs@jthread@5:
   LDX   #$00
   TXA
   RTS
.if_end@49:
   LDA   __local_validate_two_structs__ptr1
   CLC
   ADC   #$02
   STA   __local_validate_two_structs__2
   BCC   .carry_inc@0
   INC   __local_validate_two_structs__10
.carry_inc@0:
   LDA   __zpabi_validate_two_structs__ptr2_0
   CLC
   ADC   #$02
   STA   __local_validate_two_structs__1
   LDA   __zpabi_validate_two_structs__ptr2_1
   ADC   #$00
   STA   __local_validate_two_structs__0
   LDA   __local_validate_two_structs__10
   CMP   __local_validate_two_structs__0
   BNE   .if_end@50
   LDA   __local_validate_two_structs__2
   CMP   __local_validate_two_structs__1
   BNE   .if_end@50
.tail@0:
   LDX   #$00
   TXA
   RTS
.if_end@50:
   JMP   validate_full_initialization@tail0

str@0:
   DC.B  $49, $27, $6D, $20, $61, $20, $73, $74, $72, $75, $63, $74, $21, $00

str@1:
   DC.B  $59, $65, $74, $20, $61, $6E, $6F, $74, $68, $65, $72, $20, $73, $74, $72, $69
   DC.B  $6E, $67, $00
