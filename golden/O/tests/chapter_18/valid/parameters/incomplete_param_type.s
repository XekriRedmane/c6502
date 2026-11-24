SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_foo__blah_0	EQU	$80
__zpabi_foo__blah_1	EQU	$81
__zpabi_foo__blah_2	EQU	$82
__zpabi_foo__blah_3	EQU	$83
__local_foo__0	EQU	$85
__local_foo__1	EQU	$86

; @zp-link-meta-begin
; def foo params=__zpabi_foo__blah_0,__zpabi_foo__blah_1,__zpabi_foo__blah_2,__zpabi_foo__blah_3 locals=__local_foo__0,__local_foo__1,__local_foo__2,__local_foo__3,__local_foo__4 indirect=false in_cycle=false changes=__zpabi_foo__blah_0,__zpabi_foo__blah_1,__zpabi_foo__blah_2,__zpabi_foo__blah_3
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> foo
; item main section=.text local=false zero=false
; item foo section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_foo__blah_0
   LDA   #$00
   STA   __zpabi_foo__blah_1
   LDY   #$02
   STY   __zpabi_foo__blah_2
   STA   __zpabi_foo__blah_3
   JMP   foo

foo:
   SUBROUTINE

   LDA   __zpabi_foo__blah_0+2
   STA   __local_foo__0
   LDA   __zpabi_foo__blah_0+3
   STA   __local_foo__1
   LDA   __zpabi_foo__blah_0
   CLC
   ADC   __local_foo__0
   STA   __local_foo__0
   LDA   __zpabi_foo__blah_0+1
   ADC   __local_foo__1
   TAX
   LDA   __local_foo__0
   RTS
