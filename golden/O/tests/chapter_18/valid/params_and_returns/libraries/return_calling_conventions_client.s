SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strncmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__zpabi_strncmp__s2_1	EQU	$81
__zpabi_strncmp__n_0	EQU	$82
__zpabi_strncmp__n_1	EQU	$83
__zpabi_strncmp__n_2	EQU	$84
__zpabi_strncmp__n_3	EQU	$85

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; ext strncmp params=__zpabi_strncmp__s1_0,__zpabi_strncmp__s1_1,__zpabi_strncmp__s2_0,__zpabi_strncmp__s2_1,__zpabi_strncmp__n_0,__zpabi_strncmp__n_1,__zpabi_strncmp__n_2,__zpabi_strncmp__n_3 param_regs=A,X,,,,,,
; call main -> pass_and_return_regs
; call main -> return_double_struct
; call main -> return_int_struct
; call main -> return_mixed
; call main -> return_mixed2
; call main -> return_on_stack
; call main -> return_two_double_struct
; call main -> return_two_int_struct
; call main -> strcmp
; call main -> strncmp
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 149 local bytes, 20 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$97
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$96
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1
   LDA   $C0
   LDY   #$01
   STA   (FP),Y
   LDA   $C1
   INY
   STA   (FP),Y
   LDA   $C2
   INY
   STA   (FP),Y
   LDA   $C3
   INY
   STA   (FP),Y
   LDA   $C4
   INY
   STA   (FP),Y
   LDA   $C5
   INY
   STA   (FP),Y
   LDA   $C6
   INY
   STA   (FP),Y
   LDA   $C7
   INY
   STA   (FP),Y
   LDA   $C8
   INY
   STA   (FP),Y
   LDA   $C9
   INY
   STA   (FP),Y
   LDA   $CA
   INY
   STA   (FP),Y
   LDA   $CB
   INY
   STA   (FP),Y
   LDA   $CC
   INY
   STA   (FP),Y
   LDA   $CD
   INY
   STA   (FP),Y
   LDA   $CE
   INY
   STA   (FP),Y
   LDA   $CF
   INY
   STA   (FP),Y
   LDA   $D0
   INY
   STA   (FP),Y
   LDA   $D1
   INY
   STA   (FP),Y
   LDA   $D2
   INY
   STA   (FP),Y
   LDA   $D3
   INY
   STA   (FP),Y

   LDA   #$52
   STA   $D3
   LDA   #$AA
   STA   $D2
   LDA   #$08
   STA   $D1
   LDA   #$00
   STA   $D0
   LDA   #$5F
   STA   $CF
   LDA   #$05
   STA   $CE
   LDA   #$00
   STA   $CD
   LDY   #$06
   STY   $CC
   STA   $CB
   INY
   STY   $CA
   STA   $C9
   LDA   #$70
   STA   $C8
   LDA   #$3D
   STA   $C7
   LDA   #$0A
   STA   $C6
   LDA   #$D7
   STA   $C5
   LDA   #$A3
   STA   $C4
   LDA   #$70
   STA   $C3
   LDA   #$3D
   STA   $C2
   LDA   #$12
   STA   $C1
   LDA   #$40
   STA   $C0
   CLC
   LDA   FP
   ADC   #$15
   STA   $86
   LDA   FP+1
   ADC   #$00
   STA   $87
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $86
   LDY   #$01
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   JSR   return_int_struct
   LDY   #$15
   LDA   (FP),Y
   STA   $88
   INY
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   STA   $86
   LDA   $87
   BNE   .main@jthread@0
   LDA   $88
   CMP   #$01
   BNE   .main@jthread@0
   LDA   $86
   CMP   #$02
   BNE   .lb_skip@11
   JMP   .if_end@2
.lb_skip@11:
.main@jthread@0:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   INY
   LDA   (FP),Y
   STA   $D3
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@2:
   CLC
   LDA   FP
   ADC   #$18
   STA   $86
   LDA   FP+1
   ADC   #$00
   STA   $87
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $86
   LDY   #$01
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   JSR   return_two_int_struct
   LDY   #$18
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   BNE   .main@jthread@1
   LDA   $87
   CMP   #$0A
   BNE   .main@jthread@1
   CLC
   LDA   FP
   ADC   #$22
   STA   $87
   LDA   FP+1
   ADC   #$00
   STA   $88
   LDA   $87
   CLC
   ADC   #$02
   STA   $86
   BCC   .carry_inc@0
   INC   $88
.carry_inc@0:
   LDA   #<str@0
   STA   __zpabi_strncmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strncmp__s2_1
   LDA   #$08
   STA   __zpabi_strncmp__n_0
   LDA   #$00
   STA   __zpabi_strncmp__n_1
   STA   __zpabi_strncmp__n_2
   STA   __zpabi_strncmp__n_3
   LDX   $88
   LDA   $86
   JSR   strncmp
   STA   $87
   STX   $86
   ORA   $86
   BNE   .lb_skip@10
   JMP   .if_end@5
.lb_skip@10:
.main@jthread@1:
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   INY
   LDA   (FP),Y
   STA   $D3
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@5:
   CLC
   LDA   FP
   ADC   #$2C
   STA   $86
   LDA   FP+1
   ADC   #$00
   STA   $87
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $86
   LDY   #$01
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   JSR   return_double_struct
   LDY   #$2C
   LDA   (FP),Y
   STA   $8D
   INY
   LDA   (FP),Y
   STA   $8C
   INY
   LDA   (FP),Y
   STA   $8B
   INY
   LDA   (FP),Y
   STA   $8A
   INY
   LDA   (FP),Y
   STA   $89
   INY
   LDA   (FP),Y
   STA   $88
   INY
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   $87
   CMP   #$59
   BNE   .jcmp_skip@0
   LDA   $88
   CMP   #$28
   BNE   .jcmp_skip@0
   LDA   $89
   BNE   .jcmp_skip@0
   LDA   $8A
   BNE   .jcmp_skip@0
   LDA   $8B
   BNE   .jcmp_skip@0
   LDA   $8C
   BNE   .jcmp_skip@0
   LDA   $8D
   CMP   #$00
   BNE   .lb_skip@9
   JMP   .if_end@6
.lb_skip@9:
.jcmp_skip@0:
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   INY
   LDA   (FP),Y
   STA   $D3
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@6:
   CLC
   LDA   FP
   ADC   #$34
   STA   $86
   LDA   FP+1
   ADC   #$00
   STA   $87
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $86
   LDY   #$01
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   JSR   return_two_double_struct
   LDY   #$34
   LDA   (FP),Y
   STA   $95
   INY
   LDA   (FP),Y
   STA   $94
   INY
   LDA   (FP),Y
   STA   $93
   INY
   LDA   (FP),Y
   STA   $92
   INY
   LDA   (FP),Y
   STA   $91
   INY
   LDA   (FP),Y
   STA   $90
   INY
   LDA   (FP),Y
   STA   $8F
   INY
   LDA   (FP),Y
   STA   $8E
   INY
   LDA   (FP),Y
   STA   $8D
   INY
   LDA   (FP),Y
   STA   $8C
   INY
   LDA   (FP),Y
   STA   $8B
   INY
   LDA   (FP),Y
   STA   $8A
   INY
   LDA   (FP),Y
   STA   $89
   INY
   LDA   (FP),Y
   STA   $88
   INY
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   STA   $86
   LDA   $8E
   CMP   #$40
   BNE   .main@jthread@2
   LDA   $8F
   CMP   #$21
   BNE   .main@jthread@2
   LDA   $90
   CMP   #$99
   BNE   .main@jthread@2
   LDA   $91
   CMP   #$99
   BNE   .main@jthread@2
   LDA   $92
   CMP   #$99
   BNE   .main@jthread@2
   LDA   $93
   CMP   #$99
   BNE   .main@jthread@2
   LDA   $94
   CMP   #$99
   BNE   .main@jthread@2
   LDA   $95
   CMP   #$9A
   BNE   .main@jthread@2
   LDA   $86
   CMP   #$40
   BNE   .main@jthread@2
   LDA   $87
   CMP   #$1F
   BNE   .main@jthread@2
   LDA   $88
   CMP   #$33
   BNE   .main@jthread@2
   LDA   $89
   CMP   #$33
   BNE   .main@jthread@2
   LDA   $8A
   CMP   #$33
   BNE   .main@jthread@2
   LDA   $8B
   CMP   #$33
   BNE   .main@jthread@2
   LDA   $8C
   CMP   #$33
   BNE   .main@jthread@2
   LDA   $8D
   CMP   #$33
   BNE   .lb_skip@8
   JMP   .if_end@9
