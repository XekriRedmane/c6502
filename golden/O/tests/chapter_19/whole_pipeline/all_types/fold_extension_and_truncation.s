SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def target_char_int_conversion params= locals= indirect=false in_cycle=false
; def target_char_long_conversion params= locals= indirect=false in_cycle=false
; def target_char_uint_conversion params= locals= indirect=false in_cycle=false
; def target_char_ulong_conversion params= locals= indirect=false in_cycle=false
; def target_extend_int_to_long params= locals= indirect=false in_cycle=false
; def target_extend_int_to_ulong params= locals= indirect=false in_cycle=false
; def target_extend_uint_to_long params= locals= indirect=false in_cycle=false
; def target_extend_uint_to_ulong params= locals= indirect=false in_cycle=false
; def target_truncate_long_to_int params= locals= indirect=false in_cycle=false
; def target_truncate_long_to_uint params= locals= indirect=false in_cycle=false
; def target_truncate_ulong_to_int params= locals= indirect=false in_cycle=false
; def target_truncate_ulong_to_uint params= locals= indirect=false in_cycle=false
; def target_uchar_int_conversion params= locals= indirect=false in_cycle=false
; def target_uchar_long_conversion params= locals= indirect=false in_cycle=false
; def target_uchar_uint_conversion params= locals= indirect=false in_cycle=false
; def target_uchar_ulong_conversion params= locals= indirect=false in_cycle=false
; call main -> target_char_int_conversion
; call main -> target_char_long_conversion
; call main -> target_char_uint_conversion
; call main -> target_char_ulong_conversion
; call main -> target_extend_int_to_long
; call main -> target_extend_int_to_ulong
; call main -> target_extend_uint_to_long
; call main -> target_extend_uint_to_ulong
; call main -> target_truncate_long_to_int
; call main -> target_truncate_long_to_uint
; call main -> target_truncate_ulong_to_int
; call main -> target_truncate_ulong_to_uint
; call main -> target_uchar_int_conversion
; call main -> target_uchar_long_conversion
; call main -> target_uchar_uint_conversion
; call main -> target_uchar_ulong_conversion
; item target_extend_int_to_long section=.text local=false zero=false
; item target_extend_int_to_ulong section=.text local=false zero=false
; item target_extend_uint_to_long section=.text local=false zero=false
; item target_extend_uint_to_long@tail0 section=.text local=true zero=false
; item target_extend_uint_to_ulong section=.text local=false zero=false
; item target_truncate_long_to_int section=.text local=false zero=false
; item target_truncate_long_to_uint section=.text local=false zero=false
; item target_truncate_long_to_uint@tail2 section=.text local=true zero=false
; item target_truncate_ulong_to_int section=.text local=false zero=false
; item target_truncate_ulong_to_uint section=.text local=false zero=false
; item target_char_int_conversion section=.text local=false zero=false
; item target_uchar_int_conversion section=.text local=false zero=false
; item target_char_uint_conversion section=.text local=false zero=false
; item target_char_uint_conversion@tail1 section=.text local=true zero=false
; item target_uchar_uint_conversion section=.text local=false zero=false
; item target_char_long_conversion section=.text local=false zero=false
; item target_uchar_long_conversion section=.text local=false zero=false
; item target_char_ulong_conversion section=.text local=false zero=false
; item target_uchar_ulong_conversion section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

target_extend_int_to_long:
   SUBROUTINE

   LDA   #$5E
   STA   HARGS+8
   LDA   #$A9
   STA   HARGS+9
   LDA   #$AA
   STA   HARGS+10
   STA   HARGS+11
   RTS

target_extend_int_to_ulong:
   SUBROUTINE

   LDA   #$2E
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

target_extend_uint_to_long:
   SUBROUTINE

   JMP   target_extend_uint_to_long@tail0

target_extend_uint_to_long@tail0:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STA   HARGS+11
   RTS

target_extend_uint_to_ulong:
   SUBROUTINE

   JMP   target_extend_uint_to_long@tail0

target_truncate_long_to_int:
   SUBROUTINE

   LDX   #$FF
   LDA   #$EC
   RTS

target_truncate_long_to_uint:
   SUBROUTINE

   JMP   target_truncate_long_to_uint@tail2

target_truncate_long_to_uint@tail2:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_truncate_ulong_to_int:
   SUBROUTINE

   JMP   target_truncate_long_to_uint@tail2

target_truncate_ulong_to_uint:
   SUBROUTINE

   LDX   #$0C
   LDA   #$CC
   RTS

target_char_int_conversion:
   SUBROUTINE

   LDX   #$00
   LDA   #$02
   RTS

target_uchar_int_conversion:
   SUBROUTINE

   JMP   target_truncate_long_to_uint@tail2

target_char_uint_conversion:
   SUBROUTINE

   JMP   target_char_uint_conversion@tail1

target_char_uint_conversion@tail1:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

target_uchar_uint_conversion:
   SUBROUTINE

   JMP   target_truncate_long_to_uint@tail2

target_char_long_conversion:
   SUBROUTINE

   JMP   target_char_uint_conversion@tail1

target_uchar_long_conversion:
   SUBROUTINE

   JMP   target_truncate_long_to_uint@tail2

target_char_ulong_conversion:
   SUBROUTINE

   JMP   target_char_uint_conversion@tail1

target_uchar_ulong_conversion:
   SUBROUTINE

   JMP   target_truncate_long_to_uint@tail2

main:
   SUBROUTINE

   JSR   target_extend_int_to_long
   JMP   target_char_uint_conversion@tail1
