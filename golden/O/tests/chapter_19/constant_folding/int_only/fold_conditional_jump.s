SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target_jnz_to_jmp params= locals= indirect=false in_cycle=false
; def target_jz_to_jmp params= locals= indirect=false in_cycle=false
; def target_remove_jnz params= locals= indirect=false in_cycle=false
; def target_remove_jz params= locals= indirect=false in_cycle=false
; call main -> target_jnz_to_jmp
; call main -> target_jz_to_jmp
; call main -> target_remove_jnz
; call main -> target_remove_jz
; item target_jz_to_jmp section=.text local=false zero=false
; item target_remove_jz section=.text local=false zero=false
; item target_remove_jz@tail0 section=.text local=true zero=false
; item target_jnz_to_jmp section=.text local=false zero=false
; item target_remove_jnz section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target_jz_to_jmp:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_remove_jz:
   SUBROUTINE

   JMP   target_remove_jz@tail0

target_remove_jz@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

target_jnz_to_jmp:
   SUBROUTINE

   JMP   target_remove_jz@tail0

target_remove_jnz:
   SUBROUTINE

   JMP   target_remove_jz@tail0

main:
   SUBROUTINE

   JSR   target_jz_to_jmp
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@8
   JMP   target_remove_jz@tail0
.if_end@8:
   JSR   target_remove_jz
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@9:
   JSR   target_jnz_to_jmp
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@10
.jcmp_skip@1:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@10:
   JSR   target_remove_jnz
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@11
.jcmp_skip@2:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@11:
   LDX   #$00
   TXA
   RTS
