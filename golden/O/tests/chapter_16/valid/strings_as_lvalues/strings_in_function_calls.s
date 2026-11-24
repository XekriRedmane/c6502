SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_pass_string_args__s2_0	EQU	$80
__zpabi_pass_string_args__s2_1	EQU	$81
__local_main__0	EQU	$82
__local_main__1	EQU	$83
__local_main__2	EQU	$84
__local_pass_string_args__s1	EQU	$89
__local_pass_string_args__4	EQU	$8A

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2 indirect=false in_cycle=false
; def pass_string_args params=__zpabi_pass_string_args__s1_0,__zpabi_pass_string_args__s1_1,__zpabi_pass_string_args__s2_0,__zpabi_pass_string_args__s2_1 locals=__local_pass_string_args__0,__local_pass_string_args__1,__local_pass_string_args__2,__local_pass_string_args__3,__local_pass_string_args__s1,__local_pass_string_args__4 indirect=false in_cycle=false param_regs=A,X,,
; def return_string params= locals= indirect=false in_cycle=false
; ext strlen params=__zpabi_strlen__s_0,__zpabi_strlen__s_1 param_regs=A,X
; call main -> pass_string_args
; call main -> return_string
; call pass_string_args -> strlen
; item return_string section=.text local=false zero=false
; item pass_string_args section=.text local=false zero=false
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=true
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

return_string:
   SUBROUTINE

   LDX   #>str@0
   LDA   #<str@0
   RTS

pass_string_args:
   SUBROUTINE

   STA   __local_pass_string_args__s1
   STX   __local_pass_string_args__4
   ORA   __local_pass_string_args__4
   BEQ   .tail@0
   LDA   __zpabi_pass_string_args__s2_0
   ORA   __zpabi_pass_string_args__s2_1
   BEQ   .tail@0
   LDA   __local_pass_string_args__s1
   JSR   strlen
   LDA   HARGS+11
   BNE   .tail@0
   LDA   HARGS+10
   BNE   .tail@0
   LDA   HARGS+9
   BNE   .tail@0
   LDA   HARGS+8
   CMP   #$2D
   BNE   .tail@0
   LDY   #$29
   LDA   (__local_pass_string_args__s1),Y
   CMP   #$64
   BNE   .tail@0
   INY
   LDA   (__local_pass_string_args__s1),Y
   CMP   #$6F
   BNE   .tail@0
   INY
   LDA   (__local_pass_string_args__s1),Y
   CMP   #$67
   BNE   .tail@0
   LDY   #$00
   LDA   (__zpabi_pass_string_args__s2_0),Y
   BEQ   .if_end@9
.tail@0:
   LDX   #$00
   TXA
   RTS
.if_end@9:
   LDX   #$00
   LDA   #$01
   RTS

main:
   SUBROUTINE

   JSR   return_string
   STA   __local_main__1
   STX   __local_main__2
   ORA   __local_main__2
   BNE   .if_end@10
   LDX   #$00
   LDA   #$01
   RTS
.if_end@10:
   LDY   #$00
   LDA   (__local_main__1),Y
   CMP   #$49
   BNE   .main@jthread@1
   INY
   LDA   (__local_main__1),Y
   CMP   #$27
   BNE   .main@jthread@1
   LDY   #$0D
   LDA   (__local_main__1),Y
   BEQ   .if_end@15
.main@jthread@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@15:
   LDA   #<str@2
   STA   __zpabi_pass_string_args__s2_0
   LDA   #>str@2
   STA   __zpabi_pass_string_args__s2_1
   LDX   #>str@1
   LDA   #<str@1
   JSR   pass_string_args
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@16
   LDX   #$00
   LDA   #$03
   RTS
.if_end@16:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $49, $27, $6D, $20, $61, $20, $73, $74, $72, $69, $6E, $67, $21, $00

str@1:
   DC.B  $54, $68, $65, $20, $71, $75, $69, $63, $6B, $20, $62, $72, $6F, $77, $6E, $20
   DC.B  $66, $6F, $78, $20, $6A, $75, $6D, $70, $65, $64, $20, $6F, $76, $65, $72, $20
   DC.B  $74, $68, $65, $20, $6C, $61, $7A, $79, $20, $64, $6F, $67, $2E, $00

str@2:
   DS.B  1
