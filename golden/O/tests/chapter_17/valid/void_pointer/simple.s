SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_malloc__size_0	EQU	$80
__zpabi_malloc__size_1	EQU	$81
__zpabi_malloc__size_2	EQU	$82
__zpabi_malloc__size_3	EQU	$83
__local_main__0	EQU	$84
__local_main__1	EQU	$85
__local_main__2	EQU	$86
__local_main__3	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; ext free params=__zpabi_free__ptr_0,__zpabi_free__ptr_1 param_regs=A,X
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; call main -> free
; call main -> malloc
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$14
   STA   __zpabi_malloc__size_0
   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JSR   malloc
   STA   __local_main__2
   STX   __local_main__3
   STA   DPTR
   STX   DPTR+1
   LDA   #$64
   LDY   #$04
   STA   (__local_main__2),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   DEY
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   STA   __local_main__0
   LDA   __local_main__2
   JSR   free
   LDX   __local_main__0
   LDA   __local_main__1
   RTS
