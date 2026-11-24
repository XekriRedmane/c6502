SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$81

; @zp-link-meta-begin
; def get_flag params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__union1,__local_main__0,__local_main__1,__local_main__union2 indirect=false in_cycle=false
; call main -> get_flag
; item get_flag section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @0.flag section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

get_flag:
   SUBROUTINE

   LDA   @0.flag
   ORA   @0.flag+1
   BEQ   .lnot_true@0
   LDA   #$00
   BEQ   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   STA   @0.flag
   LDA   #$00
   STA   @0.flag+1
   TAX
   LDA   @0.flag
   RTS

main:
   SUBROUTINE

   JSR   get_flag
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .jcmp_skip@2
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   #$D2
   CMP   #$EA
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   JSR   get_flag
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .jcmp_skip@3
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   #$D2
   CMP   #$D2
   BEQ   .if_end@5
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS

@0.flag:
   DS.B  2
