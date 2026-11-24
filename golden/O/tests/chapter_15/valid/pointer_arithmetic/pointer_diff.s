SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_get_long_ptr_diff__ptr2_0	EQU	$80
__zpabi_get_multidim_ptr_diff_2__ptr2_0	EQU	$80
__zpabi_get_multidim_ptr_diff__ptr2_0	EQU	$80
__zpabi_get_ptr_diff__ptr2_0	EQU	$80
__zpabi_get_long_ptr_diff__ptr2_1	EQU	$81
__zpabi_get_multidim_ptr_diff_2__ptr2_1	EQU	$81
__zpabi_get_multidim_ptr_diff__ptr2_1	EQU	$81
__zpabi_get_ptr_diff__ptr2_1	EQU	$81
__local_main__1	EQU	$83
__local_main__2	EQU	$84
__local_main__4	EQU	$86
__local_main__arr	EQU	$87
__local_main__long_arr	EQU	$91
__local_get_long_ptr_diff__ptr1_1	EQU	$B1
__local_get_multidim_ptr_diff_2__ptr1_1	EQU	$B1
__local_get_multidim_ptr_diff__ptr1_1	EQU	$B1
__local_get_ptr_diff__ptr1_1	EQU	$B1
__local_get_long_ptr_diff__ptr1_0	EQU	$B2
__local_get_multidim_ptr_diff_2__ptr1_0	EQU	$B2
__local_get_multidim_ptr_diff__ptr1_0	EQU	$B2
__local_get_ptr_diff__ptr1_0	EQU	$B2

; @zp-link-meta-begin
; def get_long_ptr_diff params=__zpabi_get_long_ptr_diff__ptr1_0,__zpabi_get_long_ptr_diff__ptr1_1,__zpabi_get_long_ptr_diff__ptr2_0,__zpabi_get_long_ptr_diff__ptr2_1 locals=__local_get_long_ptr_diff__ptr1_1,__local_get_long_ptr_diff__ptr1_0 indirect=false in_cycle=false param_regs=A,X,,
; def get_multidim_ptr_diff params=__zpabi_get_multidim_ptr_diff__ptr1_0,__zpabi_get_multidim_ptr_diff__ptr1_1,__zpabi_get_multidim_ptr_diff__ptr2_0,__zpabi_get_multidim_ptr_diff__ptr2_1 locals=__local_get_multidim_ptr_diff__ptr1_1,__local_get_multidim_ptr_diff__ptr1_0 indirect=false in_cycle=false param_regs=A,X,,
; def get_multidim_ptr_diff_2 params=__zpabi_get_multidim_ptr_diff_2__ptr1_0,__zpabi_get_multidim_ptr_diff_2__ptr1_1,__zpabi_get_multidim_ptr_diff_2__ptr2_0,__zpabi_get_multidim_ptr_diff_2__ptr2_1 locals=__local_get_multidim_ptr_diff_2__ptr1_1,__local_get_multidim_ptr_diff_2__ptr1_0 indirect=false in_cycle=false param_regs=A,X,,
; def get_ptr_diff params=__zpabi_get_ptr_diff__ptr1_0,__zpabi_get_ptr_diff__ptr1_1,__zpabi_get_ptr_diff__ptr2_0,__zpabi_get_ptr_diff__ptr2_1 locals=__local_get_ptr_diff__ptr1_1,__local_get_ptr_diff__ptr1_0 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__arr,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__long_arr,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37,__local_main__38,__local_main__39,__local_main__40,__local_main__41,__local_main__42,__local_main__43,__local_main__44,__local_main__45,__local_main__46 indirect=false in_cycle=false
; call main -> get_long_ptr_diff
; call main -> get_multidim_ptr_diff
; call main -> get_multidim_ptr_diff_2
; call main -> get_ptr_diff
; item get_ptr_diff section=.text local=false zero=false
; item get_ptr_diff@tail0 section=.text local=true zero=false
; item get_long_ptr_diff section=.text local=false zero=false
; item get_multidim_ptr_diff section=.text local=false zero=false
; item get_multidim_ptr_diff_2 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @11.multidim section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

get_ptr_diff:
   SUBROUTINE

   STA   __local_get_ptr_diff__ptr1_0
   STX   __local_get_ptr_diff__ptr1_1
   LDA   __zpabi_get_ptr_diff__ptr2_0
   SEC
   SBC   __local_get_ptr_diff__ptr1_0
   STA   HARGS
   LDA   __zpabi_get_ptr_diff__ptr2_1
   SBC   __local_get_ptr_diff__ptr1_1
   STA   HARGS+1
   LDA   #$02
   JMP   get_ptr_diff@tail0

