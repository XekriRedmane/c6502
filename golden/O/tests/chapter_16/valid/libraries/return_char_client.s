SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__char_array_2	EQU	$8B
__local_main__char_array_1	EQU	$8C
__local_main__char_array_0	EQU	$8D

; @zp-link-meta-begin
; def main params= locals=__local_main__char_array2_2,__local_main__char_array4_1,__local_main__char_array4_0,__local_main__0,__local_main__char_array3_2,__local_main__char_array3_1,__local_main__char_array3_0,__local_main__1,__local_main__char_array2_1,__local_main__char_array2_0,__local_main__2,__local_main__char_array_2,__local_main__char_array_1,__local_main__char_array_0,__local_main__3 indirect=false in_cycle=false
; ext return_char params=
; ext return_schar params=
; ext return_uchar params=
; call main -> return_char
; call main -> return_schar
; call main -> return_uchar
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$79
   STA   __local_main__char_array_0
   LDA   #$86
   STA   __local_main__char_array_1
   LDA   #$FD
   STA   __local_main__char_array_2
   JSR   return_char
   JSR   return_schar
   JSR   return_uchar
   LDA   __local_main__char_array_0
   CMP   #$79
   BNE   .main@jthread@1
   LDA   #$00
   CMP   #$FF
   BNE   .main@jthread@1
   LDA   __local_main__char_array_1
   CMP   #$86
   BNE   .main@jthread@1
   LDA   #$00
   CMP   #$FF
   BNE   .main@jthread@1
   LDA   __local_main__char_array_2
   CMP   #$FD
   BEQ   .jcmp_skip@0
.main@jthread@1:
   LDX   #$00
   LDA   #$01
   RTS
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
