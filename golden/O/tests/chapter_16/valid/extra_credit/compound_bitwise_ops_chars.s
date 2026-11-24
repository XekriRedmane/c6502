SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_do_signed_ops__u_arr_0	EQU	$80
__zpabi_do_signed_ops__u_arr_1	EQU	$81
__local_main__1	EQU	$83
__local_main__arr	EQU	$86
__local_main__u_arr	EQU	$8B
__local_do_unsigned_ops__0	EQU	$8F
__local_do_signed_ops__1	EQU	$90
__local_do_signed_ops__arr	EQU	$91
__local_do_unsigned_ops__u_arr	EQU	$91
__local_do_signed_ops__2	EQU	$92
__local_do_unsigned_ops__2	EQU	$92

; @zp-link-meta-begin
; def do_signed_ops params=__zpabi_do_signed_ops__arr_0,__zpabi_do_signed_ops__arr_1,__zpabi_do_signed_ops__u_arr_0,__zpabi_do_signed_ops__u_arr_1 locals=__local_do_signed_ops__0,__local_do_signed_ops__1,__local_do_signed_ops__arr,__local_do_signed_ops__2 indirect=false in_cycle=false param_regs=A,X,,
; def do_unsigned_ops params=__zpabi_do_unsigned_ops__u_arr_0,__zpabi_do_unsigned_ops__u_arr_1 locals=__local_do_unsigned_ops__0,__local_do_unsigned_ops__1,__local_do_unsigned_ops__u_arr,__local_do_unsigned_ops__2 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__arr,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__u_arr,__local_main__10,__local_main__11,__local_main__12 indirect=false in_cycle=false
; call main -> do_signed_ops
; call main -> do_unsigned_ops
; item do_signed_ops section=.text local=false zero=false
; item do_unsigned_ops section=.text local=false zero=false
; item main section=.text local=false zero=false
; item x section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

do_signed_ops:
   SUBROUTINE

   STA   __local_do_signed_ops__arr
   STX   __local_do_signed_ops__2
   LDY   #$00
   LDA   (__local_do_signed_ops__arr),Y
   EOR   #$39
   STA   (__local_do_signed_ops__arr),Y
   INY
   LDA   (__local_do_signed_ops__arr),Y
   LDY   #$03
   ORA   (__zpabi_do_signed_ops__u_arr_0),Y
   LDY   #$01
   STA   (__local_do_signed_ops__arr),Y
   INY
   LDA   (__local_do_signed_ops__arr),Y
   STA   __local_do_signed_ops__1
   DEY
   LDA   (__zpabi_do_signed_ops__u_arr_0),Y
   SEC
   SBC   #$B9
   AND   __local_do_signed_ops__1
   INY
   STA   (__local_do_signed_ops__arr),Y
   INY
   LDA   (__local_do_signed_ops__arr),Y
   LSR
   LDA   #$00
   ROR
   STA   (__local_do_signed_ops__arr),Y
   INY
   LDA   (__local_do_signed_ops__arr),Y
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   (__local_do_signed_ops__arr),Y
   LDX   #$00
   TXA
   RTS

do_unsigned_ops:
   SUBROUTINE

   STA   __local_do_unsigned_ops__u_arr
   STX   __local_do_unsigned_ops__2
   LDY   #$03
   LDA   #$00
   STA   (__local_do_unsigned_ops__u_arr),Y
   DEY
   LDA   (__local_do_unsigned_ops__u_arr),Y
   STA   __local_do_unsigned_ops__0
   LDX   x
   DEX
   BEQ   .shift_done@1
.shift_loop@0:
   LSR   __local_do_unsigned_ops__0
   DEX
   BNE   .shift_loop@0
.shift_done@1:
   LDA   __local_do_unsigned_ops__0
   STA   (__local_do_unsigned_ops__u_arr),Y
   LDY   #$01
   LDA   (__local_do_unsigned_ops__u_arr),Y
   ORA   #$71
   STA   (__local_do_unsigned_ops__u_arr),Y
   LDA   #$38
   STA   x
   LDA   #$FB
   STA   x+1
   LDA   #$FF
   STA   x+2
   STA   x+3
   DEY
   LDA   (__local_do_unsigned_ops__u_arr),Y
   EOR   #$38
   STA   (__local_do_unsigned_ops__u_arr),Y
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #$80
   STA   __local_main__arr
   LDA   #$88
   STA   __local_main__arr+1
   LDA   #$FE
   STA   __local_main__arr+2
   LDA   #$01
   STA   __local_main__arr+3
   LDA   #$78
   STA   __local_main__arr+4
   LDA   #$00
   STA   __local_main__u_arr
   LDA   #$AA
   STA   __local_main__u_arr+1
   LDA   #$FA
   STA   __local_main__u_arr+2
   LDA   #$FF
   STA   __local_main__u_arr+3
   LDA   #<__local_main__u_arr
   STA   __zpabi_do_signed_ops__u_arr_0
   LDA   #>__local_main__u_arr
   STA   __zpabi_do_signed_ops__u_arr_1
   LDX   #>__local_main__arr
   LDA   #<__local_main__arr
   JSR   do_signed_ops
   LDX   #>__local_main__u_arr
   LDA   #<__local_main__u_arr
   JSR   do_unsigned_ops
   LDA   __local_main__arr
   BPL   .jcmp_skip@4
   CMP   #$B9
   BEQ   .if_end@0
.jcmp_skip@4:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_main__arr+1
   BPL   .jcmp_skip@7
   CMP   #$FF
   BEQ   .if_end@1
.jcmp_skip@7:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_main__arr+2
   BPL   .jcmp_skip@10
   CMP   #$F0
   BEQ   .if_end@2
.jcmp_skip@10:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__arr+3
   BPL   .jcmp_skip@13
   CMP   #$80
   BEQ   .if_end@3
.jcmp_skip@13:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __local_main__arr+4
   STA   __local_main__1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   ORA   __local_main__1
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   __local_main__u_arr
   CMP   #$A8
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   __local_main__u_arr+1
   CMP   #$FB
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   __local_main__u_arr+2
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   __local_main__u_arr+3
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDX   #$00
   TXA
   RTS

x:
   DC.L  $00000008
