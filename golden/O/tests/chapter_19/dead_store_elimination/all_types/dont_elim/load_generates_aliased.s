SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__l	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__l,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; def pass_and_return params=__zpabi_pass_and_return__ptr_0,__zpabi_pass_and_return__ptr_1 locals=__local_pass_and_return__ptr indirect=false in_cycle=false param_regs=A,X
; call main -> pass_and_return
; item pass_and_return section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

pass_and_return:
   SUBROUTINE

   RTS

main:
   SUBROUTINE

   LDX   #>__local_main__l
   LDA   #<__local_main__l
   JSR   pass_and_return
   STA   __local_main__1
   STX   __local_main__2
   LDA   #$0A
   STA   __local_main__l
   LDA   #$00
   STA   __local_main__l+1
   STA   __local_main__l+2
   STA   __local_main__l+3
   LDY   #$01
   LDA   (__local_main__1),Y
   TAX
   DEY
   LDA   (__local_main__1),Y
   RTS
