SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   LDX   @0.x+1
   LDA   @0.x
   RTS

@0.x:
   DC.W  $000A