get_ptr_diff@tail0:
   SUBROUTINE

   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDX   HARGS+5
   RTS

get_long_ptr_diff:
   SUBROUTINE

   STA   __local_get_long_ptr_diff__ptr1_0
   STX   __local_get_long_ptr_diff__ptr1_1
   LDA   __zpabi_get_long_ptr_diff__ptr2_0
   SEC
   SBC   __local_get_long_ptr_diff__ptr1_0
   STA   HARGS
   LDA   __zpabi_get_long_ptr_diff__ptr2_1
   SBC   __local_get_long_ptr_diff__ptr1_1
   STA   HARGS+1
   LDA   #$04
   JMP   get_ptr_diff@tail0

get_multidim_ptr_diff:
   SUBROUTINE

   STA   __local_get_multidim_ptr_diff__ptr1_0
   STX   __local_get_multidim_ptr_diff__ptr1_1
   LDA   __zpabi_get_multidim_ptr_diff__ptr2_0
   SEC
   SBC   __local_get_multidim_ptr_diff__ptr1_0
   STA   HARGS
   LDA   __zpabi_get_multidim_ptr_diff__ptr2_1
   SBC   __local_get_multidim_ptr_diff__ptr1_1
   STA   HARGS+1
   LDA   #$78
   JMP   get_ptr_diff@tail0

get_multidim_ptr_diff_2:
   SUBROUTINE

   STA   __local_get_multidim_ptr_diff_2__ptr1_0
   STX   __local_get_multidim_ptr_diff_2__ptr1_1
   LDA   __zpabi_get_multidim_ptr_diff_2__ptr2_0
   SEC
   SBC   __local_get_multidim_ptr_diff_2__ptr1_0
   STA   HARGS
   LDA   __zpabi_get_multidim_ptr_diff_2__ptr2_1
   SBC   __local_get_multidim_ptr_diff_2__ptr1_1
   STA   HARGS+1
   LDA   #$28
   JMP   get_ptr_diff@tail0

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__arr
   LDA   #$00
   STA   __local_main__arr+1
   LDY   #$04
   STY   __local_main__arr+2
   STA   __local_main__arr+3
   DEY
   STY   __local_main__arr+4
   STA   __local_main__arr+5
   DEY
   STY   __local_main__arr+6
   STA   __local_main__arr+7
   DEY
   STY   __local_main__arr+8
   STA   __local_main__arr+9
   LDA   #<(__local_main__arr+10)
   STA   __zpabi_get_ptr_diff__ptr2_0
   LDA   #>(__local_main__arr+10)
   STA   __zpabi_get_ptr_diff__ptr2_1
   LDX   #>__local_main__arr
   LDA   #<__local_main__arr
   JSR   get_ptr_diff
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$05
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #>__local_main__long_arr
   STA   __local_main__4
   LDA   #<__local_main__long_arr
   CLC
   ADC   #$0C
   STA   __local_main__2
   BCC   .carry_inc@0
   INC   __local_main__4
.carry_inc@0:
   LDA   #<__local_main__long_arr
   STA   __zpabi_get_long_ptr_diff__ptr2_0
   LDA   #>__local_main__long_arr
   STA   __zpabi_get_long_ptr_diff__ptr2_1
   LDX   __local_main__4
   LDA   __local_main__2
   JSR   get_long_ptr_diff
   CPX   #$FF
   BNE   .jcmp_skip@1
   CMP   #$FD
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #>(@11.multidim+1800)
   LDA   #<(@11.multidim+2160)
   STA   __zpabi_get_multidim_ptr_diff__ptr2_0
   LDA   #>(@11.multidim+2160)
   STA   __zpabi_get_multidim_ptr_diff__ptr2_1
   LDA   #<(@11.multidim+1800)
   JSR   get_multidim_ptr_diff
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$03
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #>(@11.multidim+2000)
   LDA   #<(@11.multidim+1920)
   STA   __zpabi_get_multidim_ptr_diff_2__ptr2_0
   LDA   #>(@11.multidim+1920)
   STA   __zpabi_get_multidim_ptr_diff_2__ptr2_1
   LDA   #<(@11.multidim+2000)
   JSR   get_multidim_ptr_diff_2
   CPX   #$FF
   BNE   .jcmp_skip@3
   CMP   #$FE
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

@11.multidim:
   DS.B  5040
