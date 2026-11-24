SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_double_array__0	EQU	$82
__local_int_array__0	EQU	$82
__local_double_array__1	EQU	$83
__local_int_array__1	EQU	$83
__local_double_array__2	EQU	$84
__local_int_array__2	EQU	$84
__local_double_array__3	EQU	$85
__local_int_array__3	EQU	$85
__local_double_array__4	EQU	$86
__local_int_array__4	EQU	$86
__local_double_array__5	EQU	$87
__local_int_array__5	EQU	$87
__local_double_array__6	EQU	$88
__local_int_array__6	EQU	$88
__local_double_array__7	EQU	$89
__local_int_array__7	EQU	$89
__local_double_array__8	EQU	$8A
__local_int_array__8	EQU	$8A
__local_double_array__9	EQU	$8B
__local_int_array__arr	EQU	$8B
__local_double_array__10	EQU	$8C
__local_double_array__11	EQU	$8D

; @zp-link-meta-begin
; def double_array params= locals=__local_double_array__0,__local_double_array__1,__local_double_array__2,__local_double_array__3,__local_double_array__4,__local_double_array__5,__local_double_array__6,__local_double_array__7,__local_double_array__8,__local_double_array__9,__local_double_array__10,__local_double_array__11 indirect=false in_cycle=false
; def int_array params= locals=__local_int_array__0,__local_int_array__1,__local_int_array__2,__local_int_array__3,__local_int_array__4,__local_int_array__5,__local_int_array__6,__local_int_array__7,__local_int_array__8,__local_int_array__arr,__local_int_array__10,__local_int_array__11,__local_int_array__12,__local_int_array__13,__local_int_array__14,__local_int_array__15,__local_int_array__16,__local_int_array__17,__local_int_array__18,__local_int_array__19,__local_int_array__20 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call main -> double_array
; call main -> int_array
; item int_array section=.text local=false zero=false
; item double_array section=.text local=false zero=false
; item main section=.text local=false zero=false
; item i section=.data local=false zero=false
; item @3.arr section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

int_array:
   SUBROUTINE

   LDA   #$01
   STA   __local_int_array__arr
   LDA   #$00
   STA   __local_int_array__arr+1
   LDY   #$02
   STY   __local_int_array__arr+2
   STA   __local_int_array__arr+3
   INY
   STY   __local_int_array__arr+4
   STA   __local_int_array__arr+5
   INY
   STY   __local_int_array__arr+6
   STA   __local_int_array__arr+7
   INY
   STY   __local_int_array__arr+8
   STA   __local_int_array__arr+9
   INY
   STY   __local_int_array__arr+10
   STA   __local_int_array__arr+11
   LDA   #>__local_int_array__arr
   STA   __local_int_array__8
   LDA   #<__local_int_array__arr
   CLC
   ADC   #$0A
   STA   __local_int_array__3
   BCC   .vthread@1
   INC   __local_int_array__8
   LDA   #$00
   BNE   .jcmp_skip@0
.vthread@1:
   CPY   #$06
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDA   #>__local_int_array__arr
   STA   __local_int_array__2
   LDA   #<__local_int_array__arr
   CLC
   ADC   #$0A
   STA   __local_int_array__0
   BCC   .carry_inc@1
   INC   __local_int_array__2
.carry_inc@1:
   LDA   __local_int_array__8
   CMP   __local_int_array__2
   BNE   .jcmp_skip@1
   LDA   __local_int_array__3
   CMP   __local_int_array__0
   BEQ   .if_end@2
.jcmp_skip@1:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   SEC
   SBC   #$06
   STA   __local_int_array__7
   BCS   .carry_inc@2
   DEC   __local_int_array__8
.carry_inc@2:
   LDY   #$00
   LDA   (__local_int_array__7),Y
   STA   __local_int_array__1
   INY
   LDA   (__local_int_array__7),Y
   BNE   .jcmp_skip@2
   LDA   __local_int_array__1
   CMP   #$03
   BEQ   .if_end@3
.jcmp_skip@2:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDY   #$00
   LDA   (__local_int_array__7),Y
   STA   __local_int_array__1
   INY
   LDA   (__local_int_array__7),Y
   BNE   .jcmp_skip@3
   LDA   __local_int_array__1
   CMP   #$03
   BEQ   .if_end@4
