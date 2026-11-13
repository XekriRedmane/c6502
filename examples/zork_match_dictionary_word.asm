SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__i	EQU	$81
__local_main__1	EQU	$82
__local_main__2	EQU	$83
__local_main__3	EQU	$84
__local_main__4	EQU	$85
__local_match_dictionary_word__0	EQU	$86
__local_match_dictionary_word__entry_index_0	EQU	$88
__local_match_dictionary_word__entry_index_1	EQU	$89
__local_match_dictionary_word__2	EQU	$8A
__local_match_dictionary_word__3	EQU	$8B
__local_match_dictionary_word__entry_0	EQU	$8C
__local_match_dictionary_word__entry_1	EQU	$8D
__local_get_dictionary_addr__0	EQU	$8E
__local_get_dictionary_addr__1	EQU	$8F
z_header_addr	EQU	$BA
zchar_scratch2	EQU	$DA

; @zp-link-meta-begin
; def get_dictionary_addr params= locals=__local_get_dictionary_addr__0,__local_get_dictionary_addr__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__i,__local_main__1,__local_main__2,__local_main__3,__local_main__4 indirect=false in_cycle=false
; def match_dictionary_word params= locals=__local_match_dictionary_word__0,__local_match_dictionary_word__1,__local_match_dictionary_word__entry_index_0,__local_match_dictionary_word__entry_index_1,__local_match_dictionary_word__2,__local_match_dictionary_word__3,__local_match_dictionary_word__entry_0,__local_match_dictionary_word__entry_1 indirect=false in_cycle=false
; call main -> match_dictionary_word
; call match_dictionary_word -> get_dictionary_addr
; item get_dictionary_addr section=.text local=false zero=false
; item match_dictionary_word section=.text local=false zero=false
; item main section=.text local=false zero=false
; item story section=.bss local=true zero=true
; item __zp_init section=.text local=false zero=false
; zp z_header_addr size=2 at=$BA def=true init=$00,$00
; zp zchar_scratch2 size=6 at=$DA def=true init=$00,$00,$00,$00,$00,$00
; runtime hargs_size=32 hargs_need=2
; @zp-link-meta-end

get_dictionary_addr:
   SUBROUTINE

   LDY   #$08
   LDA   (z_header_addr),Y
   STA   __local_get_dictionary_addr__0
   INY
   LDA   (z_header_addr),Y
   STA   __local_get_dictionary_addr__1
   LDA   z_header_addr
   CLC
   ADC   __local_get_dictionary_addr__1
   STA   HARGS
   LDA   z_header_addr+1
   ADC   __local_get_dictionary_addr__0
   STA   HARGS+1
   RTS

match_dictionary_word:
   SUBROUTINE

   JSR   get_dictionary_addr
   LDA   HARGS
   STA   __local_match_dictionary_word__entry_0
   LDA   HARGS+1
   STA   __local_match_dictionary_word__entry_1
   LDY   #$00
   LDA   (__local_match_dictionary_word__entry_0),Y
   STA   __local_match_dictionary_word__0
   LDA   HARGS
   CLC
   ADC   __local_match_dictionary_word__0
   STA   __local_match_dictionary_word__entry_0
   BCC   .carry_inc@0
   INC   __local_match_dictionary_word__entry_1
.carry_inc@0:
   LDY   #$01
   LDA   (__local_match_dictionary_word__entry_0),Y
   STA   __local_match_dictionary_word__3
   ASL
   ASL
   ASL
   ASL
   STA   __local_match_dictionary_word__2
   INY
   LDA   (__local_match_dictionary_word__entry_0),Y
   STA   __local_match_dictionary_word__0
   INY
   LDA   (__local_match_dictionary_word__entry_0),Y
   STA   __local_match_dictionary_word__entry_index_0
   LDA   __local_match_dictionary_word__0
   STA   __local_match_dictionary_word__entry_index_1
   LDA   __local_match_dictionary_word__entry_0
   CLC
   ADC   #$04
   STA   __local_match_dictionary_word__entry_0
   BCC   .carry_inc@1
   INC   __local_match_dictionary_word__entry_1
.carry_inc@1:
.loop@0_start:
   LDA   __local_match_dictionary_word__entry_0
   CLC
   ADC   __local_match_dictionary_word__2
   STA   __local_match_dictionary_word__entry_0
   BCC   .carry_inc@2
   INC   __local_match_dictionary_word__entry_1
.carry_inc@2:
   LDA   __local_match_dictionary_word__entry_index_0
   SEC
   SBC   #$10
   STA   __local_match_dictionary_word__entry_index_0
   BCS   .carry_inc@3
   DEC   __local_match_dictionary_word__entry_index_1
