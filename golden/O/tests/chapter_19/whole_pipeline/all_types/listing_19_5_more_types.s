SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_target__0	EQU	$81
__local_target__1	EQU	$82
__local_target__4	EQU	$85
__local_target__my_struct	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals=__local_target__0,__local_target__1,__local_target__2,__local_target__3,__local_target__4,__local_target__z,__local_target__my_struct,__local_target__7,__local_target__8,__local_target__9,__local_target__10,__local_target__11,__local_target__12,__local_target__13,__local_target__14,__local_target__15,__local_target__16,__local_target__17,__local_target__18,__local_target__19,__local_target__20,__local_target__21,__local_target__22,__local_target__23,__local_target__24,__local_target__25,__local_target__26,__local_target__27,__local_target__28,__local_target__29,__local_target__30,__local_target__31,__local_target__32,__local_target__33,__local_target__34,__local_target__35,__local_target__36,__local_target__37,__local_target__38,__local_target__39,__local_target__40,__local_target__41,__local_target__42,__local_target__43,__local_target__44,__local_target__45,__local_target__46,__local_target__47 indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item flag section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   #$0C
   STA   __local_target__4
.clear@0:
   DEC   __local_target__4
   LDA   __local_target__4
   CLC
   ADC   #<__local_target__my_struct
   STA   __local_target__0
   LDA   #$00
   ADC   #>__local_target__my_struct
   STA   __local_target__1
   LDA   #$00
   LDY   #$0A
   STA   (__local_target__0),Y
   LDA   __local_target__4
   BNE   .clear@0
   LDY   #$00
   LDA   #$09
   STA   HARGS+8
   TYA
   STA   HARGS+9
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   JSR   target
   LDX   HARGS+9
   LDA   HARGS+8
   RTS

flag:
   DC.L  $00000000
   DC.L  $41324F80