.jcmp_skip@3:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #>__local_int_array__arr
   STA   __local_int_array__2
   LDA   #<__local_int_array__arr
   CLC
   ADC   #$04
   STA   __local_int_array__0
   BCC   .carry_inc@3
   INC   __local_int_array__2
.carry_inc@3:
   LDA   __local_int_array__8
   CMP   __local_int_array__2
   BNE   .jcmp_skip@4
   LDA   __local_int_array__7
   CMP   __local_int_array__0
   BEQ   .if_end@5
.jcmp_skip@4:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   i
   SEC
   SBC   #$01
   STA   __local_int_array__0
   LDA   i+1
   SBC   #$00
   ASL   __local_int_array__0
   ROL
   STA   __local_int_array__1
   LDA   __local_int_array__7
   CLC
   ADC   __local_int_array__0
   STA   __local_int_array__5
   LDA   __local_int_array__8
   ADC   __local_int_array__1
   STA   __local_int_array__6
   LDA   #>__local_int_array__arr
   STA   __local_int_array__2
   LDA   #<__local_int_array__arr
   CLC
   ADC   #$0A
   STA   __local_int_array__0
   BCC   .carry_inc@4
   INC   __local_int_array__2
.carry_inc@4:
   LDA   __local_int_array__6
   CMP   __local_int_array__2
   BNE   .jcmp_skip@5
   LDA   __local_int_array__5
   CMP   __local_int_array__0
   BEQ   .if_end@6
.jcmp_skip@5:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDY   #$00
   LDA   (__local_int_array__5),Y
   STA   __local_int_array__1
   INY
   LDA   (__local_int_array__5),Y
   BNE   .jcmp_skip@6
   LDA   __local_int_array__1
   CMP   #$06
   BEQ   .if_end@7
.jcmp_skip@6:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   #$FF
   CLC
   ADC   i
   STA   __local_int_array__0
   LDA   #$FF
   ADC   i+1
   ASL   __local_int_array__0
   ROL
   STA   __local_int_array__1
   LDA   __local_int_array__5
   SEC
   SBC   __local_int_array__0
   STA   __local_int_array__3
   LDA   __local_int_array__6
   SBC   __local_int_array__1
   STA   __local_int_array__4
   LDA   #>__local_int_array__arr
   STA   __local_int_array__2
   LDA   #<__local_int_array__arr
   CLC
   ADC   #$04
   STA   __local_int_array__0
   BCC   .carry_inc@5
   INC   __local_int_array__2
.carry_inc@5:
   LDA   __local_int_array__4
   CMP   __local_int_array__2
   BNE   .jcmp_skip@7
   LDA   __local_int_array__3
   CMP   __local_int_array__0
   BEQ   .if_end@8
.jcmp_skip@7:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDY   #$00
   LDA   (__local_int_array__3),Y
   STA   __local_int_array__1
   INY
   LDA   (__local_int_array__3),Y
   BNE   .jcmp_skip@8
   LDA   __local_int_array__1
   CMP   #$03
   BEQ   .if_end@9
.jcmp_skip@8:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   LDA   __local_int_array__3
   CLC
   ADC   #$02
   STA   __local_int_array__3
   BCC   .carry_inc@6
   INC   __local_int_array__4
.carry_inc@6:
   LDA   #>__local_int_array__arr
   STA   __local_int_array__2
   LDA   #<__local_int_array__arr
   CLC
   ADC   #$06
   STA   __local_int_array__0
   BCC   .carry_inc@7
   INC   __local_int_array__2
.carry_inc@7:
   LDA   __local_int_array__4
   CMP   __local_int_array__2
   BNE   .jcmp_skip@9
   LDA   __local_int_array__3
   CMP   __local_int_array__0
   BEQ   .if_end@10
.jcmp_skip@9:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   LDY   #$00
   LDA   (__local_int_array__3),Y
   STA   __local_int_array__1
   INY
   LDA   (__local_int_array__3),Y
   BNE   .jcmp_skip@10
   LDA   __local_int_array__1
   CMP   #$04
   BEQ   .if_end@11
.jcmp_skip@10:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   LDX   #$00
   TXA
   RTS

