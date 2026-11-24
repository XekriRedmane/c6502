SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_pointers_to_pointers__0	EQU	$80
__zpabi_two_pointers__val_0	EQU	$80
__local_pointers_to_pointers__1	EQU	$81
__zpabi_two_pointers__val_1	EQU	$81
__local_pointers_to_pointers__p	EQU	$82
__zpabi_two_pointers__val_2	EQU	$82
__local_pointers_to_pointers__2	EQU	$83
__zpabi_two_pointers__val_3	EQU	$83
__zpabi_two_pointers__val_4	EQU	$84
__zpabi_two_pointers__val_5	EQU	$85
__zpabi_two_pointers__val_6	EQU	$86
__zpabi_two_pointers__val_7	EQU	$87
__zpabi_two_pointers__ptr_0	EQU	$88
__zpabi_two_pointers__ptr_1	EQU	$89
__local_main__0	EQU	$8A
__local_main__1	EQU	$8B
__local_main__2	EQU	$8C
__local_main__3	EQU	$8D
__local_main__4	EQU	$8E
__local_main__5	EQU	$8F
__local_main__6	EQU	$90
__local_main__7	EQU	$91
__local_main__8	EQU	$92
__local_main__9	EQU	$93
__local_main__i	EQU	$94
__local_main__i_ptr	EQU	$96
__local_main__d1	EQU	$98

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__i,__local_main__11,__local_main__i_ptr,__local_main__13,__local_main__d1,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21 indirect=false in_cycle=false
; def pointers_to_pointers params=__zpabi_pointers_to_pointers__p_0,__zpabi_pointers_to_pointers__p_1 locals=__local_pointers_to_pointers__0,__local_pointers_to_pointers__1,__local_pointers_to_pointers__p,__local_pointers_to_pointers__2 indirect=false in_cycle=false param_regs=A,X
; def return_3 params= locals= indirect=false in_cycle=false
; def two_pointers params=__zpabi_two_pointers__val_0,__zpabi_two_pointers__val_1,__zpabi_two_pointers__val_2,__zpabi_two_pointers__val_3,__zpabi_two_pointers__val_4,__zpabi_two_pointers__val_5,__zpabi_two_pointers__val_6,__zpabi_two_pointers__val_7,__zpabi_two_pointers__ptr_0,__zpabi_two_pointers__ptr_1 locals= indirect=false in_cycle=false
; call main -> pointers_to_pointers
; call main -> return_3
; call main -> two_pointers
; share member=two_pointers:__zpabi_two_pointers__ptr_0,__zpabi_two_pointers__ptr_1 holder=main live=main:return_3,two_pointers forward=main>two_pointers writer=main
; item return_3 section=.text local=false zero=false
; item two_pointers section=.text local=false zero=false
; item pointers_to_pointers section=.text local=false zero=false
; item main section=.text local=false zero=false
; item l section=.data local=false zero=false
; item @9.u section=.bss local=true zero=true
; item @10.u_ptr section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

return_3:
   SUBROUTINE

   LDX   #$00
   LDA   #$03
   RTS

two_pointers:
   SUBROUTINE

   LDA   __zpabi_two_pointers__val_0
   LDY   #$00
   STA   (__zpabi_two_pointers__ptr_0),Y
   LDA   __zpabi_two_pointers__val_1
   INY
   STA   (__zpabi_two_pointers__ptr_0),Y
   LDA   __zpabi_two_pointers__val_2
   INY
   STA   (__zpabi_two_pointers__ptr_0),Y
   LDA   __zpabi_two_pointers__val_3
   INY
   STA   (__zpabi_two_pointers__ptr_0),Y
   LDA   __zpabi_two_pointers__val_4
   INY
   STA   (__zpabi_two_pointers__ptr_0),Y
   LDA   __zpabi_two_pointers__val_5
   INY
   STA   (__zpabi_two_pointers__ptr_0),Y
   LDA   __zpabi_two_pointers__val_6
   INY
   STA   (__zpabi_two_pointers__ptr_0),Y
   LDA   __zpabi_two_pointers__val_7
   INY
   STA   (__zpabi_two_pointers__ptr_0),Y
   LDX   #>l
   LDA   #<l
   RTS

