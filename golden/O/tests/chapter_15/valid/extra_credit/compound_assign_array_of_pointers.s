SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__array1	EQU	$84
__local_main__nested_array	EQU	$8C

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__array1,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__nested_array,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item @0.array_of_pointers section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$0A
   STA   __local_main__array1
   LDA   #$00
   STA   __local_main__array1+1
   LDY   #$0B
   STY   __local_main__array1+2
   STA   __local_main__array1+3
   INY
   STY   __local_main__array1+4
   STA   __local_main__array1+5
   INY
   STY   __local_main__array1+6
   STA   __local_main__array1+7
   LDY   #$14
   STY   __local_main__nested_array
   STA   __local_main__nested_array+1
   INY
   STY   __local_main__nested_array+2
   STA   __local_main__nested_array+3
   INY
   STY   __local_main__nested_array+4
   STA   __local_main__nested_array+5
   INY
   STY   __local_main__nested_array+6
   STA   __local_main__nested_array+7
   LDY   #$1E
   STY   __local_main__nested_array+8
   STA   __local_main__nested_array+9
   INY
   STY   __local_main__nested_array+10
   STA   __local_main__nested_array+11
   INY
   STY   __local_main__nested_array+12
   STA   __local_main__nested_array+13
   INY
   STY   __local_main__nested_array+14
   STA   __local_main__nested_array+15
   LDA   #<__local_main__array1
   STA   @0.array_of_pointers
   LDA   #>__local_main__array1
   STA   @0.array_of_pointers+1
   LDA   #<__local_main__nested_array
   CLC
   STA   @0.array_of_pointers+2
   LDA   #>__local_main__nested_array
   STA   @0.array_of_pointers+3
   LDA   #<(__local_main__nested_array+8)
   STA   @0.array_of_pointers+4
   LDA   #>(__local_main__nested_array+8)
   STA   @0.array_of_pointers+5
   LDA   @0.array_of_pointers
   ADC   #$08
   STA   @0.array_of_pointers
   BCC   .carry_inc@0
   INC   @0.array_of_pointers+1
.carry_inc@0:
   STA   __local_main__1
   LDA   @0.array_of_pointers+1
   STA   __local_main__0
   LDA   #$FE
   CLC
   ADC   __local_main__1
   STA   __local_main__2
   LDA   #$FF
   ADC   __local_main__0
   STA   __local_main__3
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$0D
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   @0.array_of_pointers+2
   CLC
   ADC   #$08
   STA   @0.array_of_pointers+2
   BCC   .carry_inc@1
   INC   @0.array_of_pointers+3
.carry_inc@1:
   LDA   @0.array_of_pointers+4
   SEC
   SBC   #$08
   STA   @0.array_of_pointers+4
   BCS   .carry_inc@2
   DEC   @0.array_of_pointers+5
.carry_inc@2:
   LDA   @0.array_of_pointers+2
   STA   __local_main__2
   LDA   @0.array_of_pointers+3
   STA   __local_main__3
   LDY   #$06
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$21
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   @0.array_of_pointers+4
   STA   __local_main__2
   LDA   @0.array_of_pointers+5
   STA   __local_main__3
   LDY   #$06
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$17
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

@0.array_of_pointers:
   DS.B  6
