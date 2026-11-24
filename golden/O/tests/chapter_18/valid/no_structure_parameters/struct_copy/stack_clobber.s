SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_validate_array__start_0	EQU	$80
__zpabi_validate_array__start_1	EQU	$81
__zpabi_validate_array__code_0	EQU	$82
__zpabi_validate_static__code_0	EQU	$82
__zpabi_validate_array__code_1	EQU	$83
__zpabi_validate_static__code_1	EQU	$83
__local_test_copy_to_offset__0	EQU	$84
__local_test_store__0	EQU	$84
__local_test_copy_to_offset__1	EQU	$85
__local_test_store__1	EQU	$85
__local_test_copy__a	EQU	$86
__local_test_copy_to_offset__2	EQU	$86
__local_test_store__2	EQU	$86
__local_test_copy_to_offset__container	EQU	$87
__local_test_store__struct_array	EQU	$87
__local_test_copy__b	EQU	$89
__local_test_copy__c	EQU	$8C
__local_validate_array__0	EQU	$90
__local_validate_array__1	EQU	$91
__local_validate_array__2	EQU	$92
__local_validate_array__i_1	EQU	$93
__local_validate_array__i_0	EQU	$94
__local_validate_array__char_array_1	EQU	$95
__local_validate_array__char_array_0	EQU	$96

; @zp-link-meta-begin
; def increment_y params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def test_copy params= locals=__local_test_copy__0,__local_test_copy__1,__local_test_copy__a,__local_test_copy__3,__local_test_copy__4,__local_test_copy__b,__local_test_copy__6,__local_test_copy__7,__local_test_copy__c,__local_test_copy__9,__local_test_copy__10 indirect=false in_cycle=false
; def test_copy_from_offset params= locals=__local_test_copy_from_offset__0,__local_test_copy_from_offset__1,__local_test_copy_from_offset__a_2,__local_test_copy_from_offset__a_1,__local_test_copy_from_offset__a_0 indirect=false in_cycle=false
; def test_copy_to_offset params= locals=__local_test_copy_to_offset__0,__local_test_copy_to_offset__1,__local_test_copy_to_offset__2,__local_test_copy_to_offset__container,__local_test_copy_to_offset__4,__local_test_copy_to_offset__5,__local_test_copy_to_offset__6,__local_test_copy_to_offset__7,__local_test_copy_to_offset__8,__local_test_copy_to_offset__9 indirect=false in_cycle=false
; def test_load params= locals=__local_test_load__a_2,__local_test_load__a_1,__local_test_load__a_0 indirect=false in_cycle=false
; def test_store params= locals=__local_test_store__0,__local_test_store__1,__local_test_store__2,__local_test_store__struct_array,__local_test_store__4,__local_test_store__5,__local_test_store__6,__local_test_store__7,__local_test_store__8,__local_test_store__9,__local_test_store__10,__local_test_store__11 indirect=false in_cycle=false
; def validate_array params=__zpabi_validate_array__char_array_0,__zpabi_validate_array__char_array_1,__zpabi_validate_array__start_0,__zpabi_validate_array__start_1,__zpabi_validate_array__code_0,__zpabi_validate_array__code_1 locals=__local_validate_array__0,__local_validate_array__1,__local_validate_array__2,__local_validate_array__i_1,__local_validate_array__i_0,__local_validate_array__char_array_1,__local_validate_array__char_array_0 indirect=false in_cycle=false param_regs=A,X,,,,
; def validate_static params=__zpabi_validate_static__start_0,__zpabi_validate_static__start_1,__zpabi_validate_static__code_0,__zpabi_validate_static__code_1 locals=__local_validate_static__start_1 indirect=false in_cycle=false param_regs=A,X,,
; ext exit params=__zpabi_exit__status_0,__zpabi_exit__status_1 param_regs=A,X
; call main -> increment_y
; call main -> test_copy
; call main -> test_copy_from_offset
; call main -> test_copy_to_offset
; call main -> test_load
; call main -> test_store
; call test_copy -> validate_array
; call test_copy_from_offset -> validate_static
; call test_copy_to_offset -> exit
; call test_copy_to_offset -> validate_array
; call test_load -> validate_static
; call test_store -> validate_array
; call validate_array -> exit
; call validate_static -> validate_array
; share member=validate_array:__zpabi_validate_array__code_0,__zpabi_validate_array__code_1 member=validate_static:__zpabi_validate_static__code_0,__zpabi_validate_static__code_1 forward=validate_static>validate_array
; item validate_array section=.text local=false zero=false
; item increment_y section=.text local=false zero=false
; item test_copy section=.text local=false zero=false
; item test_copy@run0 section=.text local=true zero=false
; item test_copy@tail0 section=.text local=true zero=false
; item validate_static section=.text local=false zero=false
; item test_load section=.text local=false zero=false
; item test_load@run2 section=.text local=true zero=false
; item test_load@tail1 section=.text local=true zero=false
; item test_store section=.text local=false zero=false
; item test_store@run1 section=.text local=true zero=false
; item test_copy_from_offset section=.text local=false zero=false
; item test_copy_to_offset section=.text local=false zero=false
; item main section=.text local=false zero=false
; item y section=.data local=true zero=false
; item ptr section=.bss local=true zero=true
; item to_validate section=.bss local=true zero=true
; item @10.b section=.bss local=true zero=true
; item @15.b section=.data local=true zero=false
; item @16.container section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

