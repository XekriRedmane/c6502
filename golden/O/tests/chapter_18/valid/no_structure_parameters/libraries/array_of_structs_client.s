SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__auto_array	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__auto_array,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22 indirect=false in_cycle=false
; ext validate_struct_array params=__zpabi_validate_struct_array__struct_array_0,__zpabi_validate_struct_array__struct_array_1 param_regs=A,X
; call main -> validate_struct_array
; item main section=.text local=false zero=false
; item static_array section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__auto_array
   LDY   #$00
   STY   __local_main__auto_array+1
   STY   __local_main__auto_array+2
   STY   __local_main__auto_array+3
   STY   __local_main__auto_array+4
   STY   __local_main__auto_array+5
   STA   __local_main__auto_array+6
   LDA   #$02
   STA   __local_main__auto_array+7
   LDA   #$03
   STA   __local_main__auto_array+8
   TYA
   STA   __local_main__auto_array+9
   STY   __local_main__auto_array+10
   STA   __local_main__auto_array+11
   LDA   #$04
   STA   __local_main__auto_array+12
   LDY   #$05
   STY   __local_main__auto_array+13
   STA   __local_main__auto_array+14
   LDA   #$06
   STA   __local_main__auto_array+15
   LDA   #$00
   STA   __local_main__auto_array+16
   LDY   #$00
   STY   __local_main__auto_array+17
   STA   __local_main__auto_array+18
   LDA   #$08
   STA   __local_main__auto_array+19
   LDA   #$0A
   STA   __local_main__auto_array+20
   LDX   #>static_array
   LDA   #<static_array
   JSR   validate_struct_array
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #>__local_main__auto_array
   LDA   #<__local_main__auto_array
   JSR   validate_struct_array
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

static_array:
   DS.B  7
   DC.B  $02
   DC.L  $00000003
   DC.B  $04, $05
   DC.B  $04
   DC.L  $00000006
   DC.B  $08, $0A
