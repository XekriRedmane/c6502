SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__6	EQU	$86
__local_main__7	EQU	$87
__local_main__8	EQU	$88
__local_main__9	EQU	$89

; @zp-link-meta-begin
; def get_ptr params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9 indirect=false in_cycle=false
; call main -> get_ptr
; item get_ptr section=.text local=false zero=false
; item main section=.text local=false zero=false
; item l section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

get_ptr:
   SUBROUTINE

   LDX   #>l
   LDA   #<l
   RTS

main:
   SUBROUTINE

   JSR   get_ptr
   STA   __local_main__8
   STX   __local_main__9
   LDY   #$00
   LDA   (__local_main__8),Y
   STA   __local_main__7
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__6
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__5
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__4
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__8),Y
   STA   __local_main__0
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   BNE   .jcmp_skip@0
   LDA   __local_main__2
   BNE   .jcmp_skip@0
   LDA   __local_main__3
   BNE   .jcmp_skip@0
   LDA   __local_main__4
   BNE   .jcmp_skip@0
   LDA   __local_main__5
   BNE   .jcmp_skip@0
   LDA   __local_main__6
   BNE   .jcmp_skip@0
   LDA   __local_main__7
   CMP   #$01
   BNE   .lb_skip@4
   JMP   .case@1
.lb_skip@4:
.jcmp_skip@0:
   LDA   __local_main__0
   BEQ   .lb_skip@3
   JMP   .default@5
.lb_skip@3:
   LDA   __local_main__1
   BEQ   .lb_skip@2
   JMP   .default@5
.lb_skip@2:
   LDA   __local_main__2
   BEQ   .lb_skip@1
   JMP   .default@5
.lb_skip@1:
   LDA   __local_main__3
   BEQ   .lb_skip@0
   JMP   .default@5
.lb_skip@0:
   LDA   __local_main__4
   BNE   .vthread@4
   LDA   __local_main__5
   BNE   .vthread@2
   LDA   __local_main__6
   BNE   .default@5
   LDA   __local_main__7
   CMP   #$04
   BEQ   .case@2
   LDA   __local_main__0
   BNE   .default@5
   LDA   __local_main__1
   BNE   .default@5
   LDA   __local_main__2
   BNE   .default@5
   LDA   __local_main__3
   BNE   .default@5
   LDA   __local_main__4
   BNE   .vthread@4
   LDA   __local_main__5
.vthread@2:
   CMP   #$01
   BNE   .default@5
   LDA   __local_main__6
   BNE   .default@5
   LDA   __local_main__7
   CMP   #$04
   BEQ   .case@3
   LDA   __local_main__0
   BNE   .default@5
   LDA   __local_main__1
   BNE   .default@5
   LDA   __local_main__2
   BNE   .default@5
   LDA   __local_main__3
   BNE   .default@5
   LDA   __local_main__4
.vthread@4:
   CMP   #$FF
   BNE   .default@5
   LDA   __local_main__5
   CMP   #$FF
   BNE   .default@5
   LDA   __local_main__6
   CMP   #$FF
   BNE   .default@5
   LDA   __local_main__7
   CMP   #$F0
   BEQ   .case@4
   BNE   .default@5
.case@1:
   LDX   #$00
   LDA   #$01
   RTS
.case@2:
   LDX   #$00
   LDA   #$02
   RTS
.case@3:
   LDX   #$00
   TXA
   RTS
.case@4:
   LDX   #$00
   LDA   #$03
   RTS
.default@5:
   LDX   #$00
   LDA   #$04
   RTS

l:
   DC.L  $00010004
   DC.L  $00000000