.lb_skip@8:
.main@jthread@2:
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   INY
   LDA   (FP),Y
   STA   $D3
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@9:
   CLC
   LDA   FP
   ADC   #$44
   STA   $86
   LDA   FP+1
   ADC   #$00
   STA   $87
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $86
   LDY   #$01
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   JSR   return_mixed
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   STA   $8D
   INY
   LDA   (FP),Y
   STA   $8C
   INY
   LDA   (FP),Y
   STA   $8B
   INY
   LDA   (FP),Y
   STA   $8A
   INY
   LDA   (FP),Y
   STA   $89
   INY
   LDA   (FP),Y
   STA   $88
   INY
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   CMP   #$40
   BNE   .main@jthread@3
   LDA   $87
   CMP   #$24
   BNE   .main@jthread@3
   LDA   $88
   BNE   .main@jthread@3
   LDA   $89
   BNE   .main@jthread@3
   LDA   $8A
   BNE   .main@jthread@3
   LDA   $8B
   BNE   .main@jthread@3
   LDA   $8C
   BNE   .main@jthread@3
   LDA   $8D
   CMP   #$00
   BNE   .main@jthread@3
   CLC
   LDA   FP
   ADC   #$4F
   STA   $87
   LDA   FP+1
   ADC   #$00
   STA   $88
   LDA   $87
   CLC
   ADC   #$08
   STA   $86
   BCC   .carry_inc@1
   INC   $88
.carry_inc@1:
   LDA   #<str@1
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@1
   STA   __zpabi_strcmp__s2_1
   LDX   $88
   LDA   $86
   JSR   strcmp
   STA   $87
   STX   $86
   ORA   $86
   BNE   .lb_skip@7
   JMP   .if_end@12
.lb_skip@7:
.main@jthread@3:
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   INY
   LDA   (FP),Y
   STA   $D3
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@12:
   CLC
   LDA   FP
   ADC   #$5A
   STA   $86
   LDA   FP+1
   ADC   #$00
   STA   $87
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $86
   LDY   #$01
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   JSR   return_mixed2
   LDY   #$5A
   LDA   (FP),Y
   STA   $8E
   INY
   LDA   (FP),Y
   STA   $8D
   INY
   LDA   (FP),Y
   STA   $8C
   INY
   LDA   (FP),Y
   STA   $8B
   INY
   LDA   (FP),Y
   STA   $8A
   INY
   LDA   (FP),Y
   STA   $89
   INY
   LDA   (FP),Y
   STA   $88
   INY
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   STA   $86
   LDA   $8E
   CMP   #$7F
   BNE   .main@jthread@4
   LDA   $86
   CMP   #$49
   BNE   .main@jthread@4
   LDA   $87
   CMP   #$2E
   BNE   .main@jthread@4
   LDA   $88
   CMP   #$7E
   BNE   .main@jthread@4
   LDA   $89
   CMP   #$04
   BNE   .main@jthread@4
   LDA   $8A
   CMP   #$65
   BNE   .main@jthread@4
   LDA   $8B
   CMP   #$7A
   BNE   .main@jthread@4
   LDA   $8C
   CMP   #$55
   BNE   .main@jthread@4
   LDA   $8D
   CMP   #$00
   BNE   .lb_skip@6
   JMP   .if_end@15
.lb_skip@6:
.main@jthread@4:
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   INY
   LDA   (FP),Y
   STA   $D3
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@15:
   CLC
   LDA   FP
   ADC   #$63
   STA   $86
   LDA   FP+1
   ADC   #$00
   STA   $87
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $86
   LDY   #$01
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   JSR   return_on_stack
   LDY   #$63
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   STA   $8D
   INY
   LDA   (FP),Y
   STA   $8C
   INY
   LDA   (FP),Y
   STA   $8B
   INY
   LDA   (FP),Y
   STA   $8A
   INY
   LDA   (FP),Y
   STA   $89
   INY
   LDA   (FP),Y
   STA   $88
   INY
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   CMP   #$3F
   BEQ   .lb_skip@5
   JMP   .main@jthread@7
.lb_skip@5:
   LDA   $87
   CMP   #$F4
   BEQ   .lb_skip@4
   JMP   .main@jthread@7
.lb_skip@4:
   LDA   $88
   BEQ   .lb_skip@12
   JMP   .main@jthread@7
.lb_skip@12:
   LDA   $89
   BNE   .main@jthread@7
   LDA   $8A
   BNE   .main@jthread@7
   LDA   $8B
   BNE   .main@jthread@7
   LDA   $8C
   BNE   .main@jthread@7
   LDA   $8D
   CMP   #$00
   BNE   .main@jthread@7
   CLC
   LDA   FP
   ADC   #$74
   STA   $87
   LDA   FP+1
   ADC   #$00
   STA   $88
   LDA   $87
   CLC
   ADC   #$08
   STA   $86
   BCC   .carry_inc@2
   INC   $88