validate_array:
   SUBROUTINE

   STA   __local_validate_array__char_array_0
   STX   __local_validate_array__char_array_1
   LDA   #$00
   STA   __local_validate_array__i_0
   LDY   #$00
   STY   __local_validate_array__i_1
.loop@0_start:
   LDA   __local_validate_array__i_0
   SEC
   SBC   #$03
   LDA   __local_validate_array__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_validate_array__char_array_0
   CLC
   ADC   __local_validate_array__i_0
   STA   __local_validate_array__0
   LDA   __local_validate_array__char_array_1
   ADC   __local_validate_array__i_1
   STA   __local_validate_array__1
   LDY   #$00
   LDA   (__local_validate_array__0),Y
   STA   __local_validate_array__2
   LDA   __zpabi_validate_array__start_0
   CLC
   ADC   __local_validate_array__i_0
   STA   __local_validate_array__1
   LDA   __zpabi_validate_array__start_1
   ADC   __local_validate_array__i_1
   STA   __local_validate_array__0
   LDA   #$00
   CMP   __local_validate_array__0
   BNE   .jcmp_skip@1
   LDA   __local_validate_array__2
   CMP   __local_validate_array__1
   BEQ   .if_end@0
.jcmp_skip@1:
   LDX   __zpabi_validate_array__code_1
   LDA   __zpabi_validate_array__code_0
   JSR   exit
.if_end@0:
   INC   __local_validate_array__i_0
   BNE   .loop@0_start
   INC   __local_validate_array__i_1
   JMP   .loop@0_start
.loop@0_break:
   RTS

increment_y:
   SUBROUTINE

   LDA   y
   CLC
   ADC   #$03
   STA   y
   LDA   y+1
   CLC
   ADC   #$03
   STA   y+1
   LDA   y+2
   CLC
   ADC   #$03
   STA   y+2
   RTS

test_copy:
   SUBROUTINE

   LDA   #$61
   STA   __local_test_copy__a
   LDA   #$62
   STA   __local_test_copy__a+1
   LDA   #$63
   STA   __local_test_copy__a+2
   LDA   #$64
   STA   __local_test_copy__c
   LDA   #$65
   STA   __local_test_copy__c+1
   LDA   #$66
   STA   __local_test_copy__c+2
   LDA   y
   STA   __local_test_copy__b
   LDA   y+1
   STA   __local_test_copy__b+1
   LDA   y+2
   STA   __local_test_copy__b+2
   LDA   #$61
   STA   __zpabi_validate_array__start_0
   LDA   #$01
   JSR   test_copy@run0
   LDX   #>__local_test_copy__a
   LDA   #<__local_test_copy__a
   JSR   validate_array
   LDA   #$02
   STA   __zpabi_validate_array__code_0
   LDA   #$00
   STA   __zpabi_validate_array__start_0
   STA   __zpabi_validate_array__start_1
   STA   __zpabi_validate_array__code_1
   LDX   #>__local_test_copy__b
   LDA   #<__local_test_copy__b
   JSR   validate_array
   LDA   #$64
   STA   __zpabi_validate_array__start_0
   LDA   #$03
   JSR   test_copy@run0
   LDX   #>__local_test_copy__c
   LDA   #<__local_test_copy__c
   JMP   test_copy@tail0

