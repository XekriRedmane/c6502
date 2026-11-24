SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_aliased_dst__x_0	EQU	$80
__zpabi_aliased_src__x_0	EQU	$80
__zpabi_aliased_dst__x_1	EQU	$81
__zpabi_aliased_src__x_1	EQU	$81
__zpabi_aliased_dst__ptr_0	EQU	$82
__zpabi_aliased_src__ptr_0	EQU	$82
__zpabi_aliased_dst__ptr_1	EQU	$83
__zpabi_aliased_src__ptr_1	EQU	$83
__local_main__1	EQU	$85
__local_main__i	EQU	$86
__local_aliased_dst__ptr	EQU	$88
__local_aliased_src__ptr	EQU	$88
__local_aliased_dst__0	EQU	$89
__local_aliased_src__0	EQU	$89
__local_aliased_dst__flag_0	EQU	$8B
__local_aliased_src__y_0	EQU	$8B
__local_aliased_dst__y	EQU	$8C
__local_aliased_src__flag_0	EQU	$8D

; @zp-link-meta-begin
; def aliased_dst params=__zpabi_aliased_dst__flag_0,__zpabi_aliased_dst__flag_1,__zpabi_aliased_dst__x_0,__zpabi_aliased_dst__x_1,__zpabi_aliased_dst__ptr_0,__zpabi_aliased_dst__ptr_1 locals=__local_aliased_dst__ptr,__local_aliased_dst__0,__local_aliased_dst__flag_1,__local_aliased_dst__flag_0,__local_aliased_dst__y,__local_aliased_dst__2 indirect=false in_cycle=false param_regs=A,X,,,,
; def aliased_src params=__zpabi_aliased_src__flag_0,__zpabi_aliased_src__flag_1,__zpabi_aliased_src__x_0,__zpabi_aliased_src__x_1,__zpabi_aliased_src__ptr_0,__zpabi_aliased_src__ptr_1 locals=__local_aliased_src__ptr,__local_aliased_src__0,__local_aliased_src__y_1,__local_aliased_src__y_0,__local_aliased_src__flag_1,__local_aliased_src__flag_0 indirect=false in_cycle=false param_regs=A,X,,,, changes=__zpabi_aliased_src__x_0,__zpabi_aliased_src__x_1
; def main params= locals=__local_main__0,__local_main__1,__local_main__i,__local_main__3 indirect=false in_cycle=false
; call main -> aliased_dst
; call main -> aliased_src
; item aliased_src section=.text local=false zero=false
; item aliased_dst section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

aliased_src:
   SUBROUTINE

   STA   __local_aliased_src__flag_0
   LDA   __zpabi_aliased_src__x_0
   STA   __local_aliased_src__y_0
   LDA   __zpabi_aliased_src__ptr_0
   STA   __local_aliased_src__ptr
   LDA   __zpabi_aliased_src__ptr_1
   STA   __local_aliased_src__0
   TXA
   ORA   __local_aliased_src__flag_0
   BEQ   .if_end@0
   LDA   #<__zpabi_aliased_src__x_0
   STA   __local_aliased_src__ptr
   LDY   #>__zpabi_aliased_src__x_0
   STY   __local_aliased_src__0
.if_end@0:
   LDA   #$64
   LDY   #$00
   STA   (__local_aliased_src__ptr),Y
   LDA   #$00
   INY
   STA   (__local_aliased_src__ptr),Y
   LDX   __zpabi_aliased_src__x_1
   LDA   __local_aliased_src__y_0
   RTS

aliased_dst:
   SUBROUTINE

   STA   __local_aliased_dst__flag_0
   LDA   __zpabi_aliased_dst__x_0
   STA   __local_aliased_dst__y
   LDA   __zpabi_aliased_dst__x_1
   STA   __local_aliased_dst__y+1
   LDA   __zpabi_aliased_dst__ptr_0
   STA   __local_aliased_dst__ptr
   LDA   __zpabi_aliased_dst__ptr_1
   STA   __local_aliased_dst__0
   TXA
   ORA   __local_aliased_dst__flag_0
   BEQ   .if_end@1
   LDA   #<__local_aliased_dst__y
   STA   __local_aliased_dst__ptr
   LDY   #>__local_aliased_dst__y
   STY   __local_aliased_dst__0
.if_end@1:
   LDA   #$64
   LDY   #$00
   STA   (__local_aliased_dst__ptr),Y
   LDA   #$00
   INY
   STA   (__local_aliased_dst__ptr),Y
   LDX   __local_aliased_dst__y+1
   LDA   __local_aliased_dst__y
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__i
   STA   __local_main__i+1
   LDA   #$01
   STA   __zpabi_aliased_src__x_0
   JSR   main@run1
   TXA
   JSR   aliased_src
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDA   __local_main__i+1
   BNE   .jcmp_skip@1
   LDA   __local_main__i
   CMP   #$64
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   LDA   #$00
   STA   __local_main__i
   STA   __local_main__i+1
   LDA   #$02
   STA   __zpabi_aliased_src__x_0
   JSR   main@run1
   LDA   #$01
   JSR   aliased_src
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$02
   BEQ   .if_end@4
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@4:
   LDA   __local_main__i
   ORA   __local_main__i+1
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$04
   RTS
.if_end@5:
   JSR   main@run0
   TXA
   JSR   aliased_dst
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$05
   BEQ   .if_end@6
.jcmp_skip@3:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@6:
   LDA   __local_main__i+1
   BNE   .jcmp_skip@4
   LDA   __local_main__i
   CMP   #$64
   BEQ   .if_end@7
.jcmp_skip@4:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@7:
   LDA   #$00
   STA   __local_main__i
   STA   __local_main__i+1
   JSR   main@run0
   LDA   #$01
   JSR   aliased_dst
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@5
   LDA   __local_main__1
   CMP   #$64
   BEQ   .if_end@8
.jcmp_skip@5:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@8:
   LDA   __local_main__i
   ORA   __local_main__i+1
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$08
   RTS
.if_end@9:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_aliased_src__x_1
   LDA   #<__local_main__i
   STA   __zpabi_aliased_src__ptr_0
   LDA   #>__local_main__i
   STA   __zpabi_aliased_src__ptr_1
   LDX   #$00
   RTS

main@run0:
   SUBROUTINE

   LDA   #$05
   STA   __zpabi_aliased_dst__x_0
   LDA   #$00
   STA   __zpabi_aliased_dst__x_1
   LDA   #<__local_main__i
   STA   __zpabi_aliased_dst__ptr_0
   LDA   #>__local_main__i
   STA   __zpabi_aliased_dst__ptr_1
   LDX   #$00
   RTS
