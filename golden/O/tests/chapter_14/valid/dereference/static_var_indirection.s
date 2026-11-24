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
__local_main__l	EQU	$88
__local_main__d	EQU	$90
__local_increment_ptr__0	EQU	$98
__local_modify_ptr__new_ptr_1	EQU	$98
__local_increment_ptr__1	EQU	$99
__local_modify_ptr__new_ptr_0	EQU	$99
__local_increment_ptr__2	EQU	$9A
__local_increment_ptr__3	EQU	$9B
__local_increment_ptr__4	EQU	$9C
__local_increment_ptr__5	EQU	$9D
__local_increment_ptr__6	EQU	$9E

; @zp-link-meta-begin
; def increment_ptr params= locals=__local_increment_ptr__0,__local_increment_ptr__1,__local_increment_ptr__2,__local_increment_ptr__3,__local_increment_ptr__4,__local_increment_ptr__5,__local_increment_ptr__6,__local_increment_ptr__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__l,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__d,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23 indirect=false in_cycle=false
; def modify_ptr params=__zpabi_modify_ptr__new_ptr_0,__zpabi_modify_ptr__new_ptr_1 locals=__local_modify_ptr__new_ptr_1,__local_modify_ptr__new_ptr_0 indirect=false in_cycle=false param_regs=A,X
; call main -> increment_ptr
; call main -> modify_ptr
; item modify_ptr section=.text local=false zero=false
; item increment_ptr section=.text local=false zero=false
; item increment_ptr@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item w section=.data local=false zero=false
; item x section=.data local=false zero=false
; item y section=.data local=false zero=false
; item dbl_ptr section=.bss local=false zero=true
; item @1.p section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

modify_ptr:
   SUBROUTINE

   STA   __local_modify_ptr__new_ptr_0
   STX   __local_modify_ptr__new_ptr_1
   ORA   __local_modify_ptr__new_ptr_1
   BEQ   .if_end@0
   LDA   __local_modify_ptr__new_ptr_0
   STA   @1.p
   STX   @1.p+1
.if_end@0:
   LDA   @1.p
   STA   DPTR
   LDA   @1.p+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   HARGS+16
   INY
   LDA   (DPTR),Y
   STA   HARGS+17
   INY
   LDA   (DPTR),Y
   STA   HARGS+18
   INY
   LDA   (DPTR),Y
   STA   HARGS+19
   INY
   LDA   (DPTR),Y
   STA   HARGS+20
   INY
   LDA   (DPTR),Y
   STA   HARGS+21
   INY
   LDA   (DPTR),Y
   STA   HARGS+22
   INY
   LDA   (DPTR),Y
   STA   HARGS+23
   RTS

increment_ptr:
   SUBROUTINE

   JSR   increment_ptr@run0
   LDY   #$00
   LDA   (DPTR),Y
   STA   HARGS
   INY
   LDA   (DPTR),Y
   STA   HARGS+1
   INY
   LDA   (DPTR),Y
   STA   HARGS+2
   INY
   LDA   (DPTR),Y
   STA   HARGS+3
   INY
   LDA   (DPTR),Y
   STA   HARGS+4
   INY
   LDA   (DPTR),Y
   STA   HARGS+5
   INY
   LDA   (DPTR),Y
   STA   HARGS+6
   INY
   LDA   (DPTR),Y
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$14
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+17
   STA   __local_increment_ptr__6
   LDA   HARGS+18
   STA   __local_increment_ptr__5
   LDA   HARGS+19
   STA   __local_increment_ptr__4
   LDA   HARGS+20
   STA   __local_increment_ptr__3
   LDA   HARGS+21
   STA   __local_increment_ptr__2
   LDA   HARGS+22
   STA   __local_increment_ptr__1
   LDA   HARGS+23
   STA   __local_increment_ptr__0
   JSR   increment_ptr@run0
   LDA   HARGS+16
   LDY   #$00
   STA   (DPTR),Y
   JSR   increment_ptr@run0
   LDA   __local_increment_ptr__6
   LDY   #$01
   STA   (DPTR),Y
   JSR   increment_ptr@run0
   LDA   __local_increment_ptr__5
   LDY   #$02
   STA   (DPTR),Y
   JSR   increment_ptr@run0
   LDA   __local_increment_ptr__4
   LDY   #$03
   STA   (DPTR),Y
   JSR   increment_ptr@run0
   LDA   __local_increment_ptr__3
   LDY   #$04
   STA   (DPTR),Y
   JSR   increment_ptr@run0
   LDA   __local_increment_ptr__2
   LDY   #$05
   STA   (DPTR),Y
   JSR   increment_ptr@run0
   LDA   __local_increment_ptr__1
   LDY   #$06
   STA   (DPTR),Y
   JSR   increment_ptr@run0
   LDA   __local_increment_ptr__0
   LDY   #$07
   STA   (DPTR),Y
   LDX   #$00
   TXA
   RTS

