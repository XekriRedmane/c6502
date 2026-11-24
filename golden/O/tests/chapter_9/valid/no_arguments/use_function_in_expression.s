SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_foo__0	EQU	$84
__local_foo__1	EQU	$85

; @zp-link-meta-begin
; def bar params= locals= indirect=false in_cycle=false
; def foo params= locals=__local_foo__0,__local_foo__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; call foo -> bar
; call main -> bar
; call main -> foo
; item bar section=.text local=false zero=false
; item foo section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

bar:
   SUBROUTINE

   LDX   #$00
   LDA   #$09
   RTS

foo:
   SUBROUTINE

   JSR   bar
   STA   __local_foo__0
   STX   __local_foo__1
   ASL   __local_foo__0
   ROL   __local_foo__1
   LDX   __local_foo__1
   LDA   __local_foo__0
   RTS

main:
   SUBROUTINE

   JSR   foo
   STA   __local_main__3
   STX   __local_main__2
   JSR   bar
   STA   HARGS
   STX   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   __local_main__3
   CLC
   ADC   HARGS+4
   STA   __local_main__1
   LDA   __local_main__2
   ADC   HARGS+5
   BNE   .thread@0
   LDA   __local_main__1
   CMP   #$15
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   EOR   #$01
   LDX   #$00
   RTS
