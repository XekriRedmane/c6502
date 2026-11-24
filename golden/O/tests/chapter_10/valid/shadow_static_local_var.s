SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_update_static_or_global__new_val_0	EQU	$80
__zpabi_update_static_or_global__new_val_1	EQU	$81
__local_main__0	EQU	$82
__local_main__1	EQU	$83
__local_update_static_or_global__update_global_1	EQU	$84

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def update_static_or_global params=__zpabi_update_static_or_global__update_global_0,__zpabi_update_static_or_global__update_global_1,__zpabi_update_static_or_global__new_val_0,__zpabi_update_static_or_global__new_val_1 locals=__local_update_static_or_global__update_global_1,__local_update_static_or_global__update_global_0 indirect=false in_cycle=false param_regs=A,X,,
; call main -> update_static_or_global
; item update_static_or_global section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; item i section=.bss local=false zero=true
; item @2.i section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

update_static_or_global:
   SUBROUTINE

   STX   __local_update_static_or_global__update_global_1
   ORA   __local_update_static_or_global__update_global_1
   BEQ   .if_else@1
   LDA   __zpabi_update_static_or_global__new_val_0
   STA   i
   LDA   __zpabi_update_static_or_global__new_val_1
   STA   i+1
   JMP   .if_end@0
.if_else@1:
   LDA   __zpabi_update_static_or_global__new_val_0
   STA   @2.i
   LDA   __zpabi_update_static_or_global__new_val_1
   STA   @2.i+1
.if_end@0:
   LDX   @2.i+1
   LDA   @2.i
   RTS

main:
   SUBROUTINE

   LDA   i
   ORA   i+1
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDA   #$0A
   STA   __zpabi_update_static_or_global__new_val_0
   JSR   main@run0
   LDA   #$01
   JSR   update_static_or_global
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .jcmp_skip@4
   LDA   i+1
   BNE   .jcmp_skip@4
   LDA   i
   CMP   #$0A
   BNE   .jcmp_skip@4
   LDA   #$09
   STA   __zpabi_update_static_or_global__new_val_0
   JSR   main@run0
   JSR   update_static_or_global
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   CMP   #$09
   BNE   .jcmp_skip@4
   LDA   i+1
   BNE   .jcmp_skip@4
   LDA   i
   CMP   #$0A
   BNE   .jcmp_skip@4
   LDA   #$0B
   STA   __zpabi_update_static_or_global__new_val_0
   JSR   main@run0
   LDA   #$01
   JSR   update_static_or_global
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   CMP   #$09
   BNE   .jcmp_skip@4
   LDA   i+1
   BNE   .jcmp_skip@4
   LDA   i
   CMP   #$0B
   BEQ   .if_end@8
.jcmp_skip@4:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@8:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_update_static_or_global__new_val_1
   LDX   #$00
   RTS

i:
   DS.B  2

@2.i:
   DS.B  2
