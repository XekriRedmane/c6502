SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__2	EQU	$82
__local_main__3	EQU	$83

; @zp-link-meta-begin
; def bar params= locals= indirect=false in_cycle=false
; def foo params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; call main -> bar
; call main -> foo
; item foo section=.text local=false zero=false
; item bar section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @0.a section=.data local=true zero=false
; item @1.a section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

foo:
   SUBROUTINE

   ASL   @0.a
   ROL   @0.a+1
   LDX   @0.a+1
   LDA   @0.a
   RTS

bar:
   SUBROUTINE

   INC   @1.a
   BNE   .inc_done@0
   INC   @1.a+1
.inc_done@0:
   LDX   @1.a+1
   LDA   @1.a
   RTS

main:
   SUBROUTINE

   JSR   foo
   STA   __local_main__3
   STX   __local_main__2
   JSR   bar
   CLC
   ADC   __local_main__3
   STA   __local_main__3
   TXA
   ADC   __local_main__2
   STA   __local_main__2
   JSR   foo
   CLC
   ADC   __local_main__3
   STA   __local_main__3
   TXA
   ADC   __local_main__2
   STA   __local_main__2
   JSR   bar
   CLC
   ADC   __local_main__3
   STA   __local_main__0
   TXA
   ADC   __local_main__2
   TAX
   LDA   __local_main__0
   RTS

@0.a:
   DC.W  $0003

@1.a:
   DC.W  $0004
