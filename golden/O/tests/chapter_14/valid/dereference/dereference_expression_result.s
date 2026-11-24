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
__local_main__val	EQU	$86

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__val,__local_main__7 indirect=false in_cycle=false
; def return_pointer params= locals= indirect=false in_cycle=false
; call main -> return_pointer
; item return_pointer section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @0.var section=.data local=true zero=false
; item one section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

return_pointer:
   SUBROUTINE

   LDX   #>@0.var
   LDA   #<@0.var
   RTS

main:
   SUBROUTINE

   LDA   #$64
   STA   __local_main__val
   LDA   #$00
   STA   __local_main__val+1
   LDA   #<__local_main__val
   STA   __local_main__4
   LDA   #>__local_main__val
   STA   __local_main__5
   JSR   return_pointer
   STA   __local_main__2
   STX   __local_main__3
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   one
   ORA   one+1
   BEQ   .cond_else@1
   JSR   return_pointer
   STA   __local_main__2
   STX   __local_main__3
   JMP   .cond_end@2
.cond_else@1:
   LDA   #<__local_main__val
   STA   __local_main__2
   LDY   #>__local_main__val
   STY   __local_main__3
.cond_end@2:
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   LDA   one
   SEC
   SBC   #$01
   STA   __local_main__1
   LDA   one+1
   SBC   #$00
   ORA   __local_main__1
   BEQ   .cond_else@4
   JSR   return_pointer
   STA   __local_main__4
   STX   __local_main__5
.cond_else@4:
   LDY   #$00
   LDA   (__local_main__4),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__4),Y
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$64
   BEQ   .if_end@6
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@6:
   LDA   one+1
   BNE   .jcmp_skip@3
   LDA   one
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@7:
   JSR   return_pointer
   STA   __local_main__0
   STX   __local_main__1
   LDA   #$14
   LDY   #$00
   STA   (__local_main__0),Y
   LDA   #$00
   INY
   STA   (__local_main__0),Y
   LDA   one
   ORA   one+1
   BEQ   .cond_else@8
   LDA   #<one
   STA   __local_main__0
   LDA   #>one
   STA   __local_main__1
   JMP   .cond_end@9
.cond_else@8:
   JSR   return_pointer
   STA   __local_main__0
   STX   __local_main__1
.cond_end@9:
   LDA   #$1E
   LDY   #$00
   STA   (__local_main__0),Y
   LDA   #$00
   INY
   STA   (__local_main__0),Y
   JSR   return_pointer
   STA   __local_main__2
   STX   __local_main__3
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   CMP   #$14
   BEQ   .if_end@10
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@10:
   LDA   one+1
   BNE   .jcmp_skip@5
   LDA   one
   CMP   #$1E
   BEQ   .if_end@12
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@12:
   LDX   #$00
   TXA
   RTS

@0.var:
   DC.W  $000A

one:
   DC.W  $0001
