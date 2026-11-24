SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_calloc__size_0	EQU	$84
__zpabi_calloc__size_1	EQU	$85
__zpabi_calloc__size_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87
__local_main__0	EQU	$88
__local_main__1	EQU	$89
__local_main__2	EQU	$8A
__local_main__4	EQU	$8C
__local_main__5	EQU	$8D
__local_main__i	EQU	$8E
__local_main__arr	EQU	$90

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__i,__local_main__7,__local_main__arr,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext free params=__zpabi_free__ptr_0,__zpabi_free__ptr_1 param_regs=A,X
; call main -> calloc
; call main -> free
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$0A
   STA   __local_main__i
   LDA   #$00
   STA   __local_main__i+1
   STA   __zpabi_calloc__nmemb_1
   STA   __zpabi_calloc__nmemb_2
   STA   __zpabi_calloc__nmemb_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   LDA   #$02
   STA   __zpabi_calloc__nmemb_0
   STA   __zpabi_calloc__size_0
   JSR   calloc
   STA   __local_main__arr
   STX   __local_main__arr+1
   LDA   #<__local_main__i
   STA   __local_main__arr+2
   LDA   #>__local_main__i
   STA   __local_main__arr+3
   LDA   #$00
   STA   __local_main__arr+4
   STA   __local_main__arr+5
   LDA   #<__local_main__arr
   STA   __local_main__arr+6
   LDA   #>__local_main__arr
   STA   __local_main__arr+7
   LDA   __local_main__arr
   STA   __local_main__4
   STX   __local_main__5
   LDY   #$01
   LDA   (__local_main__4),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__0
   LDY   #$00
   LDA   (__local_main__4),Y
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_main__i
   STA   __local_main__1
   LDA   __local_main__i+1
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$00
   LDY   #$00
   STY   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #>__local_main__arr
   CMP   #>__local_main__arr
   BNE   .jcmp_skip@1
   LDA   #<__local_main__arr
   CMP   #<__local_main__arr
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __local_main__4
   JSR   free
   LDX   #$00
   TXA
   RTS
