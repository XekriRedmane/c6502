SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__6	EQU	$86
__local_main__7	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_add params= locals= indirect=false in_cycle=false
; def target_complement params= locals= indirect=false in_cycle=false
; def target_div params= locals= indirect=false in_cycle=false
; def target_eq params= locals= indirect=false in_cycle=false
; def target_ge params= locals= indirect=false in_cycle=false
; def target_gt params= locals= indirect=false in_cycle=false
; def target_le params= locals= indirect=false in_cycle=false
; def target_le2 params= locals= indirect=false in_cycle=false
; def target_lt params= locals= indirect=false in_cycle=false
; def target_mult params= locals= indirect=false in_cycle=false
; def target_neg params= locals= indirect=false in_cycle=false
; def target_neq params= locals= indirect=false in_cycle=false
; def target_not params= locals= indirect=false in_cycle=false
; def target_rem params= locals= indirect=false in_cycle=false
; def target_sub params= locals= indirect=false in_cycle=false
; call main -> target_add
; call main -> target_complement
; call main -> target_div
; call main -> target_eq
; call main -> target_ge
; call main -> target_gt
; call main -> target_le
; call main -> target_le2
; call main -> target_lt
; call main -> target_mult
; call main -> target_neg
; call main -> target_neq
; call main -> target_not
; call main -> target_rem
; call main -> target_sub
; item target_add section=.text local=false zero=false
; item target_add@tail1 section=.text local=true zero=false
; item target_add@tail1@tail3 section=.text local=true zero=false
; item target_sub section=.text local=false zero=false
; item target_sub@tail0 section=.text local=true zero=false
; item target_mult section=.text local=false zero=false
; item target_div section=.text local=false zero=false
; item target_rem section=.text local=false zero=false
; item target_complement section=.text local=false zero=false
; item target_neg section=.text local=false zero=false
; item target_not section=.text local=false zero=false
; item target_eq section=.text local=false zero=false
; item target_eq@tail2 section=.text local=true zero=false
; item target_neq section=.text local=false zero=false
; item target_gt section=.text local=false zero=false
; item target_ge section=.text local=false zero=false
; item target_lt section=.text local=false zero=false
; item target_le section=.text local=false zero=false
; item target_le2 section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

target_add:
   SUBROUTINE

   LDA   #$09
   JMP   target_add@tail1

target_add@tail1:
   SUBROUTINE

   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   JMP   target_add@tail1@tail3

target_add@tail1@tail3:
   SUBROUTINE

   STY   HARGS+10
   STA   HARGS+11
   RTS

target_sub:
   SUBROUTINE

   LDA   #$FE
   JMP   target_sub@tail0

target_sub@tail0:
   SUBROUTINE

   STA   HARGS+8
   LDA   #$FF
   STA   HARGS+9
   LDY   #$FF
   JMP   target_add@tail1@tail3

target_mult:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   JMP   target_add@tail1@tail3

target_div:
   SUBROUTINE

   LDA   #$99
   STA   HARGS+8
   LDY   #$99
   STY   HARGS+9
   JMP   target_add@tail1@tail3

target_rem:
   SUBROUTINE

   LDA   #$04
   JMP   target_add@tail1

target_complement:
   SUBROUTINE

   LDA   #$FE
   JMP   target_sub@tail0

target_neg:
   SUBROUTINE

   LDA   #$A4
   JMP   target_sub@tail0

target_not:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_eq:
   SUBROUTINE

   JMP   target_eq@tail2

target_eq@tail2:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

target_neq:
   SUBROUTINE

   JMP   target_eq@tail2

target_gt:
   SUBROUTINE

   JMP   target_eq@tail2

target_ge:
   SUBROUTINE

   JMP   target_eq@tail2

target_lt:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_le:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_le2:
   SUBROUTINE

   JMP   target_eq@tail2

main:
   SUBROUTINE

   JSR   target_add
   LDA   HARGS+11
   BNE   .jcmp_skip@0
   LDA   HARGS+10
   BNE   .jcmp_skip@0
   LDA   HARGS+9
   BNE   .jcmp_skip@0
   LDA   HARGS+8
   CMP   #$09
   BEQ   .if_end@0
.jcmp_skip@0:
   JMP   target_eq@tail2
.if_end@0:
   JSR   target_sub
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   HARGS+8
   CMP   #$FE
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   target_mult
   LDA   #$00
   CMP   #$80
   BNE   .jcmp_skip@2
   CMP   #$00
   BNE   .jcmp_skip@2
   LDA   HARGS+11
   BNE   .jcmp_skip@2
   LDA   HARGS+10
   BNE   .jcmp_skip@2
   LDA   HARGS+9
   BNE   .jcmp_skip@2
   LDA   HARGS+8
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   target_div
   LDA   #$00
   CMP   #$19
   BNE   .jcmp_skip@3
   CMP   #$99
   BNE   .jcmp_skip@3
   LDA   HARGS+11
   CMP   #$99
   BNE   .jcmp_skip@3
   LDA   HARGS+10
   CMP   #$99
   BNE   .jcmp_skip@3
   LDA   HARGS+9
   CMP   #$99
   BNE   .jcmp_skip@3
   LDA   HARGS+8
   CMP   #$99
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   target_rem
   LDA   HARGS+11
   BNE   .jcmp_skip@4
   LDA   HARGS+10
   BNE   .jcmp_skip@4
   LDA   HARGS+9
   BNE   .jcmp_skip@4
   LDA   HARGS+8
   CMP   #$04
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   JSR   target_complement
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   HARGS+8
   CMP   #$FE
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   JSR   target_neg
   LDA   HARGS+8
   CLC
   ADC   #$5C
   STA   __local_main__4
   LDA   HARGS+9
   ADC   #$00
   STA   __local_main__7
   LDA   HARGS+10
   ADC   #$00
   STA   __local_main__6
   LDA   HARGS+11
   ADC   #$00
   STA   __local_main__5
   LDA   #$00
   ADC   #$00
   STA   __local_main__3
   LDA   #$00
   ADC   #$00
   STA   __local_main__2
   LDA   #$00
   ADC   #$00
   STA   __local_main__1
   LDA   #$00
   ADC   #$80
   STA   __local_main__0
   LDA   __local_main__4
   ORA   __local_main__7
   ORA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   JSR   target_not
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   JSR   target_eq
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@8
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   JSR   target_neq
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@9
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   JSR   target_gt
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@10
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   JSR   target_ge
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@11
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   JSR   target_lt
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@12
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@12:
   JSR   target_le
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@13
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@13:
   JSR   target_le2
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@14
   LDX   #$00
   LDA   #$0F
   RTS
.if_end@14:
   LDX   #$00
   TXA
   RTS
