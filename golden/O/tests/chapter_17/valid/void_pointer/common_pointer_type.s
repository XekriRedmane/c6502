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
__local_main__1	EQU	$89
__local_main__2	EQU	$8A
__local_main__3	EQU	$8B
__local_main__5	EQU	$8D
__local_main__array	EQU	$8E

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__array,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext free params=__zpabi_free__ptr_0,__zpabi_free__ptr_1 param_regs=A,X
; call main -> calloc
; call main -> free
; item main section=.text local=false zero=false
; item @5.null_ptr section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$03
   STA   __zpabi_calloc__nmemb_0
   LDA   #$02
   STA   __zpabi_calloc__size_0
   LDA   #$00
   STA   __zpabi_calloc__nmemb_1
   STA   __zpabi_calloc__nmemb_2
   STA   __zpabi_calloc__nmemb_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   JSR   calloc
   STA   __local_main__5
   LDA   #$01
   STA   __local_main__array
   LDA   #$00
   STA   __local_main__array+1
   LDY   #$02
   STY   __local_main__array+2
   STA   __local_main__array+3
   INY
   STY   __local_main__array+4
   STA   __local_main__array+5
   TXA
   ORA   __local_main__5
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   TXA
   CMP   #>__local_main__array
   BNE   .if_end@1
   LDA   __local_main__5
   CMP   #<__local_main__array
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   TXA
   CMP   #>__local_main__array
   BNE   .if_end@2
   LDA   __local_main__5
   CMP   #<__local_main__array
   BNE   .if_end@2
   LDX   #$00
   TYA
   RTS
.if_end@2:
   LDA   @5.null_ptr
   ORA   @5.null_ptr+1
   BEQ   .cond_else@3
   LDA   __local_main__5
   STA   __local_main__2
   STX   __local_main__3
   JMP   .cond_end@4
.cond_else@3:
   LDA   #<__local_main__array
   STA   __local_main__2
   LDY   #>__local_main__array
   STY   __local_main__3
.cond_end@4:
   LDY   #$02
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$02
   BEQ   .if_end@5
.jcmp_skip@0:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@5:
   LDA   __local_main__5
   JSR   free
   LDX   #$00
   TXA
   RTS

@5.null_ptr:
   DS.B  2
