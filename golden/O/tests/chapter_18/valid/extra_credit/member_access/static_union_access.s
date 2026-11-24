SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__i	EQU	$81
__local_main__1	EQU	$82
__local_main__2	EQU	$83
__local_main__3	EQU	$84
__local_main__4	EQU	$85
__local_main__5	EQU	$86
__local_main__6	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__i,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; item my_union section=.data local=true zero=false
; item union_ptr section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #<my_union
   STA   union_ptr
   LDA   #>my_union
   STA   union_ptr+1
   LDA   my_union+7
   BNE   .jcmp_skip@0
   LDA   my_union+6
   BNE   .jcmp_skip@0
   LDA   my_union+5
   BNE   .jcmp_skip@0
   LDA   my_union+4
   BNE   .jcmp_skip@0
   LDA   my_union+3
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   my_union+2
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   my_union+1
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   my_union
   CMP   #$FF
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$00
   STA   __local_main__i
   LDY   #$00
.loop@0_start:
   LDA   __local_main__i
   SEC
   SBC   #$04
   TYA
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .loop@0_break
   LDX   __local_main__i
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   my_union,X
   CMP   #$FF
   BEQ   .if_end@1
.jcmp_skip@2:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   INC   __local_main__i
   BNE   .loop@0_start
   INY
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$04
   STA   __local_main__i
   LDY   #$00
.loop@1_start:
   LDA   __local_main__i
   SEC
   SBC   #$08
   TYA
   SBC   #$00
   BVC   .jcmp_novf@3
   EOR   #$80
.jcmp_novf@3:
   BPL   .loop@1_break
   LDX   __local_main__i
   LDA   my_union,X
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$07
   RTS
.if_end@2:
   INC   __local_main__i
   BNE   .loop@1_start
   INY
   JMP   .loop@1_start
.loop@1_break:
   JSR   main@run0
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$F0
   INY
   STA   (DPTR),Y
   LDA   #$BF
   INY
   STA   (DPTR),Y
   JSR   main@run0
   LDY   #$00
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
   STA   __local_main__i
   INY
   LDA   (DPTR),Y
   STA   __local_main__0
   LDA   __local_main__6
   ORA   __local_main__5
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__i
   ORA   __local_main__0
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$03
   RTS
.if_end@3:
   LDA   #$00
   STA   __local_main__i
   LDY   #$00
.loop@2_start:
   LDA   __local_main__i
   SEC
   SBC   #$06
   TYA
   SBC   #$00
   BVC   .jcmp_novf@4
   EOR   #$80
.jcmp_novf@4:
   BPL   .loop@2_break
   LDX   __local_main__i
   LDA   my_union,X
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$04
   RTS
.if_end@4:
   INC   __local_main__i
   BNE   .loop@2_start
   INY
   JMP   .loop@2_start
.loop@2_break:
   LDY   #$06
   LDA   (DPTR),Y
   STA   __local_main__0
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __local_main__0
   CMP   #$F0
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@5:
   LDY   #$07
   LDA   (DPTR),Y
   STA   __local_main__0
   LDA   #$00
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __local_main__0
   CMP   #$BF
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@6:
   LDX   #$00
   TXA
   RTS

main@run0:
   SUBROUTINE

   LDA   union_ptr
   STA   DPTR
   LDA   union_ptr+1
   STA   DPTR+1
   RTS

my_union:
   DC.L  $FFFFFFFF
   DC.L  $00000000

union_ptr:
   DS.B  2