double_array:
   SUBROUTINE

   LDA   #>(@3.arr+40)
   STA   __local_double_array__9
   LDA   @3.arr+47
   CMP   #$40
   BNE   .jcmp_skip@11
   LDA   @3.arr+46
   CMP   #$18
   BNE   .jcmp_skip@11
   LDA   @3.arr+45
   BNE   .jcmp_skip@11
   LDA   @3.arr+44
   BNE   .jcmp_skip@11
   LDA   @3.arr+43
   BNE   .jcmp_skip@11
   LDA   @3.arr+42
   BNE   .jcmp_skip@11
   LDA   @3.arr+41
   BNE   .jcmp_skip@11
   LDA   @3.arr+40
   BEQ   .if_end@13
.jcmp_skip@11:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@13:
   LDA   #>(@3.arr+40)
   CMP   #>(@3.arr+40)
   BNE   .jcmp_skip@13
   LDA   #<(@3.arr+40)
   CMP   #<(@3.arr+40)
   BEQ   .if_end@14
.jcmp_skip@13:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@14:
   SEC
   SBC   #$18
   STA   __local_double_array__8
   BCS   .carry_inc@8
   DEC   __local_double_array__9
.carry_inc@8:
   LDY   #$00
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__7
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__6
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__5
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__4
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__3
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__2
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__1
   INY
   LDA   (__local_double_array__8),Y
   CMP   #$40
   BNE   .jcmp_skip@14
   LDA   __local_double_array__1
   CMP   #$08
   BNE   .jcmp_skip@14
   LDA   __local_double_array__2
   BNE   .jcmp_skip@14
   LDA   __local_double_array__3
   BNE   .jcmp_skip@14
   LDA   __local_double_array__4
   BNE   .jcmp_skip@14
   LDA   __local_double_array__5
   BNE   .jcmp_skip@14
   LDA   __local_double_array__6
   BNE   .jcmp_skip@14
   LDA   __local_double_array__7
   BEQ   .if_end@15
.jcmp_skip@14:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@15:
   LDY   #$00
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__7
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__6
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__5
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__4
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__3
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__2
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__1
   INY
   LDA   (__local_double_array__8),Y
   CMP   #$40
   BNE   .jcmp_skip@15
   LDA   __local_double_array__1
   CMP   #$08
   BNE   .jcmp_skip@15
   LDA   __local_double_array__2
   BNE   .jcmp_skip@15
   LDA   __local_double_array__3
   BNE   .jcmp_skip@15
   LDA   __local_double_array__4
   BNE   .jcmp_skip@15
   LDA   __local_double_array__5
   BNE   .jcmp_skip@15
   LDA   __local_double_array__6
   BNE   .jcmp_skip@15
   LDA   __local_double_array__7
   BEQ   .if_end@16
.jcmp_skip@15:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@16:
   LDA   __local_double_array__9
   CMP   #>(@3.arr+16)
   BNE   .jcmp_skip@16
   LDA   __local_double_array__8
   CMP   #<(@3.arr+16)
   BEQ   .if_end@17
.jcmp_skip@16:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@17:
   LDA   i
   SEC
   SBC   #$01
   STA   __local_double_array__0
   LDA   i+1
   SBC   #$00
   ASL   __local_double_array__0
   ROL
   ASL   __local_double_array__0
   ROL
   ASL   __local_double_array__0
   ROL
   STA   __local_double_array__1
   LDA   __local_double_array__8
   CLC
   ADC   __local_double_array__0
   STA   __local_double_array__10
   LDA   __local_double_array__9
   ADC   __local_double_array__1
   STA   __local_double_array__11
   CMP   #>(@3.arr+40)
   BNE   .jcmp_skip@17
   LDA   __local_double_array__10
   CMP   #<(@3.arr+40)
   BEQ   .if_end@18
.jcmp_skip@17:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@18:
   LDY   #$00
   LDA   (__local_double_array__10),Y
   STA   __local_double_array__7
   INY
   LDA   (__local_double_array__10),Y
   STA   __local_double_array__6
   INY
   LDA   (__local_double_array__10),Y
   STA   __local_double_array__5
   INY
   LDA   (__local_double_array__10),Y
   STA   __local_double_array__4
   INY
   LDA   (__local_double_array__10),Y
   STA   __local_double_array__3
   INY
   LDA   (__local_double_array__10),Y
   STA   __local_double_array__2
   INY
   LDA   (__local_double_array__10),Y
   STA   __local_double_array__1
   INY
   LDA   (__local_double_array__10),Y
   CMP   #$40
   BNE   .jcmp_skip@18
   LDA   __local_double_array__1
   CMP   #$18
   BNE   .jcmp_skip@18
   LDA   __local_double_array__2
   BNE   .jcmp_skip@18
   LDA   __local_double_array__3
   BNE   .jcmp_skip@18
   LDA   __local_double_array__4
   BNE   .jcmp_skip@18
   LDA   __local_double_array__5
   BNE   .jcmp_skip@18
   LDA   __local_double_array__6
   BNE   .jcmp_skip@18
   LDA   __local_double_array__7
   BEQ   .if_end@19
