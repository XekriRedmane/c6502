SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_target_chars__0	EQU	$82
__local_target_long_bitshift__2	EQU	$84
__local_target_long_bitshift__3	EQU	$85

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target_chars params= locals=__local_target_chars__0 indirect=false in_cycle=false
; def target_long_bitshift params= locals=__local_target_long_bitshift__0,__local_target_long_bitshift__1,__local_target_long_bitshift__2,__local_target_long_bitshift__3,__local_target_long_bitshift__4,__local_target_long_bitshift__5,__local_target_long_bitshift__6,__local_target_long_bitshift__7 indirect=false in_cycle=false
; def target_long_bitwise params= locals= indirect=false in_cycle=false
; def target_unsigned_bitshift params= locals=__local_target_unsigned_bitshift__0,__local_target_unsigned_bitshift__1,__local_target_unsigned_bitshift__2,__local_target_unsigned_bitshift__3 indirect=false in_cycle=false
; def target_unsigned_bitwise params= locals= indirect=false in_cycle=false
; call main -> target_chars
; call main -> target_long_bitshift
; call main -> target_long_bitwise
; call main -> target_unsigned_bitshift
; call main -> target_unsigned_bitwise
; item target_chars section=.text local=false zero=false
; item target_long_bitwise section=.text local=false zero=false
; item target_long_bitwise@tail0 section=.text local=true zero=false
; item target_long_bitshift section=.text local=false zero=false
; item target_long_bitshift@tail1 section=.text local=true zero=false
; item target_unsigned_bitwise section=.text local=false zero=false
; item target_unsigned_bitshift section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

target_chars:
   SUBROUTINE

   LDA   #$FA
   STA   __local_target_chars__0
   LDX   #$1F
.shift_loop@0:
   LSR   __local_target_chars__0
   DEX
   BNE   .shift_loop@0
   LDA   __local_target_chars__0
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS

target_long_bitwise:
   SUBROUTINE

   JMP   target_long_bitwise@tail0

target_long_bitwise@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

target_long_bitshift:
   SUBROUTINE

   LDA   #$64
   STA   __local_target_long_bitshift__2
   LDA   #$00
   STA   __local_target_long_bitshift__3
   LDX   #$16
.shift_loop@2:
   ASL   __local_target_long_bitshift__2
   ROL   __local_target_long_bitshift__3
   DEX
   BNE   .shift_loop@2
   JMP   target_long_bitwise@tail0
.shift_loop@7:
   LDA   __local_target_long_bitshift__3
   ASL
   ROR   __local_target_long_bitshift__3
   ROR   __local_target_long_bitshift__2
   DEX
   BNE   .shift_loop@7
   JMP   target_long_bitshift@tail1

target_long_bitshift@tail1:
   SUBROUTINE

   LDX   #$00
   LDA   #$02
   RTS

target_unsigned_bitwise:
   SUBROUTINE

   JMP   target_long_bitwise@tail0

target_unsigned_bitshift:
   SUBROUTINE

   LDA   #$FF
   STA   HARGS
   LDY   #$FF
   STY   HARGS+1
   STY   HARGS+2
   STA   HARGS+3
   LDA   #$2C
   STA   HARGS+4
   JSR   asl32
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@18
   CMP   #$F0
   BNE   .jcmp_skip@18
   CMP   #$00
   BNE   .jcmp_skip@18
   LDA   HARGS+8
   BNE   .jcmp_skip@18
   LDA   HARGS+7
   BNE   .jcmp_skip@18
   LDA   HARGS+6
   BNE   .jcmp_skip@18
   LDA   HARGS+5
   BEQ   .if_end@27
.jcmp_skip@18:
   JMP   target_long_bitshift@tail1
.if_end@27:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   JSR   target_chars
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@28
   JMP   target_long_bitwise@tail0
.if_end@28:
   JSR   target_long_bitwise
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@29
   JMP   target_long_bitshift@tail1
.if_end@29:
   JSR   target_long_bitshift
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@30
   LDX   #$00
   LDA   #$03
   RTS
.if_end@30:
   JSR   target_unsigned_bitwise
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@31
   LDX   #$00
   LDA   #$04
   RTS
.if_end@31:
   JSR   target_unsigned_bitshift
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@32
   LDX   #$00
   LDA   #$05
   RTS
.if_end@32:
   LDX   #$00
   TXA
   RTS
