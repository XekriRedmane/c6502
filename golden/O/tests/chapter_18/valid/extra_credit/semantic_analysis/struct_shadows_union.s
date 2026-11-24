SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_malloc__size_0	EQU	$80
__zpabi_malloc__size_1	EQU	$81
__zpabi_malloc__size_2	EQU	$82
__zpabi_malloc__size_3	EQU	$83
__local_main__1	EQU	$85
__local_main__2	EQU	$86
__local_main__3	EQU	$87
__local_main__my_struct_3	EQU	$88
__local_main__my_struct_2	EQU	$89

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__my_struct_3,__local_main__my_struct_2 indirect=false in_cycle=false
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; call main -> malloc
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__my_struct_3
   LDA   #$0D
   STA   __local_main__my_struct_2
   LDA   #$04
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
   LDA   #$0A
   LDY   #$00
   STA   (__local_main__2),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   DEY
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_main__my_struct_3
   BNE   .jcmp_skip@1
   LDA   __local_main__my_struct_2
   CMP   #$0D
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS
