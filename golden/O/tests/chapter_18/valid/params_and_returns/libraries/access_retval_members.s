SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_calloc__nmemb_0	EQU	$80
__local_return_small_struct__i_3	EQU	$81
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_calloc__size_0	EQU	$84
__zpabi_calloc__size_1	EQU	$85
__zpabi_calloc__size_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87
__local_return_nested_struct__0	EQU	$88
__local_return_nested_struct__1	EQU	$89

; @zp-link-meta-begin
; def return_nested_struct params= locals=__local_return_nested_struct__0,__local_return_nested_struct__1 indirect=false in_cycle=false
; def return_small_struct params= locals=__local_return_small_struct__i_4,__local_return_small_struct__i_3,__local_return_small_struct__i_2,__local_return_small_struct__i_1,__local_return_small_struct__i_0 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; call return_nested_struct -> calloc
; item return_small_struct section=.text local=false zero=false
; item return_nested_struct section=.text local=false zero=false
; item @3.ret section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

return_small_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$00
   STY   __local_return_small_struct__i_3
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$66
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   __local_return_small_struct__i_3
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

return_nested_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   @3.ret+8
   STA   __local_return_nested_struct__1
   ORA   @3.ret+9
   BNE   .if_end@0
   LDA   #$01
   STA   __zpabi_calloc__nmemb_0
   LDA   #$05
   STA   __zpabi_calloc__size_0
   LDA   #$00
   STA   __zpabi_calloc__nmemb_1
   STA   __zpabi_calloc__nmemb_2
   STA   __zpabi_calloc__nmemb_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   JSR   calloc
   STA   @3.ret+8
   TXA
   STA   @3.ret+9
   STA   __local_return_nested_struct__1
   STA   DPTR+1
   LDA   @3.ret+8
   STA   __local_return_nested_struct__0
   STA   DPTR
   LDA   #$0C
   LDY   #$00
   STA   (__local_return_nested_struct__0),Y
   LDA   #$0D
   INY
   STA   (__local_return_nested_struct__0),Y
   LDA   #$00
   INY
   STA   (__local_return_nested_struct__0),Y
   INY
   STA   (__local_return_nested_struct__0),Y
   INY
   STA   (__local_return_nested_struct__0),Y
.if_end@0:
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   @3.ret
   LDY   #$00
   STA   (DPTR),Y
   LDA   @3.ret+1
   INY
   STA   (DPTR),Y
   LDA   @3.ret+2
   INY
   STA   (DPTR),Y
   LDA   @3.ret+3
   INY
   STA   (DPTR),Y
   LDA   @3.ret+4
   INY
   STA   (DPTR),Y
   LDA   @3.ret+5
   INY
   STA   (DPTR),Y
   LDA   @3.ret+6
   INY
   STA   (DPTR),Y
   LDA   @3.ret+7
   INY
   STA   (DPTR),Y
   LDA   @3.ret+8
   INY
   STA   (DPTR),Y
   LDA   @3.ret+9
   INY
   STA   (DPTR),Y
   LDA   @3.ret+10
   INY
   STA   (DPTR),Y
   LDA   @3.ret+11
   INY
   STA   (DPTR),Y
   LDA   @3.ret+12
   INY
   STA   (DPTR),Y
   LDA   @3.ret+13
   INY
   STA   (DPTR),Y
   LDA   @3.ret+14
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

@3.ret:
   DC.L  $00000000
   DC.L  $40000000
   DS.B  2
   DC.B  $0A
   DC.L  $0000000B
