SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; ext update_outer_struct params=
; ext update_struct params=
; call main -> update_outer_struct
; call main -> update_struct
; item main section=.text local=false zero=false
; item global section=.data local=false zero=false
; item global_outer section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   JSR   update_struct
   LDA   global+3
   CMP   #$04
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   global+11
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   global+10
   CMP   #$14
   BNE   .jcmp_skip@0
   LDA   global+9
   BNE   .jcmp_skip@0
   LDA   global+8
   BNE   .jcmp_skip@0
   LDA   global+7
   BNE   .jcmp_skip@0
   LDA   global+6
   BNE   .jcmp_skip@0
   LDA   global+5
   BNE   .jcmp_skip@0
   LDA   global+4
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   update_outer_struct
   LDA   global_outer
   CMP   #$05
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   global_outer+1
   ORA   global_outer+2
   BNE   .main@jthread@0
   LDA   global_outer+5
   ORA   global_outer+6
   ORA   global_outer+7
   ORA   global_outer+8
   ORA   global_outer+9
   ORA   global_outer+10
   ORA   global_outer+11
   ORA   global_outer+12
   BEQ   .main@jthread@1
.main@jthread@0:
   LDX   #$00
   LDA   #$04
   RTS
.main@jthread@1:
   LDX   #$00
   LDA   #$05
   RTS

global:
   DC.W  $0001
   DC.B  $02, $03
   DC.L  $00000000
   DC.L  $40100000

global_outer:
   DC.B  $05
   DC.W  $0006
   DC.B  $07, $08
   DC.L  $00000000
   DC.L  $40220000
