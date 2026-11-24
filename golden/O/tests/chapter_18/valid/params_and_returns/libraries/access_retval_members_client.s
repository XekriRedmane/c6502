SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_calloc__size_0	EQU	$84
__zpabi_calloc__size_1	EQU	$85
__zpabi_calloc__size_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; call main -> return_nested_struct
; call main -> return_small_struct
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 140 local bytes
   SEC
   LDA   SSP
   SBC   #$8E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$8D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   CLC
   LDA   FP
   ADC   #$01
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   return_small_struct
   LDY   #$02
   LDA   (FP),Y
   STA   $83
   INY
   LDA   (FP),Y
   STA   $82
   INY
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   BNE   .jcmp_skip@0
   LDA   $81
   BNE   .jcmp_skip@0
   LDA   $82
   BNE   .jcmp_skip@0
   LDA   $83
   CMP   #$66
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
.if_end@0:
   CLC
   LDA   FP
   ADC   #$06
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   return_nested_struct
   LDY   #$06
   LDA   (FP),Y
   STA   $87
   INY
   LDA   (FP),Y
   STA   $86
   INY
   LDA   (FP),Y
   STA   $85
   INY
   LDA   (FP),Y
   STA   $84
   INY
   LDA   (FP),Y
   STA   $83
   INY
   LDA   (FP),Y
   STA   $82
   INY
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   CMP   #$40
   BEQ   .lb_skip@7
   JMP   .main@jthread@1
.lb_skip@7:
   LDA   $81
   BEQ   .lb_skip@6
   JMP   .main@jthread@1
.lb_skip@6:
   LDA   $82
   BEQ   .lb_skip@5
   JMP   .main@jthread@1
.lb_skip@5:
   LDA   $83
   BEQ   .lb_skip@4
   JMP   .main@jthread@1
.lb_skip@4:
   LDA   $84
   BEQ   .lb_skip@3
   JMP   .main@jthread@1
.lb_skip@3:
   LDA   $85
   BEQ   .lb_skip@2
   JMP   .main@jthread@1
.lb_skip@2:
   LDA   $86
   BEQ   .lb_skip@1
   JMP   .main@jthread@1
.lb_skip@1:
   LDA   $87
   CMP   #$00
   BEQ   .lb_skip@0
   JMP   .main@jthread@1
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$15
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   return_nested_struct
   LDY   #$1F
   LDA   (FP),Y
   CMP   #$0A
   BNE   .main@jthread@1
   CLC
   LDA   FP
   ADC   #$24
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   return_nested_struct
   LDY   #$2F
   LDA   (FP),Y
   STA   $83
   INY
   LDA   (FP),Y
   STA   $82
   INY
   LDA   (FP),Y
   STA   $81
   INY
   LDA   (FP),Y
   BNE   .main@jthread@1
   LDA   $81
   BNE   .main@jthread@1
   LDA   $82
   BNE   .main@jthread@1
   LDA   $83
   CMP   #$0B
   BEQ   .if_end@5
.main@jthread@1:
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
   ADC   #$33
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   return_nested_struct
   LDY   #$3B
   LDA   (FP),Y
   STA   $80
   INY
   LDA   (FP),Y
   STA   $81
   STA   DPTR+1
   LDA   $80
   STA   DPTR
   LDY   #$00
   LDA   ($80),Y
   CMP   #$0C
   BNE   .main@jthread@2
   CLC
   LDA   FP
   ADC   #$42
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   return_nested_struct
   LDY   #$4A
   LDA   (FP),Y
   STA   $84
   INY
   LDA   (FP),Y
   STA   $85
   STA   DPTR+1
   LDA   $84
   STA   DPTR
   LDY   #$01
   LDA   ($84),Y
   STA   $83
   INY
   LDA   ($84),Y
   STA   $82
   INY
   LDA   ($84),Y
   STA   $81
   INY
   LDA   ($84),Y
   BNE   .main@jthread@2
   LDA   $81
   BNE   .main@jthread@2
   LDA   $82
   BNE   .main@jthread@2
   LDA   $83
   CMP   #$0D
   BEQ   .if_end@8
.main@jthread@2:
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
.if_end@8:
   CLC
   LDA   FP
   ADC   #$51
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   return_nested_struct
   LDY   #$59
   LDA   (FP),Y
   STA   $80
   INY
   LDA   (FP),Y
   STA   $81
   STA   DPTR+1
   LDA   $80
   STA   DPTR
   LDA   #$46
   LDY   #$00
   STA   ($80),Y
   CLC
   LDA   FP
   ADC   #$60
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   return_nested_struct
   LDY   #$68
   LDA   (FP),Y
   STA   $80
   INY
   LDA   (FP),Y
   STA   $81
   STA   DPTR+1
   LDA   $80
   STA   DPTR
   LDA   #$47
   LDY   #$01
   STA   ($80),Y
   LDA   #$00
   INY
   STA   ($80),Y
   INY
   STA   ($80),Y
   INY
   STA   ($80),Y
   CLC
   LDA   FP
   ADC   #$6F
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   return_nested_struct
   LDY   #$77
   LDA   (FP),Y
   STA   $80
   INY
   LDA   (FP),Y
   STA   $81
   STA   DPTR+1
   LDA   $80
   STA   DPTR
   LDY   #$00
   LDA   ($80),Y
   CMP   #$46
   BNE   .main@jthread@3
   CLC
   LDA   FP
   ADC   #$7E
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   return_nested_struct
   LDY   #$86
   LDA   (FP),Y
   STA   $84
   INY
   LDA   (FP),Y
   STA   $85
   LDY   #$01
   LDA   ($84),Y
   STA   $83
   INY
   LDA   ($84),Y
   STA   $82
   INY
   LDA   ($84),Y
   STA   $81
   INY
   LDA   ($84),Y
   BNE   .main@jthread@3
   LDA   $81
   BNE   .main@jthread@3
   LDA   $82
   BNE   .main@jthread@3
   LDA   $83
   CMP   #$47
   BEQ   .if_end@11
.main@jthread@3:
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
.if_end@11:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