.carry_inc@3:
   LDA   __local_match_dictionary_word__entry_index_1
   BMI   .loop@0_break
   LDY   #$00
   LDA   (__local_match_dictionary_word__entry_0),Y
   CMP   zchar_scratch2+1
   BCC   .loop@0_start
.loop@0_break:
   LDA   __local_match_dictionary_word__entry_0
   SEC
   SBC   __local_match_dictionary_word__2
   STA   __local_match_dictionary_word__entry_0
   BCS   .carry_inc@4
   DEC   __local_match_dictionary_word__entry_1
.carry_inc@4:
   LDA   __local_match_dictionary_word__entry_index_0
   CLC
   ADC   #$10
   STA   __local_match_dictionary_word__entry_index_0
   BCC   .carry_inc@5
   INC   __local_match_dictionary_word__entry_index_1
.carry_inc@5:
.loop@1_start:
   LDY   #$00
   LDA   zchar_scratch2+1
   CMP   (__local_match_dictionary_word__entry_0),Y
   BCC   .tail@0
   BNE   .if_end@4
   INY
   LDA   zchar_scratch2
   CMP   (__local_match_dictionary_word__entry_0),Y
   BCC   .tail@0
   BNE   .if_end@6
   INY
   LDA   zchar_scratch2+3
   CMP   (__local_match_dictionary_word__entry_0),Y
   BCC   .tail@0
   BNE   .if_end@8
   INY
   LDA   zchar_scratch2+2
   CMP   (__local_match_dictionary_word__entry_0),Y
   BCC   .tail@0
   BNE   .if_end@10
   LDA   __local_match_dictionary_word__entry_0
   SEC
   SBC   z_header_addr
   STA   __local_match_dictionary_word__0
   LDA   __local_match_dictionary_word__entry_1
   SBC   z_header_addr+1
   TAX
   LDA   __local_match_dictionary_word__0
   RTS
.if_end@10:
.if_end@8:
.if_end@6:
.if_end@4:
   LDA   __local_match_dictionary_word__entry_0
   CLC
   ADC   __local_match_dictionary_word__3
   STA   __local_match_dictionary_word__entry_0
   BCC   .carry_inc@6
   INC   __local_match_dictionary_word__entry_1
.carry_inc@6:
   LDA   __local_match_dictionary_word__entry_index_0
   SEC
   SBC   #$01
   STA   __local_match_dictionary_word__entry_index_0
   BCS   .carry_inc@7
   DEC   __local_match_dictionary_word__entry_index_1
.carry_inc@7:
   ORA   __local_match_dictionary_word__entry_index_1
   BNE   .loop@1_start
.tail@0:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #<story
   STA   z_header_addr
   LDA   #>story
   STA   z_header_addr+1
   LDA   #$00
   STA   story+8
   LDA   #$40
   STA   story+9
   LDA   #$03
   STA   story+64
   LDA   #$2E
   STA   story+65
   LDA   #$2C
   STA   story+66
   LDA   #$22
   STA   story+67
   LDA   #$07
   STA   story+68
   LDA   #$00
   STA   story+69
   LDA   #$14
   STA   story+70
   LDA   #$00
   STA   __local_main__i
.loop@2_start:
   CMP   #$14
   BCS   .loop@2_break
   CLC
   ADC   #$01
   STA   __local_main__4
   ASL
   ASL
   CLC
   ADC   __local_main__4
   STA   __local_main__3
   LDA   __local_main__i
   ASL
   STA   __local_main__1
   LDA   #$00
   STA   __local_main__2
   ROL   __local_main__2
   ASL   __local_main__1
   ROL   __local_main__2
   ASL   __local_main__1
   ROL   __local_main__2
   LDA   __local_main__1
   SEC
   SBC   __local_main__i
   BCS   .carry_inc@8
   DEC   __local_main__2
.carry_inc@8:
   CLC
   ADC   #<story
   STA   __local_main__0
   LDA   __local_main__2
   ADC   #>story
   STA   __local_main__i
   LDA   __local_main__3
   LDY   #$4A
   STA   (__local_main__0),Y
   LDA   __local_main__4
   STA   __local_main__i
   JMP   .loop@2_start
.loop@2_break:
   LDA   #$00
   STA   zchar_scratch2
   STA   zchar_scratch2+1
   LDA   #$55
   STA   zchar_scratch2+2
   LDA   #$00
   STA   zchar_scratch2+3
   JSR   match_dictionary_word
   RTS

story:
   DS.B  512

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z_header_addr
   STA   z_header_addr+1
   STA   zchar_scratch2
   STA   zchar_scratch2+1
   STA   zchar_scratch2+2
   STA   zchar_scratch2+3
   STA   zchar_scratch2+4
   STA   zchar_scratch2+5
   RTS
