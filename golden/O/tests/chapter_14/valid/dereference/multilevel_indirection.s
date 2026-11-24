SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__4	EQU	$84
__local_main__5	EQU	$85
__local_main__6	EQU	$86
__local_main__7	EQU	$87
__local_main__8	EQU	$88
__local_main__9	EQU	$89
__local_main__d	EQU	$8E
__local_main__d_ptr	EQU	$96
__local_main__d2	EQU	$9A
__local_main__d2_ptr	EQU	$A2
__local_main__d2_ptr2	EQU	$A4

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__d,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__d_ptr,__local_main__23,__local_main__d_ptr_ptr,__local_main__25,__local_main__d2,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__d2_ptr,__local_main__35,__local_main__d2_ptr2,__local_main__37 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$40
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   #$24
   CMP   #$24
   BNE   .jcmp_skip@0
   LDA   #$00
   BNE   .jcmp_skip@0
   BEQ   .vthread@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.vthread@0:
   LDY   #$00
   LDA   #$40
   CMP   #$40
   BNE   .jcmp_skip@1
   LDA   #$24
   CMP   #$24
   BNE   .jcmp_skip@1
   TYA
   BNE   .jcmp_skip@1
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   STA   __local_main__7
   STA   __local_main__2
   LDA   #$40
   CMP   #$40
   BNE   .jcmp_skip@2
   LDA   #$24
   CMP   #$24
   BNE   .jcmp_skip@2
   LDA   __local_main__2
   BNE   .jcmp_skip@2
   TYA
   BNE   .jcmp_skip@2
   LDA   __local_main__7
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   TYA
   STA   __local_main__7
   STA   __local_main__2
   LDA   #$40
   CMP   #$40
   BNE   .jcmp_skip@3
   LDA   #$24
   CMP   #$24
   BNE   .jcmp_skip@3
   LDA   __local_main__2
   BNE   .jcmp_skip@3
   TYA
   BNE   .jcmp_skip@3
   LDA   __local_main__7
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #>__local_main__d
   CMP   #>__local_main__d
   BNE   .jcmp_skip@4
   LDA   #<__local_main__d
   CMP   #<__local_main__d
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #>__local_main__d
   CMP   #>__local_main__d
   BNE   .jcmp_skip@5
   LDA   #<__local_main__d
   CMP   #<__local_main__d
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   #>__local_main__d
   CMP   #>__local_main__d
   BNE   .jcmp_skip@6
   LDA   #<__local_main__d
   CMP   #<__local_main__d
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   TYA
   STA   __local_main__d
   STA   __local_main__d+1
   STA   __local_main__d+2
   STA   __local_main__d+3
   STA   __local_main__d+4
   STA   __local_main__d+5
   LDA   #$14
   STA   __local_main__d+6
   LDA   #$40
   STA   __local_main__d+7
   CMP   #$40
   BNE   .jcmp_skip@7
   LDA   #$14
   CMP   #$14
   BNE   .jcmp_skip@7
   LDA   __local_main__d+5
   BNE   .jcmp_skip@7
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   STA   __local_main__2
   LDA   #$40
   CMP   #$40
   BNE   .jcmp_skip@8
   LDA   #$14
   CMP   #$14
   BNE   .jcmp_skip@8
   LDA   __local_main__2
   BNE   .jcmp_skip@8
   BEQ   .if_end@9
.jcmp_skip@8:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@9:
   LDA   #<__local_main__d
   STA   __local_main__8
   LDA   __local_main__d_ptr+1
   STA   __local_main__9
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
   CMP   #$40
   BNE   .jcmp_skip@10
   LDA   __local_main__1
   CMP   #$14
   BNE   .jcmp_skip@10
   LDA   __local_main__2
   BNE   .jcmp_skip@10
   LDA   __local_main__3
   BNE   .jcmp_skip@10
   LDA   __local_main__4
   BNE   .jcmp_skip@10
   LDA   __local_main__5
   BNE   .jcmp_skip@10
   LDA   __local_main__6
   BNE   .jcmp_skip@10
   LDA   __local_main__7
   BEQ   .if_end@10
.jcmp_skip@10:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   LDA   #$00
   STA   __local_main__d2
   STA   __local_main__d2+1
   STA   __local_main__d2+2
   STA   __local_main__d2+3
   STA   __local_main__d2+4
   STA   __local_main__d2+5
   LDA   #$F0
   STA   __local_main__d2+6
   LDA   #$3F
   STA   __local_main__d2+7
   LDA   #<__local_main__d2
   STA   __local_main__d2_ptr
   STA   __local_main__8
   LDA   __local_main__d2_ptr+1
   STA   __local_main__9
   CMP   __local_main__d2_ptr+1
   BNE   .jcmp_skip@11
   LDA   #<__local_main__d2
   CMP   #<__local_main__d2
   BEQ   .if_end@11
.jcmp_skip@11:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
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
   CMP   #$3F
   BNE   .jcmp_skip@12
   LDA   __local_main__1
   CMP   #$F0
   BNE   .jcmp_skip@12
   LDA   __local_main__2
   BNE   .jcmp_skip@12
   LDA   __local_main__3
   BNE   .jcmp_skip@12
   LDA   __local_main__4
   BNE   .jcmp_skip@12
   LDA   __local_main__5
   BNE   .jcmp_skip@12
   LDA   __local_main__6
   BNE   .jcmp_skip@12
   LDA   __local_main__7
   BEQ   .if_end@12
.jcmp_skip@12:
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@12:
   LDA   #>__local_main__d2_ptr
   CMP   #>__local_main__d2_ptr2
   BNE   .if_end@13
   LDA   #<__local_main__d2_ptr
   CMP   #<__local_main__d2_ptr2
   BNE   .if_end@13
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@13:
   LDA   #<__local_main__d
   STA   __local_main__d2_ptr
   STA   __local_main__8
   LDA   __local_main__d_ptr+1
   STA   __local_main__d2_ptr+1
   STA   __local_main__9
   CMP   __local_main__d_ptr+1
   BNE   .jcmp_skip@13
   LDA   #<__local_main__d
   CMP   #<__local_main__d
   BEQ   .if_end@14
.jcmp_skip@13:
   LDX   #$00
   LDA   #$0F
   RTS
.if_end@14:
   LDA   __local_main__d2_ptr+1
   CMP   __local_main__d_ptr+1
   BNE   .jcmp_skip@14
   LDA   #<__local_main__d
   CMP   #<__local_main__d
   BEQ   .if_end@15
.jcmp_skip@14:
   LDX   #$00
   LDA   #$10
   RTS
.if_end@15:
   LDA   __local_main__9
   CMP   #>__local_main__d2
   BNE   .if_end@16
   LDA   #<__local_main__d
   CMP   #<__local_main__d2
   BNE   .if_end@16
   LDX   #$00
   LDA   #$11
   RTS
.if_end@16:
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
   CMP   #$40
   BNE   .jcmp_skip@15
   LDA   __local_main__1
   CMP   #$14
   BNE   .jcmp_skip@15
   LDA   __local_main__2
   BNE   .jcmp_skip@15
   LDA   __local_main__3
   BNE   .jcmp_skip@15
   LDA   __local_main__4
   BNE   .jcmp_skip@15
   LDA   __local_main__5
   BNE   .jcmp_skip@15
   LDA   __local_main__6
   BNE   .jcmp_skip@15
   LDA   __local_main__7
   BEQ   .if_end@17
.jcmp_skip@15:
   LDX   #$00
   LDA   #$12
   RTS
.if_end@17:
   LDX   #$00
   TXA
   RTS
