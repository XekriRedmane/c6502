SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__2	EQU	$82
__local_main__flat_arr	EQU	$83
__local_main__nested_array	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__flat_arr,__local_main__4,__local_main__5,__local_main__6,__local_main__nested_array,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18 indirect=false in_cycle=false
; ext puts params=__zpabi_puts__s_0,__zpabi_puts__s_1 param_regs=A,X
; call main -> puts
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$61
   STA   __local_main__flat_arr
   LDA   #$62
   STA   __local_main__flat_arr+1
   LDA   #$63
   STA   __local_main__flat_arr+2
   LDA   #$00
   STA   __local_main__flat_arr+3
   LDX   #>__local_main__flat_arr
   LDA   #<__local_main__flat_arr
   JSR   puts
   LDA   #$78
   STA   __local_main__flat_arr+2
   LDX   #>__local_main__flat_arr
   LDA   #<__local_main__flat_arr
   JSR   puts
   LDA   #$48
   STA   __local_main__nested_array
   LDA   #$65
   STA   __local_main__nested_array+1
   LDA   #$6C
   STA   __local_main__nested_array+2
   STA   __local_main__nested_array+3
   STA   __local_main__nested_array+9
   LDA   #$6F
   STA   __local_main__nested_array+4
   STA   __local_main__nested_array+7
   LDA   #$00
   STA   __local_main__nested_array+5
   STA   __local_main__nested_array+11
   LDA   #$57
   STA   __local_main__nested_array+6
   LDA   #$72
   STA   __local_main__nested_array+8
   LDA   #$64
   STA   __local_main__nested_array+10
   LDA   #>__local_main__nested_array
   STA   __local_main__2
   LDA   #<__local_main__nested_array
   CLC
   BCC   .carry_inc@0
   INC   __local_main__2
.carry_inc@0:
   LDX   __local_main__2
   JSR   puts
   LDA   #>__local_main__nested_array
   STA   __local_main__2
   LDA   #<__local_main__nested_array
   CLC
   ADC   #$06
   BCC   .carry_inc@1
   INC   __local_main__2
.carry_inc@1:
   LDX   __local_main__2
   JSR   puts
   LDA   #$4A
   STA   __local_main__nested_array
   LDA   #>__local_main__nested_array
   STA   __local_main__2
   LDA   #<__local_main__nested_array
   CLC
   BCC   .carry_inc@2
   INC   __local_main__2
.carry_inc@2:
   LDX   __local_main__2
   JSR   puts
   LDX   #$00
   TXA
   RTS
