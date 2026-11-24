SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81
__local_main__x	EQU	$82
__local_test_arr__0	EQU	$84
__local_test_array_of_pointers__0	EQU	$84
__local_test_arr__1	EQU	$85
__local_test_array_of_pointers__1	EQU	$85
__local_test_array_of_pointers__i_0	EQU	$86
__local_test_array_of_pointers__ptr	EQU	$87
__local_test_array_of_pointers__i_1	EQU	$88
__local_test_array_of_pointers__2	EQU	$89
__local_test_arr__i_1	EQU	$8A
__local_test_arr__i_0	EQU	$8B

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__x,__local_main__3 indirect=false in_cycle=false
; def test_arr params= locals=__local_test_arr__0,__local_test_arr__1,__local_test_arr__2,__local_test_arr__3,__local_test_arr__4,__local_test_arr__5,__local_test_arr__i_1,__local_test_arr__i_0 indirect=false in_cycle=false
; def test_array_of_pointers params=__zpabi_test_array_of_pointers__ptr_0,__zpabi_test_array_of_pointers__ptr_1 locals=__local_test_array_of_pointers__0,__local_test_array_of_pointers__1,__local_test_array_of_pointers__i_0,__local_test_array_of_pointers__ptr,__local_test_array_of_pointers__i_1,__local_test_array_of_pointers__2 indirect=false in_cycle=false param_regs=A,X
; def test_ptr_to_arr params= locals=__local_test_ptr_to_arr__0,__local_test_ptr_to_arr__1 indirect=false in_cycle=false
; call main -> test_arr
; call main -> test_array_of_pointers
; call main -> test_ptr_to_arr
; item test_arr section=.text local=false zero=false
; item test_ptr_to_arr section=.text local=false zero=false
; item test_array_of_pointers section=.text local=false zero=false
; item main section=.text local=false zero=false
; item arr section=.data local=false zero=false
; item ptr_to_arr section=.bss local=false zero=true
; item array_of_pointers section=.bss local=false zero=true
; item @1.nested_arr section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_arr:
   SUBROUTINE

   LDA   #$00
   STA   __local_test_arr__i_0
   LDY   #$00
   STY   __local_test_arr__i_1
.loop@0_start:
   LDA   __local_test_arr__i_0
   SEC
   SBC   #$04
   LDA   __local_test_arr__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_test_arr__i_0
   ASL
   ASL
   TAX
   INC   __local_test_arr__i_0
   BNE   .inc_done@0
   INC   __local_test_arr__i_1
.inc_done@0:
   LDA   __local_test_arr__i_1
   BMI   .sx_neg@1
   TYA
   JMP   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_test_arr__1
   STA   __local_test_arr__0
   LDA   arr+3,X
   CMP   __local_test_arr__0
   BNE   .jcmp_skip@3
   LDA   arr+2,X
   CMP   __local_test_arr__1
   BNE   .jcmp_skip@3
   LDA   arr+1,X
   CMP   __local_test_arr__i_1
   BNE   .jcmp_skip@3
   LDA   arr,X
   CMP   __local_test_arr__i_0
   BEQ   .loop@0_start
.jcmp_skip@3:
   LDX   #$00
   LDA   #$01
   RTS
.loop@0_break:
   LDX   #$00
   TXA
   RTS

test_ptr_to_arr:
   SUBROUTINE

   LDA   ptr_to_arr
   ORA   ptr_to_arr+1
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #<@1.nested_arr
   STA   ptr_to_arr
   LDY   #>@1.nested_arr
   STY   ptr_to_arr+1
   STA   DPTR
   STY   DPTR+1
   LDA   #$64
   LDY   #$20
   STA   (DPTR),Y
   LDA   ptr_to_arr
   STA   DPTR
   LDA   ptr_to_arr+1
   STA   DPTR+1
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   @1.nested_arr+33
   BNE   .jcmp_skip@4
   LDA   @1.nested_arr+32
   CMP   #$64
   BEQ   .if_end@2
