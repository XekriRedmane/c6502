SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def update_global_chars params= locals= indirect=false in_cycle=false
; item update_global_chars section=.text local=false zero=false
; item c section=.data local=false zero=false
; item uc section=.data local=false zero=false
; item sc section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

update_global_chars:
   SUBROUTINE

   LDA   c
   CLC
   ADC   #$0A
   STA   c
   LDA   uc
   CLC
   ADC   #$0A
   STA   uc
   LDA   sc
   SEC
   SBC   #$0A
   STA   sc
   LDX   #$00
   TXA
   RTS

c:
   DC.B  $64

uc:
   DC.B  $FA

sc:
   DS.B  1
