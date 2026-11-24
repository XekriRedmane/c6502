SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i	EQU	$80
__local_main__0	EQU	$81
__local_main__x	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__i,__local_main__0,__local_main__x,__local_main__2 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$0A
   STA   __local_main__x
   LDA   #$00
   STA   __local_main__x+1
   LDA   #<__local_main__x
   STA   __local_main__i
   LDY   #>__local_main__x
   STY   __local_main__0
   JMP   .loop@0_start
.main@rot@0:
   LDA   #$05
   LDY   #$00
   STA   (__local_main__i),Y
   LDA   #$00
   INY
   STA   (__local_main__i),Y
   STA   __local_main__i
   STA   __local_main__0
.loop@0_start:
   ORA   __local_main__0
   BNE   .main@rot@0
   LDX   __local_main__x+1
   LDA   __local_main__x
   RTS