.jcmp_skip@4:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

test_array_of_pointers:
   SUBROUTINE

   STA   __local_test_array_of_pointers__ptr
   STX   __local_test_array_of_pointers__i_1
   LDA   #$00
   STA   __local_test_array_of_pointers__i_0
   LDY   #$00
.loop@1_start:
   LDA   __local_test_array_of_pointers__i_0
   SEC
   SBC   #$03
   TYA
   SBC   #$00
   BVC   .jcmp_novf@5
   EOR   #$80
.jcmp_novf@5:
   BPL   .loop@1_break
   LDA   __local_test_array_of_pointers__i_0
   ASL
   TAX
   LDA   array_of_pointers,X
   ORA   array_of_pointers+1,X
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __local_test_array_of_pointers__ptr
   STA   array_of_pointers,X
   LDA   __local_test_array_of_pointers__i_1
   STA   array_of_pointers+1,X
   INC   __local_test_array_of_pointers__i_0
   BNE   .loop@1_start
   INY
   JMP   .loop@1_start
.loop@1_break:
   LDA   array_of_pointers+4
   STA   __local_test_array_of_pointers__0
   LDA   array_of_pointers+5
   STA   __local_test_array_of_pointers__1
   LDA   #$0B
   LDY   #$00
   STA   (__local_test_array_of_pointers__0),Y
   LDA   #$00
   INY
   STA   (__local_test_array_of_pointers__0),Y
   DEY
   LDA   (__local_test_array_of_pointers__ptr),Y
   STA   __local_test_array_of_pointers__1
   INY
   LDA   (__local_test_array_of_pointers__ptr),Y
   BNE   .jcmp_skip@6
   LDA   __local_test_array_of_pointers__1
   CMP   #$0B
   BEQ   .if_end@4
.jcmp_skip@6:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #$00
   STA   __local_test_array_of_pointers__2
   LDY   #$00
   STY   __local_test_array_of_pointers__i_1
.loop@2_start:
   LDA   __local_test_array_of_pointers__2
   SEC
   SBC   #$03
   LDA   __local_test_array_of_pointers__i_1
   SBC   #$00
   BVC   .jcmp_novf@7
   EOR   #$80
.jcmp_novf@7:
   BPL   .loop@2_break
   LDA   __local_test_array_of_pointers__2
   ASL
   TAX
   LDA   array_of_pointers,X
   STA   __local_test_array_of_pointers__i_0
   LDA   array_of_pointers+1,X
   STA   __local_test_array_of_pointers__ptr
   LDY   #$00
   LDA   (__local_test_array_of_pointers__i_0),Y
   STA   __local_test_array_of_pointers__1
   INY
   LDA   (__local_test_array_of_pointers__i_0),Y
   BNE   .jcmp_skip@8
   LDA   __local_test_array_of_pointers__1
   CMP   #$0B
   BEQ   .if_end@5
.jcmp_skip@8:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   INC   __local_test_array_of_pointers__2
   BNE   .loop@2_start
   INC   __local_test_array_of_pointers__i_1
   JMP   .loop@2_start
.loop@2_break:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   JSR   test_arr
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .tail@0
   JSR   test_ptr_to_arr
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .tail@0
   LDA   #$00
   STA   __local_main__x
   STA   __local_main__x+1
   LDX   #>__local_main__x
   LDA   #<__local_main__x
   JSR   test_array_of_pointers
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@8
.tail@0:
   LDA   __local_main__1
   RTS
.if_end@8:
   LDX   #$00
   TXA
   RTS

arr:
   DC.L  $00000001
   DC.L  $00000002
   DC.L  $00000003
   DC.L  $00000004

ptr_to_arr:
   DS.B  2

array_of_pointers:
   DS.B  6

@1.nested_arr:
   DS.B  36
