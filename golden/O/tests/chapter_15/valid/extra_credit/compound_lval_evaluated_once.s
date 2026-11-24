SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__5	EQU	$85
__local_main__arr	EQU	$86

; @zp-link-meta-begin
; def get_call_count params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__arr,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14 indirect=false in_cycle=false
; call main -> get_call_count
; item get_call_count section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @0.count section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

get_call_count:
   SUBROUTINE

   INC   @0.count
   BNE   .inc_done@0
   INC   @0.count+1
.inc_done@0:
   LDX   @0.count+1
   LDA   @0.count
   RTS

main:
   SUBROUTINE

   LDA   #$0A
   STA   __local_main__arr
   LDA   #$00
   STA   __local_main__arr+1
   LDY   #$0B
   STY   __local_main__arr+2
   STA   __local_main__arr+3
   INY
   STY   __local_main__arr+4
   STA   __local_main__arr+5
   INY
   STY   __local_main__arr+6
   STA   __local_main__arr+7
   LDA   #>__local_main__arr
   STA   __local_main__3
   JSR   get_call_count
   ASL
   STA   __local_main__0
   LDA   #<__local_main__arr
   CLC
   ADC   __local_main__0
   STA   __local_main__2
   BCC   .carry_inc@0
   INC   __local_main__3
.carry_inc@0:
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$0B
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #>__local_main__arr
   STA   __local_main__5
   LDA   #<__local_main__arr
   CLC
   ADC   #$08
   BCC   .carry_inc@1
   INC   __local_main__5
.carry_inc@1:
   SEC
   SBC   #$02
   STA   __local_main__3
   BCS   .carry_inc@2
   DEC   __local_main__5
.carry_inc@2:
   JSR   get_call_count
   STA   __local_main__0
   STX   __local_main__2
   SEC
   LDA   #$00
   SBC   __local_main__0
   STA   __local_main__0
   LDA   #$00
   SBC   __local_main__2
   ASL   __local_main__0
   ROL
   STA   __local_main__1
   LDA   __local_main__3
   CLC
   ADC   __local_main__0
   STA   __local_main__2
   LDA   __local_main__5
   ADC   __local_main__1
   STA   __local_main__3
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$0B
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   get_call_count
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
   LDX   #$00
   TXA
   RTS

@0.count:
   DS.B  2