test_copy@run0:
   SUBROUTINE

   STA   __zpabi_validate_array__code_0
   LDA   #$00
   STA   __zpabi_validate_array__start_1
   STA   __zpabi_validate_array__code_1
   RTS

test_copy@tail0:
   SUBROUTINE

   JSR   validate_array
   LDX   #$00
   TXA
   RTS

validate_static:
   SUBROUTINE

   STA   __zpabi_validate_array__start_0
   STX   __zpabi_validate_array__start_1
   LDX   #>to_validate
   LDA   #<to_validate
   JMP   validate_array

test_load:
   SUBROUTINE

   LDA   ptr
   STA   DPTR
   LDA   ptr+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   @10.b
   INY
   LDA   (DPTR),Y
   STA   @10.b+1
   INY
   LDA   (DPTR),Y
   STA   @10.b+2
   LDA   #$67
   STA   to_validate
   LDA   #$68
   STA   to_validate+1
   LDA   #$69
   STA   to_validate+2
   LDA   #$04
   STA   __zpabi_validate_static__code_0
   JSR   test_load@run2
   LDA   #$67
   JSR   validate_static
   LDA   @10.b
   STA   to_validate
   LDA   @10.b+1
   STA   to_validate+1
   LDA   @10.b+2
   STA   to_validate+2
   LDA   #$05
   STA   __zpabi_validate_static__code_0
   JSR   test_load@run2
   LDA   #$03
   JMP   test_load@tail1

test_load@run2:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_validate_static__code_1
   LDX   #$00
   RTS

test_load@tail1:
   SUBROUTINE

   JSR   validate_static
   LDX   #$00
   TXA
   RTS

test_store:
   SUBROUTINE

   LDA   #$6A
   STA   __local_test_store__struct_array
   LDA   #$6B
   STA   __local_test_store__struct_array+1
   LDA   #$6C
   STA   __local_test_store__struct_array+2
   LDA   #$78
   STA   __local_test_store__struct_array+3
   LDA   #$79
   STA   __local_test_store__struct_array+4
   LDA   #$7A
   STA   __local_test_store__struct_array+5
   LDA   #$6D
   STA   __local_test_store__struct_array+6
   LDA   #$6E
   STA   __local_test_store__struct_array+7
   LDA   #$6F
   STA   __local_test_store__struct_array+8
   LDA   #>__local_test_store__struct_array
   STA   __local_test_store__1
   LDA   #<__local_test_store__struct_array
   CLC
   ADC   #$03
   STA   __local_test_store__0
   BCC   .carry_inc@0
   INC   __local_test_store__1
.carry_inc@0:
   STA   DPTR
   LDA   __local_test_store__1
   STA   DPTR+1
   LDA   y
   LDY   #$00
   STA   (__local_test_store__0),Y
   JSR   test_store@run1
   LDA   #>__local_test_store__struct_array
   STA   __local_test_store__2
   CLC
   BCC   .carry_inc@1
   INC   __local_test_store__2
.carry_inc@1:
   LDA   #$6A
   STA   __zpabi_validate_array__start_0
   LDA   #$06
   JSR   test_copy@run0
   LDX   __local_test_store__2
   LDA   #<__local_test_store__struct_array
   JSR   validate_array
   LDA   #>__local_test_store__struct_array
   STA   __local_test_store__2
   LDA   #<__local_test_store__struct_array
   CLC
   ADC   #$03
   STA   __local_test_store__0
   BCC   .carry_inc@2
   INC   __local_test_store__2
.carry_inc@2:
   LDA   #$06
   STA   __zpabi_validate_array__start_0
   LDA   #$07
   JSR   test_copy@run0
   LDX   __local_test_store__2
   LDA   __local_test_store__0
   JSR   validate_array
   LDA   #>__local_test_store__struct_array
   STA   __local_test_store__2
   LDA   #<__local_test_store__struct_array
   CLC
   ADC   #$06
   STA   __local_test_store__0
   BCC   .carry_inc@3
   INC   __local_test_store__2
.carry_inc@3:
   LDA   #$6D
   STA   __zpabi_validate_array__start_0
   LDA   #$08
   JSR   test_copy@run0
   LDX   __local_test_store__2
   LDA   __local_test_store__0
   JMP   test_copy@tail0

