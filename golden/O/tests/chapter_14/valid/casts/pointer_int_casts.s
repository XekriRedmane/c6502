SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def cast_int_round_trip params= locals=__local_cast_int_round_trip__0,__local_cast_int_round_trip__1 indirect=false in_cycle=false
; def cast_long_round_trip params= locals=__local_cast_long_round_trip__0,__local_cast_long_round_trip__1 indirect=false in_cycle=false
; def cast_ulong_round_trip params= locals=__local_cast_ulong_round_trip__0 indirect=false in_cycle=false
; def int_to_pointer params= locals=__local_int_to_pointer__0,__local_int_to_pointer__1,__local_int_to_pointer__2,__local_int_to_pointer__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def pointer_to_int params= locals=__local_pointer_to_int__0 indirect=false in_cycle=false
; call main -> cast_int_round_trip
; call main -> cast_long_round_trip
; call main -> cast_ulong_round_trip
; call main -> int_to_pointer
; call main -> pointer_to_int
; item int_to_pointer section=.text local=false zero=false
; item pointer_to_int section=.text local=false zero=false
; item cast_long_round_trip section=.text local=false zero=false
; item cast_ulong_round_trip section=.text local=false zero=false
; item cast_int_round_trip section=.text local=false zero=false
; item main section=.text local=false zero=false
; item i section=.data local=false zero=false
; item l section=.data local=false zero=false
; item @2.l section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

int_to_pointer:
   SUBROUTINE

   LDA   i+1
   CMP   l+1
   BNE   .thread@0
   LDA   i
   CMP   l
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

pointer_to_int:
   SUBROUTINE

   LDA   #>@2.l
   BNE   .cmp_true@3
   LDA   #<@2.l
   BNE   .cmp_true@3
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

cast_long_round_trip:
   SUBROUTINE

   LDA   l+3
   BNE   .thread@1
   LDA   l+2
   BNE   .thread@1
   LDA   l+1
   CMP   l+1
   BNE   .thread@1
   LDA   l
   CMP   l
   BEQ   .cmp_true@6
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDX   #$00
   RTS

cast_ulong_round_trip:
   SUBROUTINE

   LDA   #>l
   CMP   #>l
   BNE   .thread@2
   LDA   #<l
   CMP   #<l
   BEQ   .cmp_true@9
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDX   #$00
   RTS

cast_int_round_trip:
   SUBROUTINE

   LDA   i+1
   BNE   .thread@3
   LDA   i
   CMP   #$08
   BEQ   .cmp_true@12
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   JSR   int_to_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   pointer_to_int
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   cast_long_round_trip
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   JSR   cast_ulong_round_trip
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JSR   cast_int_round_trip
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDX   #$00
   TXA
   RTS

i:
   DC.W  $0008

l:
   DC.L  $00000008

@2.l:
   DS.B  4
