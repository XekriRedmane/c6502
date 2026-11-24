SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__x	EQU	$82
__local_get_ptr_val__0	EQU	$84

; @zp-link-meta-begin
; def get_ptr_val params= locals=__local_get_ptr_val__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__x,__local_main__3 indirect=false in_cycle=false
; def set_ptr params=__zpabi_set_ptr__arg_0,__zpabi_set_ptr__arg_1 locals=__local_set_ptr__arg_1,__local_set_ptr__arg_0 indirect=false in_cycle=false param_regs=A,X
; call main -> get_ptr_val
; call main -> set_ptr
; item set_ptr section=.text local=false zero=false
; item get_ptr_val section=.text local=false zero=false
; item main section=.text local=false zero=false
; item i section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

set_ptr:
   SUBROUTINE

   STA   i
   STX   i+1
   RTS

get_ptr_val:
   SUBROUTINE

   LDA   i
   STA   DPTR
   LDA   i+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_get_ptr_val__0
   INY
   LDA   (DPTR),Y
   TAX
   LDA   __local_get_ptr_val__0
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__x
   LDA   #$00
   STA   __local_main__x+1
   LDX   #>__local_main__x
   LDA   #<__local_main__x
   JSR   set_ptr
   LDA   #$04
   STA   __local_main__x
   LDA   #$00
   STA   __local_main__x+1
   JMP   get_ptr_val

i:
   DS.B  2
