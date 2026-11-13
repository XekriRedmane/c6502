SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_find_index_of_page_table__page_0	EQU	$80
__zpabi_find_index_of_page_table__page_1	EQU	$81
__local_main__0	EQU	$82
__local_main__1	EQU	$83
__local_main__2	EQU	$84
__local_main__3	EQU	$85
num_page_table_entries	EQU	$BD
last_z_page	EQU	$BF

; @zp-link-meta-begin
; def find_index_of_page_table params=__zpabi_find_index_of_page_table__page_0,__zpabi_find_index_of_page_table__page_1 locals=__local_find_index_of_page_table__0,__local_find_index_of_page_table__count indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; call main -> find_index_of_page_table
; item find_index_of_page_table section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp num_page_table_entries size=1 at=$BD def=true init=$00
; zp last_z_page size=1 at=$BF def=true init=$00
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

find_index_of_page_table:
   SUBROUTINE

   LDX   num_page_table_entries
   LDY   #$00
   LDA   __zpabi_find_index_of_page_table__page_0
.loop@0_start:
   CMP   $2200,Y
   BNE   .if_end@2
   LDA   $2280,Y
   CMP   __zpabi_find_index_of_page_table__page_1
   BNE   .find_index_of_page_table@lia@1
   LDX   #$00
   TYA
   RTS
.find_index_of_page_table@lia@1:
   LDA   __zpabi_find_index_of_page_table__page_0
.if_end@2:
   INY
   DEX
   BNE   .loop@0_start
   LDX   #$01
   LDA   last_z_page
   RTS

main:
   SUBROUTINE

   LDA   #$03
   STA   num_page_table_entries
   LDA   #$02
   STA   last_z_page
   LDA   #$00
   STA   DPTR
   LDA   #$22
   STA   DPTR+1
   LDA   #$10
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   STA   DPTR
   LDA   #$22
   STA   DPTR+1
   LDA   #$00
   STA   (DPTR),Y
   LDA   #$01
   STA   DPTR
   LDA   #$22
   STA   DPTR+1
   LDA   #$20
   STA   (DPTR),Y
   LDA   #$81
   STA   DPTR
   LDA   #$22
   STA   DPTR+1
   LDA   #$01
   STA   (DPTR),Y
   LDA   #$02
   STA   DPTR
   LDA   #$22
   STA   DPTR+1
   LDA   #$30
   STA   (DPTR),Y
   LDA   #$82
   STA   DPTR
   LDA   #$22
   STA   DPTR+1
   LDA   #$00
   STA   (DPTR),Y
   LDA   #$20
   STA   __zpabi_find_index_of_page_table__page_0
   LDA   #$01
   STA   __zpabi_find_index_of_page_table__page_1
   JSR   find_index_of_page_table
   STA   __local_main__3
   STX   __local_main__2
   LDA   #$30
   STA   __zpabi_find_index_of_page_table__page_0
   LDA   #$01
   STA   __zpabi_find_index_of_page_table__page_1
   JSR   find_index_of_page_table
   STA   __local_main__0
   STX   __local_main__1
   LDA   __local_main__3
   CLC
   ADC   __local_main__0
   STA   __local_main__0
   LDA   __local_main__2
   ADC   __local_main__1
   TAX
   LDA   __local_main__0
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   num_page_table_entries
   STA   last_z_page
   RTS
