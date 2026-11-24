SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__j_1	EQU	$84
__local_main__4	EQU	$85
__local_main__j_0	EQU	$86
__local_main__i_1	EQU	$87
__local_main__i_0	EQU	$88
__local_main__auto_array	EQU	$89

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__j_1,__local_main__4,__local_main__j_0,__local_main__i_1,__local_main__i_0,__local_main__auto_array,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item static_array section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$6C
   STA   __local_main__auto_array
   LDA   #$6D
   STA   __local_main__auto_array+1
   LDA   #$6E
   STA   __local_main__auto_array+2
   LDA   #$6F
   STA   __local_main__auto_array+3
   LDA   #$70
   STA   __local_main__auto_array+4
   LDA   #$00
   STA   __local_main__auto_array+5
   STA   __local_main__i_1
   LDY   #$00
   STY   __local_main__j_0
.loop@0_start:
   LDA   __local_main__i_1
   SEC
   SBC   #$03
   LDA   __local_main__j_0
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   #$00
   STA   __local_main__4
   LDY   #$00
   STY   __local_main__j_1
.loop@1_start:
   LDA   __local_main__4
   SEC
   SBC   #$04
   LDA   __local_main__j_1
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .loop@1_break
   LDA   __local_main__i_1
   ASL
   ASL
   STA   __local_main__3
   LDA   #<static_array
   CLC
   ADC   __local_main__3
   STA   __local_main__0
   LDA   #>static_array
   ADC   #$00
   STA   __local_main__1
   LDY   __local_main__4
   LDA   (__local_main__0),Y
   STA   __local_main__2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_main__1
   LDA   __local_main__3
   CLC
   ADC   __local_main__4
   TAX
   LDA   str@0,X
   STA   __local_main__0
   LDA   __local_main__1
   BNE   .jcmp_skip@4
   LDA   __local_main__2
   CMP   __local_main__0
   BEQ   .if_end@0
.jcmp_skip@4:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   INC   __local_main__4
   BNE   .loop@1_start
   INC   __local_main__j_1
   JMP   .loop@1_start
.loop@1_break:
   INC   __local_main__i_1
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_main__j_0
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@2_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$02
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@5
   EOR   #$80
.jcmp_novf@5:
   BPL   .loop@2_break
   LDA   #$00
   STA   __local_main__j_0
   LDY   #$00
   STY   __local_main__4
.loop@3_start:
   LDA   __local_main__j_0
   SEC
   SBC   #$03
   LDA   __local_main__4
   SBC   #$00
   BVC   .jcmp_novf@6
   EOR   #$80
.jcmp_novf@6:
   BPL   .loop@3_break
   LDA   __local_main__i_0
   ASL
   CLC
   ADC   __local_main__i_0
   STA   __local_main__2
   LDA   #<__local_main__auto_array
   CLC
   ADC   __local_main__2
   STA   __local_main__0
   LDA   #>__local_main__auto_array
   ADC   #$00
   STA   __local_main__1
   LDY   __local_main__j_0
   LDA   (__local_main__0),Y
   STA   __local_main__1
   TYA
   CLC
   ADC   __local_main__2
   TAX
   LDA   __local_main__1
   CMP   str@1,X
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   INC   __local_main__j_0
   BNE   .loop@3_start
   INC   __local_main__4
   JMP   .loop@3_start
.loop@3_break:
   INC   __local_main__i_0
   BNE   .loop@2_start
   INC   __local_main__i_1
   JMP   .loop@2_start
.loop@2_break:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $61, $62, $63, $64, $65, $66, $67, $68, $69, $6A, $6B, $00

str@1:
   DC.B  $6C, $6D, $6E, $6F, $70, $00

static_array:
   DC.B  $61, $62, $63, $64
   DC.B  $65, $66, $67, $68
   DC.B  $69, $6A, $6B, $00
