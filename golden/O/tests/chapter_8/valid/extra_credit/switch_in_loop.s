SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__acc_0	EQU	$80
__local_main__acc_1	EQU	$81
__local_main__0	EQU	$82
__local_main__1	EQU	$83
__local_main__i_1	EQU	$84
__local_main__i_0	EQU	$85
__local_main__ctr	EQU	$86

; @zp-link-meta-begin
; def main params= locals=__local_main__acc_0,__local_main__acc_1,__local_main__0,__local_main__1,__local_main__i_1,__local_main__i_0,__local_main__ctr indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__acc_0
   STA   __local_main__acc_1
   STA   __local_main__ctr
   LDX   #$00
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$0A
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_main__i_0
   ORA   __local_main__i_1
   BEQ   .case@2
   LDA   __local_main__i_1
   BNE   .default@5
   LDA   __local_main__i_0
   CMP   #$01
   BEQ   .case@3
   LDA   __local_main__i_1
   BNE   .default@5
   LDA   __local_main__i_0
   CMP   #$02
   BEQ   .case@4
   BNE   .default@5
.case@2:
   LDA   #$02
   STA   __local_main__acc_0
   LDA   #$00
   STA   __local_main__acc_1
   JMP   .switch@1_break
.case@3:
   LDA   __local_main__acc_0
   ASL
   STA   __local_main__0
   LDA   __local_main__acc_1
   ROL
   STA   __local_main__1
   LDA   __local_main__0
   CLC
   ADC   __local_main__acc_0
   STA   __local_main__acc_0
   LDA   __local_main__1
   ADC   __local_main__acc_1
   STA   __local_main__acc_1
   JMP   .switch@1_break
.case@4:
   ASL   __local_main__acc_0
   ROL   __local_main__acc_1
   ASL   __local_main__acc_0
   ROL   __local_main__acc_1
   JMP   .switch@1_break
.default@5:
   INC   __local_main__acc_0
   BNE   .inc_done@0
   INC   __local_main__acc_1
.inc_done@0:
.switch@1_break:
   INC   __local_main__ctr
   BNE   .inc_done@1
   INX
.inc_done@1:
   INC   __local_main__i_0
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_main__i_1
   JMP   .loop@0_start
.loop@0_break:
   CPX   #$00
   BNE   .and_false@0
   LDA   __local_main__ctr
   CMP   #$0A
   BNE   .and_false@0
   LDA   __local_main__acc_1
   BNE   .and_false@0
   LDA   __local_main__acc_0
   CMP   #$1F
   BNE   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   TYA
   LDX   #$00
   RTS