increment_ptr@run0:
   SUBROUTINE

   LDA   dbl_ptr
   STA   DPTR
   LDA   dbl_ptr+1
   STA   DPTR+1
   RTS

main:
   SUBROUTINE

   LDA   #$14
   STA   x
   LDY   #$00
   STY   x+1
   BNE   .jcmp_skip@0
   CMP   #$14
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDA   #$64
   STA   x
   TYA
   BNE   .jcmp_skip@1
   LDA   x
   CMP   #$64
   BEQ   .if_end@2
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   LDA   w+3
   BNE   .jcmp_skip@2
   LDA   w+2
   BNE   .jcmp_skip@2
   LDA   w+1
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   w
   CMP   #$FF
   BEQ   .if_end@3
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@3:
   LDA   y+3
   BNE   .jcmp_skip@3
   LDA   y+2
   BNE   .jcmp_skip@3
   LDA   y+1
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   y
   CMP   #$FF
   BEQ   .if_end@4
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@4:
   LDA   dbl_ptr
   ORA   dbl_ptr+1
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$05
   RTS
.if_end@5:
   LDA   #$E8
   STA   __local_main__l
   LDA   #$03
   STA   __local_main__l+1
   LDA   #$00
   STA   __local_main__l+2
   STA   __local_main__l+3
   STA   __local_main__l+4
   STA   __local_main__l+5
   STA   __local_main__l+6
   STA   __local_main__l+7
   LDX   #>__local_main__l
   LDA   #<__local_main__l
   JSR   modify_ptr
   LDA   HARGS+23
   BNE   .jcmp_skip@4
   LDA   HARGS+22
   BNE   .jcmp_skip@4
   LDA   HARGS+21
   BNE   .jcmp_skip@4
   LDA   HARGS+20
   BNE   .jcmp_skip@4
   LDA   HARGS+19
   BNE   .jcmp_skip@4
   LDA   HARGS+18
   BNE   .jcmp_skip@4
   LDA   HARGS+17
   CMP   #$03
   BNE   .jcmp_skip@4
   LDA   HARGS+16
   CMP   #$E8
   BEQ   .if_end@6
.jcmp_skip@4:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@6:
   LDA   #$FF
   STA   __local_main__l
   STA   __local_main__l+1
   STA   __local_main__l+2
   STA   __local_main__l+3
   STA   __local_main__l+4
   STA   __local_main__l+5
   STA   __local_main__l+6
   STA   __local_main__l+7
   LDX   #$00
   TXA
   JSR   modify_ptr
   LDA   HARGS+23
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   HARGS+22
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   HARGS+21
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   HARGS+20
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   HARGS+19
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   HARGS+18
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   HARGS+17
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   HARGS+16
   CMP   #$FF
   BEQ   .if_end@7
.jcmp_skip@5:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@7:
   LDA   #$00
   STA   __local_main__d
   STA   __local_main__d+1
   STA   __local_main__d+2
   STA   __local_main__d+3
   STA   __local_main__d+4
   STA   __local_main__d+5
   LDA   #$24
   STA   __local_main__d+6
   LDA   #$40
   STA   __local_main__d+7
   LDA   #<__local_main__d
   STA   dbl_ptr
   LDA   #>__local_main__d
   STA   dbl_ptr+1
   JSR   increment_ptr
   JSR   increment_ptr@run0
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_main__7
   INY
   LDA   (DPTR),Y
   STA   __local_main__6
   INY
   LDA   (DPTR),Y
   STA   __local_main__5
   INY
   LDA   (DPTR),Y
   STA   __local_main__4
   INY
   LDA   (DPTR),Y
   STA   __local_main__3
   INY
   LDA   (DPTR),Y
   STA   __local_main__2
   INY
   LDA   (DPTR),Y
   STA   __local_main__1
   INY
   LDA   (DPTR),Y
   CMP   #$40
   BNE   .jcmp_skip@6
   LDA   __local_main__1
   CMP   #$2E
   BNE   .jcmp_skip@6
   LDA   __local_main__2
   BNE   .jcmp_skip@6
   LDA   __local_main__3
   BNE   .jcmp_skip@6
   LDA   __local_main__4
   BNE   .jcmp_skip@6
   LDA   __local_main__5
   BNE   .jcmp_skip@6
   LDA   __local_main__6
   BNE   .jcmp_skip@6
   LDA   __local_main__7
   BEQ   .if_end@8
.jcmp_skip@6:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@8:
   LDX   #$00
   TXA
   RTS

w:
   DC.L  $0000FFFF

x:
   DC.W  $000A

y:
   DC.L  $0000FFFF

dbl_ptr:
   DS.B  2

@1.p:
   DS.B  2