test_store@run1:
   SUBROUTINE

   LDA   y+1
   LDY   #$01
   STA   (DPTR),Y
   LDA   y+2
   INY
   STA   (DPTR),Y
   RTS

test_copy_from_offset:
   SUBROUTINE

   LDA   @16.container+1
   STA   @15.b
   LDA   @16.container+2
   STA   @15.b+1
   LDA   @16.container+3
   STA   @15.b+2
   LDA   #$70
   STA   to_validate
   LDA   #$71
   STA   to_validate+1
   LDA   #$72
   STA   to_validate+2
   LDA   #$09
   STA   __zpabi_validate_static__code_0
   JSR   test_load@run2
   LDA   #$70
   JSR   validate_static
   LDA   @15.b
   STA   to_validate
   LDA   @15.b+1
   STA   to_validate+1
   LDA   @15.b+2
   STA   to_validate+2
   LDA   #$0A
   STA   __zpabi_validate_static__code_0
   JSR   test_load@run2
   LDA   #$09
   JMP   test_load@tail1

test_copy_to_offset:
   SUBROUTINE

   LDA   #$78
   STA   __local_test_copy_to_offset__container
   LDA   #$00
   STA   __local_test_copy_to_offset__container+1
   LDY   #$00
   STY   __local_test_copy_to_offset__container+2
   STA   __local_test_copy_to_offset__container+3
   LDA   #$73
   STA   __local_test_copy_to_offset__container+4
   LDA   #$74
   STA   __local_test_copy_to_offset__container+5
   LDA   #$75
   STA   __local_test_copy_to_offset__container+6
   LDA   #<__local_test_copy_to_offset__container
   STA   __local_test_copy_to_offset__0
   LDA   #>__local_test_copy_to_offset__container
   STA   __local_test_copy_to_offset__1
   INC   __local_test_copy_to_offset__0
   BNE   .inc_done@1
   INC   __local_test_copy_to_offset__1
.inc_done@1:
   LDA   __local_test_copy_to_offset__0
   STA   DPTR
   LDA   __local_test_copy_to_offset__1
   STA   DPTR+1
   LDA   y
   LDY   #$00
   STA   (__local_test_copy_to_offset__0),Y
   JSR   test_store@run1
   LDA   __local_test_copy_to_offset__container
   CMP   #$78
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$0B
   JSR   exit
.if_end@1:
   LDA   #>__local_test_copy_to_offset__container
   STA   __local_test_copy_to_offset__2
   LDA   #<__local_test_copy_to_offset__container
   CLC
   ADC   #$01
   STA   __local_test_copy_to_offset__0
   BCC   .carry_inc@4
   INC   __local_test_copy_to_offset__2
.carry_inc@4:
   LDA   #$0C
   STA   __zpabi_validate_array__start_0
   JSR   test_copy@run0
   LDX   __local_test_copy_to_offset__2
   LDA   __local_test_copy_to_offset__0
   JSR   validate_array
   LDA   #>__local_test_copy_to_offset__container
   STA   __local_test_copy_to_offset__2
   LDA   #<__local_test_copy_to_offset__container
   CLC
   ADC   #$04
   STA   __local_test_copy_to_offset__0
   BCC   .carry_inc@5
   INC   __local_test_copy_to_offset__2
.carry_inc@5:
   LDA   #$73
   STA   __zpabi_validate_array__start_0
   LDA   #$0D
   JSR   test_copy@run0
   LDX   __local_test_copy_to_offset__2
   LDA   __local_test_copy_to_offset__0
   JMP   test_copy@tail0

main:
   SUBROUTINE

   LDA   #<y
   STA   ptr
   LDA   #>y
   STA   ptr+1
   JSR   test_copy
   JSR   increment_y
   JSR   test_load
   JSR   increment_y
   JSR   test_store
   JSR   increment_y
   JSR   test_copy_from_offset
   JSR   increment_y
   JSR   test_copy_to_offset
   LDX   #$00
   TXA
   RTS

y:
   DC.B  $00, $01, $02

ptr:
   DS.B  2

to_validate:
   DS.B  3

@10.b:
   DS.B  3

@15.b:
   DC.B  $78, $79, $7A

@16.container:
   DC.B  $64
   DC.B  $09, $0A, $0B
   DC.B  $31, $32, $33
