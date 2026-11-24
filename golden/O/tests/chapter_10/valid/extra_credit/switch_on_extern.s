SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; def update_x params= locals= indirect=false in_cycle=false
; call main -> update_x
; item main section=.text local=false zero=false
; item update_x section=.text local=false zero=false
; item x section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   JSR   update_x
   LDA   x
   ORA   x+1
   BEQ   .case@1
   LDA   x+1
   BNE   .default@4
   LDA   x
   CMP   #$01
   BEQ   .case@2
   LDA   x+1
   BNE   .default@4
   LDA   x
   CMP   #$04
   BEQ   .case@3
   BNE   .default@4
.case@1:
   LDX   #$00
   LDA   #$01
   RTS
.case@2:
   LDX   #$00
   LDA   #$02
   RTS
.case@3:
   LDX   #$00
   TXA
   RTS
.default@4:
   LDX   #$00
   LDA   #$04
   RTS

update_x:
   SUBROUTINE

   LDA   #$04
   STA   x
   LDA   #$00
   STA   x+1
   LDX   #$00
   RTS

x:
   DS.B  2
