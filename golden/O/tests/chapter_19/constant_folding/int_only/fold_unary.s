SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__3	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def target_complement params= locals= indirect=false in_cycle=false
; def target_negate params= locals= indirect=false in_cycle=false
; def target_negate_zero params= locals= indirect=false in_cycle=false
; def target_not params= locals= indirect=false in_cycle=false
; def target_not_zero params= locals= indirect=false in_cycle=false
; call main -> target_complement
; call main -> target_negate
; call main -> target_negate_zero
; call main -> target_not
; call main -> target_not_zero
; item target_negate section=.text local=false zero=false
; item target_negate_zero section=.text local=false zero=false
; item target_not section=.text local=false zero=false
; item target_not_zero section=.text local=false zero=false
; item target_complement section=.text local=false zero=false
; item main section=.text local=false zero=false
; item three section=.data local=false zero=false
; item two section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target_negate:
   SUBROUTINE

   LDX   #$FF
   LDA   #$FD
   RTS

target_negate_zero:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_not:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_not_zero:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

target_complement:
   SUBROUTINE

   LDX   #$FF
   LDA   #$FE
   RTS

main:
   SUBROUTINE

   JSR   target_negate
   STA   __local_main__3
   SEC
   LDA   #$00
   SBC   three
   STA   __local_main__1
   LDA   #$00
   SBC   three+1
   STA   __local_main__0
   CPX   __local_main__0
   BNE   .jcmp_skip@0
   LDA   __local_main__3
   CMP   __local_main__1
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   target_negate_zero
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   target_not
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   target_not_zero
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   target_complement
   STA   __local_main__3
   SEC
   LDA   #$00
   SBC   two
   STA   __local_main__1
   LDA   #$00
   SBC   two+1
   STA   __local_main__0
   CPX   __local_main__0
   BNE   .jcmp_skip@2
   LDA   __local_main__3
   CMP   __local_main__1
   BEQ   .if_end@4
.jcmp_skip@2:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

three:
   DC.W  $0003

two:
   DC.W  $0002