.carry_inc@2:
   LDA   #<str@2
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@2
   STA   __zpabi_strcmp__s2_1
   LDX   $88
   LDA   $86
   JSR   strcmp
   STX   $86
   ORA   $86
   BNE   .main@jthread@7
   LDY   #$7F
   LDA   (FP),Y
   STA   $89
   INY
   LDA   (FP),Y
   STA   $88
   INY
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   BNE   .main@jthread@7
   LDA   $87
   BNE   .main@jthread@7
   LDA   $88
   BNE   .main@jthread@7
   LDA   $89
   CMP   #$64
   BNE   .main@jthread@7
   INY
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   BNE   .main@jthread@7
   LDA   $87
   CMP   #$2C
   BNE   .lb_skip@3
   JMP   .if_end@22
.lb_skip@3:
.main@jthread@7:
   LDX   #$00
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   INY
   LDA   (FP),Y
   STA   $D3
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@22:
   CLC
   LDA   FP
   ADC   #$85
   STA   $86
   LDA   FP+1
   ADC   #$00
   STA   $87
   SEC
   LDA   SSP
   SBC   #$28
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $86
   LDY   #$01
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   LDA   #$06
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$10
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   $C8
   INY
   STA   (SSP),Y
   LDA   $C7
   INY
   STA   (SSP),Y
   LDA   $C6
   INY
   STA   (SSP),Y
   LDA   $C5
   INY
   STA   (SSP),Y
   LDA   $C4
   INY
   STA   (SSP),Y
   LDA   $C3
   INY
   STA   (SSP),Y
   LDA   $C2
   INY
   STA   (SSP),Y
   LDA   $C1
   INY
   STA   (SSP),Y
   LDA   $C0
   INY
   STA   (SSP),Y
   LDA   #$05
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $CF
   INY
   STA   (SSP),Y
   LDA   $CE
   INY
   STA   (SSP),Y
   LDA   $CD
   INY
   STA   (SSP),Y
   LDA   $CC
   INY
   STA   (SSP),Y
   LDA   $CB
   INY
   STA   (SSP),Y
   LDA   $CA
   INY
   STA   (SSP),Y
   LDA   $C9
   INY
   STA   (SSP),Y
   LDA   #$4D
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   $D3
   INY
   STA   (SSP),Y
   LDA   $D2
   INY
   STA   (SSP),Y
   LDA   $D1
   INY
   STA   (SSP),Y
   LDA   $D0
   INY
   STA   (SSP),Y
   LDA   #$63
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   pass_and_return_regs
   LDY   #$85
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   STA   $8D
   INY
   LDA   (FP),Y
   STA   $8C
   INY
   LDA   (FP),Y
   STA   $8B
   INY
   LDA   (FP),Y
   STA   $8A
   INY
   LDA   (FP),Y
   STA   $89
   INY
   LDA   (FP),Y
   STA   $88
   INY
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   STA   $86
   LDA   $8D
   ORA   $8C
   ORA   $8B
   ORA   $8A
   ORA   $89
   ORA   $88
   ORA   $87
   ORA   $86
   BNE   .main@jthread@10
   INY
   LDA   (FP),Y
   BNE   .main@jthread@10
   INY
   LDA   (FP),Y
   BNE   .main@jthread@10
   INY
   LDA   (FP),Y
   BNE   .lb_skip@2
   JMP   .if_end@29
.lb_skip@2:
.main@jthread@10:
   LDX   #$00
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   INY
   LDA   (FP),Y
   STA   $D3
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@29:
   LDY   #$83
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   ORA   $87
   BNE   .lb_skip@1
   JMP   .if_end@30
.lb_skip@1:
   LDX   #$00
   LDA   #$09

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   INY
   LDA   (FP),Y
   STA   $D3
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@30:
   LDY   #$7F
   LDA   (FP),Y
   STA   $89
   INY
   LDA   (FP),Y
   STA   $88
   INY
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   BNE   .jcmp_skip@1
   LDA   $87
   BNE   .jcmp_skip@1
   LDA   $88
   BNE   .jcmp_skip@1
   LDA   $89
   CMP   #$64
   BNE   .lb_skip@0
   JMP   .if_end@31
.lb_skip@0:
.jcmp_skip@1:
   LDX   #$00
   LDA   #$0A

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   INY
   LDA   (FP),Y
   STA   $D3
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@31:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   INY
   LDA   (FP),Y
   STA   $CE
   INY
   LDA   (FP),Y
   STA   $CF
   INY
   LDA   (FP),Y
   STA   $D0
   INY
   LDA   (FP),Y
   STA   $D1
   INY
   LDA   (FP),Y
   STA   $D2
   INY
   LDA   (FP),Y
   STA   $D3
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS

str@0:
   DC.B  $31, $32, $33, $34, $35, $36, $37, $38, $00

str@1:
   DC.B  $61, $62, $00

str@2:
   DC.B  $78, $79, $00
