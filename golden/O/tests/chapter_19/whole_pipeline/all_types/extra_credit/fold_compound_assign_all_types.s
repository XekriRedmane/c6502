SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target_assign_long_to_int params= locals= indirect=false in_cycle=false
; def target_assign_to_long params= locals= indirect=false in_cycle=false
; def target_chars params= locals= indirect=false in_cycle=false
; def target_double params= locals= indirect=false in_cycle=false
; def target_double_cast params= locals= indirect=false in_cycle=false
; def target_uint params= locals= indirect=false in_cycle=false
; call main -> target_assign_long_to_int
; call main -> target_assign_to_long
; call main -> target_chars
; call main -> target_double
; call main -> target_double_cast
; call main -> target_uint
; item target_chars section=.text local=false zero=false
; item target_chars@tail0 section=.text local=true zero=false
; item target_double section=.text local=false zero=false
; item target_double_cast section=.text local=false zero=false
; item target_uint section=.text local=false zero=false
; item target_assign_long_to_int section=.text local=false zero=false
; item target_assign_to_long section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target_chars:
   SUBROUTINE

   JMP   target_chars@tail0

target_chars@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

target_double:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_double_cast:
   SUBROUTINE

   LDX   #$00
   LDA   #$02
   RTS

target_uint:
   SUBROUTINE

   JMP   target_chars@tail0

target_assign_long_to_int:
   SUBROUTINE

   JMP   target_chars@tail0

target_assign_to_long:
   SUBROUTINE

   JMP   target_chars@tail0

main:
   SUBROUTINE

   JSR   target_chars
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@18
   JMP   target_chars@tail0
.if_end@18:
   JSR   target_double
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@19
   LDX   #$00
   LDA   #$02
   RTS
.if_end@19:
   JSR   target_double_cast
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@20
   LDX   #$00
   LDA   #$03
   RTS
.if_end@20:
   JSR   target_uint
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@21
   LDX   #$00
   LDA   #$04
   RTS
.if_end@21:
   JSR   target_assign_long_to_int
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@22
   LDX   #$00
   LDA   #$05
   RTS
.if_end@22:
   JSR   target_assign_to_long
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@23
   LDX   #$00
   LDA   #$06
   RTS
.if_end@23:
   LDX   #$00
   TXA
   RTS