pointers_to_pointers:
   SUBROUTINE

   STA   __local_pointers_to_pointers__p
   STX   __local_pointers_to_pointers__2
   LDA   #<@9.u
   STA   @10.u_ptr
   LDA   #>@9.u
   STA   @10.u_ptr+1
   LDY   #$00
   LDA   (__local_pointers_to_pointers__p),Y
   STA   __local_pointers_to_pointers__0
   INY
   LDA   (__local_pointers_to_pointers__p),Y
   STA   __local_pointers_to_pointers__1
   DEY
   LDA   (__local_pointers_to_pointers__0),Y
   STA   @9.u
   INY
   LDA   (__local_pointers_to_pointers__0),Y
   STA   @9.u+1
   LDX   #>@10.u_ptr
   LDA   #<@10.u_ptr
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__i
   STA   __local_main__i+1
   LDA   #<__local_main__i
   STA   __local_main__i_ptr
   LDA   #<__local_main__i_ptr
   STA   __local_main__8
   LDA   #>__local_main__i_ptr
   STA   __local_main__9
   LDA   #$00
   STA   __local_main__d1
   STA   __local_main__d1+1
   STA   __local_main__d1+2
   STA   __local_main__d1+3
   STA   __local_main__d1+4
   STA   __local_main__d1+5
   STA   __local_main__d1+6
   STA   __local_main__d1+7
   LDA   #<__local_main__d1
   STA   __zpabi_two_pointers__ptr_0
   LDA   #>__local_main__d1
   STA   __zpabi_two_pointers__ptr_0+1
   JSR   return_3
   STA   __local_main__i
   STX   __local_main__i+1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__i
   CMP   #$03
   BEQ   .vthread@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.vthread@0:
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$03
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$00
   STA   __zpabi_two_pointers__val_0
   STA   __zpabi_two_pointers__val_1
   STA   __zpabi_two_pointers__val_2
   STA   __zpabi_two_pointers__val_3
   STA   __zpabi_two_pointers__val_4
   STA   __zpabi_two_pointers__val_5
   LDA   #$24
   STA   __zpabi_two_pointers__val_6
   LDA   #$40
   STA   __zpabi_two_pointers__val_7
   JSR   two_pointers
   STA   __local_main__4
   STX   __local_main__5
   TXA
   CMP   #>l
   BNE   .jcmp_skip@2
   LDA   __local_main__4
   CMP   #<l
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDY   #$00
   LDA   (__local_main__4),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__4),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__4),Y
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   BNE   .jcmp_skip@3
   LDA   __local_main__2
   BNE   .jcmp_skip@3
   LDA   __local_main__3
   CMP   #$64
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDY   #$00
   LDA   (__zpabi_two_pointers__ptr_0),Y
   STA   __local_main__7
   INY
   LDA   (__zpabi_two_pointers__ptr_0),Y
   STA   __local_main__6
   INY
   LDA   (__zpabi_two_pointers__ptr_0),Y
   STA   __local_main__5
   INY
   LDA   (__zpabi_two_pointers__ptr_0),Y
   STA   __local_main__4
   INY
   LDA   (__zpabi_two_pointers__ptr_0),Y
   STA   __local_main__3
   INY
   LDA   (__zpabi_two_pointers__ptr_0),Y
   STA   __local_main__2
   INY
   LDA   (__zpabi_two_pointers__ptr_0),Y
   STA   __local_main__1
   INY
   LDA   (__zpabi_two_pointers__ptr_0),Y
   CMP   #$40
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   CMP   #$24
   BNE   .jcmp_skip@4
   LDA   __local_main__2
   BNE   .jcmp_skip@4
   LDA   __local_main__3
   BNE   .jcmp_skip@4
   LDA   __local_main__4
   BNE   .jcmp_skip@4
   LDA   __local_main__5
   BNE   .jcmp_skip@4
   LDA   __local_main__6
   BNE   .jcmp_skip@4
   LDA   __local_main__7
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   __local_main__d1+7
   CMP   #$40
   BNE   .jcmp_skip@5
   LDA   __local_main__d1+6
   CMP   #$24
   BNE   .jcmp_skip@5
   LDA   __local_main__d1+5
   BNE   .jcmp_skip@5
   LDA   __local_main__d1+4
   BNE   .jcmp_skip@5
   LDA   __local_main__d1+3
   BNE   .jcmp_skip@5
   LDA   __local_main__d1+2
   BNE   .jcmp_skip@5
   LDA   __local_main__d1+1
   BNE   .jcmp_skip@5
   LDA   __local_main__d1
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDX   __local_main__9
   LDA   __local_main__8
   JSR   pointers_to_pointers
   STA   __local_main__0
   STX   __local_main__1
   LDY   #$00
   LDA   (__local_main__0),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__0),Y
   STA   __local_main__3
   DEY
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@6
   LDA   __local_main__1
   CMP   #$03
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDX   #$00
   TXA
   RTS

l:
   DC.L  $00000064

@9.u:
   DS.B  2

@10.u_ptr:
   DS.B  2
