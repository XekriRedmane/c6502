SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_strcmp__s2_1	EQU	$81
__local_main__0	EQU	$82
__local_main__1	EQU	$83
__local_main__2	EQU	$84
__local_main__i	EQU	$86
__local_main__strings	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__i,__local_main__strings,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29 indirect=false in_cycle=false
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call main -> strcmp
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$61
   STA   __local_main__strings
   LDA   #$62
   STA   __local_main__strings+1
   LDA   #$63
   STA   __local_main__strings+2
   LDA   #$64
   STA   __local_main__strings+3
   LDA   #$65
   STA   __local_main__strings+4
   LDA   #$66
   STA   __local_main__strings+5
   LDA   #$67
   STA   __local_main__strings+6
   LDA   #$68
   STA   __local_main__strings+7
   LDA   #$69
   STA   __local_main__strings+8
   LDA   #$6A
   STA   __local_main__strings+9
   LDA   #$6B
   STA   __local_main__strings+10
   LDA   #$6C
   STA   __local_main__strings+11
   LDA   #$00
   STA   __local_main__strings+12
   LDA   #$7A
   STA   __local_main__strings+13
   LDA   #$0C
   STA   __local_main__i
.clear@0:
   DEC   __local_main__i
   LDA   __local_main__i
   CLC
   ADC   #<__local_main__strings
   STA   __local_main__0
   LDA   #$00
   ADC   #>__local_main__strings
   STA   __local_main__1
   LDA   #$00
   LDY   #$0E
   STA   (__local_main__0),Y
   LDA   __local_main__i
   BNE   .clear@0
   LDA   #>__local_main__strings
   STA   __local_main__2
   CLC
   BCC   .carry_inc@0
   INC   __local_main__2
.carry_inc@0:
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDX   __local_main__2
   LDA   #<__local_main__strings
   JSR   strcmp
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDA   __local_main__strings+13
   CMP   #$7A
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   LDA   #$01
   STA   __local_main__i
   LDX   #$00
.loop@0_start:
   LDA   __local_main__i
   SEC
   SBC   #$0D
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_main__i
   CLC
   ADC   #<__local_main__strings
   STA   __local_main__0
   LDA   #$00
   ADC   #>__local_main__strings
   STA   __local_main__1
   LDY   #$0D
   LDA   (__local_main__0),Y
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$03
   RTS
.if_end@3:
   INC   __local_main__i
   BNE   .loop@0_start
   INX
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $61, $62, $63, $64, $65, $66, $67, $68, $69, $6A, $6B, $6C, $00