.jcmp_skip@18:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@19:
   LDA   #$FF
   CLC
   ADC   i
   STA   __local_double_array__0
   LDA   #$FF
   ADC   i+1
   ASL   __local_double_array__0
   ROL
   ASL   __local_double_array__0
   ROL
   ASL   __local_double_array__0
   ROL
   STA   __local_double_array__1
   LDA   __local_double_array__10
   SEC
   SBC   __local_double_array__0
   STA   __local_double_array__8
   LDA   __local_double_array__11
   SBC   __local_double_array__1
   STA   __local_double_array__9
   CMP   #>(@3.arr+16)
   BNE   .jcmp_skip@19
   LDA   __local_double_array__8
   CMP   #<(@3.arr+16)
   BEQ   .if_end@20
.jcmp_skip@19:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@20:
   LDY   #$00
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__7
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__6
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__5
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__4
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__3
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__2
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__1
   INY
   LDA   (__local_double_array__8),Y
   CMP   #$40
   BNE   .jcmp_skip@20
   LDA   __local_double_array__1
   CMP   #$08
   BNE   .jcmp_skip@20
   LDA   __local_double_array__2
   BNE   .jcmp_skip@20
   LDA   __local_double_array__3
   BNE   .jcmp_skip@20
   LDA   __local_double_array__4
   BNE   .jcmp_skip@20
   LDA   __local_double_array__5
   BNE   .jcmp_skip@20
   LDA   __local_double_array__6
   BNE   .jcmp_skip@20
   LDA   __local_double_array__7
   BEQ   .if_end@21
.jcmp_skip@20:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@21:
   LDA   __local_double_array__8
   CLC
   ADC   #$08
   STA   __local_double_array__8
   BCC   .carry_inc@9
   INC   __local_double_array__9
.carry_inc@9:
   LDA   __local_double_array__9
   CMP   #>(@3.arr+24)
   BNE   .jcmp_skip@21
   LDA   __local_double_array__8
   CMP   #<(@3.arr+24)
   BEQ   .if_end@22
.jcmp_skip@21:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@22:
   LDY   #$00
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__7
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__6
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__5
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__4
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__3
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__2
   INY
   LDA   (__local_double_array__8),Y
   STA   __local_double_array__1
   INY
   LDA   (__local_double_array__8),Y
   CMP   #$40
   BNE   .jcmp_skip@22
   LDA   __local_double_array__1
   CMP   #$10
   BNE   .jcmp_skip@22
   LDA   __local_double_array__2
   BNE   .jcmp_skip@22
   LDA   __local_double_array__3
   BNE   .jcmp_skip@22
   LDA   __local_double_array__4
   BNE   .jcmp_skip@22
   LDA   __local_double_array__5
   BNE   .jcmp_skip@22
   LDA   __local_double_array__6
   BNE   .jcmp_skip@22
   LDA   __local_double_array__7
   BEQ   .if_end@23
.jcmp_skip@22:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@23:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   JSR   int_array
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@24
   LDA   __local_main__1
   RTS
.if_end@24:
   JSR   double_array
   STA   __local_main__0
   STX   __local_main__1
   ORA   __local_main__1
   BEQ   .if_end@25
   LDA   __local_main__0
   CLC
   ADC   #$0C
   BCC   .carry_inc@10
   INC   __local_main__1
.carry_inc@10:
   LDX   __local_main__1
   RTS
.if_end@25:
   LDX   #$00
   TXA
   RTS

i:
   DC.W  $0004

@3.arr:
   DC.L  $00000000
   DC.L  $3FF00000
   DC.L  $00000000
   DC.L  $40000000
   DC.L  $00000000
   DC.L  $40080000
   DC.L  $00000000
   DC.L  $40100000
   DC.L  $00000000
   DC.L  $40140000
   DC.L  $00000000
   DC.L  $40180000
