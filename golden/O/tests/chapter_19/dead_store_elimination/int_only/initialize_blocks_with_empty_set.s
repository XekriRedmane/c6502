SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
j	EQU	$26

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals= indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @0.i section=.bss local=true zero=true
; item __zp_init section=.text local=false zero=false
; zp j size=2 at=auto def=true init=$03,$00 weight=25
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   JMP   .loop@0_continue
.target@rot@0:
   ORA   j
   BEQ   .loop@0_break
   LDA   j
   SEC
   SBC   #$01
   STA   j
   BCS   .loop@0_continue
   DEC   j+1
.loop@0_continue:
   LDA   j+1
   BPL   .target@rot@0
.loop@0_break:
   LDA   #$00
   STA   @0.i
   STA   @0.i+1
   LDX   #$00
   RTS

main:
   SUBROUTINE

   JSR   target
   LDA   j+1
   BNE   .thread@0
   LDA   j
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

@0.i:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   j+1
   LDA   #$03
   STA   j
   RTS
