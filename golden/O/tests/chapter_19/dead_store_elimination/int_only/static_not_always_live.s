SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
i	EQU	$26
__local_target__counter_1	EQU	$80
__local_target__counter_0	EQU	$81

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; def target params= locals=__local_target__counter_1,__local_target__counter_0 indirect=false in_cycle=false
; ext exit_wrapper params=__zpabi_exit_wrapper__status_0,__zpabi_exit_wrapper__status_1 param_regs=A,X
; call main -> target
; call target -> exit_wrapper
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp i size=2 at=auto def=true init=$00,$00 weight=25
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   #$00
   STA   __local_target__counter_0
   LDY   #$00
   STY   __local_target__counter_1
.loop@0_start:
   LDA   __local_target__counter_0
   SEC
   SBC   #$0A
   LDA   __local_target__counter_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .if_else@1
   LDA   __local_target__counter_0
   CLC
   ADC   #$01
   STA   i
   LDA   __local_target__counter_1
   ADC   #$00
   STA   i+1
   JMP   .if_end@0
.if_else@1:
   LDA   __local_target__counter_0
   CLC
   ADC   #$02
   STA   i
   LDA   __local_target__counter_1
   ADC   #$00
   STA   i+1
.if_end@0:
   LDA   #$14
   SEC
   SBC   __local_target__counter_0
   LDA   #$00
   SBC   __local_target__counter_1
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .if_end@2
   LDX   i+1
   LDA   i
   JSR   exit_wrapper
.if_end@2:
   INC   __local_target__counter_0
   BNE   .loop@0_start
   INC   __local_target__counter_1
   JMP   .loop@0_start

main:
   SUBROUTINE

   JSR   target
   LDX   #$00
   TXA
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   i
   STA   i+1
   RTS
