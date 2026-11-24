SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   LDX   foo+1
   LDA   foo
   RTS

foo:
   DC.W  $0003
