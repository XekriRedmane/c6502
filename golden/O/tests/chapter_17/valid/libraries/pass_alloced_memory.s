SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_fill_100_bytes__byte_0	EQU	$80
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_fill_100_bytes__byte_1	EQU	$81
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_memset__c_0	EQU	$82
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_memset__c_1	EQU	$83
__zpabi_calloc__size_0	EQU	$84
__zpabi_memset__n_0	EQU	$84
__zpabi_calloc__size_1	EQU	$85
__zpabi_memset__n_1	EQU	$85
__zpabi_calloc__size_2	EQU	$86
__zpabi_memset__n_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87
__zpabi_memset__n_3	EQU	$87

; @zp-link-meta-begin
; def fill_100_bytes params=__zpabi_fill_100_bytes__pointer_0,__zpabi_fill_100_bytes__pointer_1,__zpabi_fill_100_bytes__byte_0,__zpabi_fill_100_bytes__byte_1 locals=__local_fill_100_bytes__pointer indirect=false in_cycle=false param_regs=A,X,,
; def free_bytes params=__zpabi_free_bytes__ptr_0,__zpabi_free_bytes__ptr_1 locals=__local_free_bytes__ptr indirect=false in_cycle=false param_regs=A,X
; def get_100_zeroed_bytes params= locals=__local_get_100_zeroed_bytes__0 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext free params=__zpabi_free__ptr_0,__zpabi_free__ptr_1 param_regs=A,X
; ext memset params=__zpabi_memset__s_0,__zpabi_memset__s_1,__zpabi_memset__c_0,__zpabi_memset__c_1,__zpabi_memset__n_0,__zpabi_memset__n_1,__zpabi_memset__n_2,__zpabi_memset__n_3 param_regs=A,X,,,,,,
; call fill_100_bytes -> memset
; call free_bytes -> free
; call get_100_zeroed_bytes -> calloc
; item get_100_zeroed_bytes section=.text local=false zero=false
; item fill_100_bytes section=.text local=false zero=false
; item free_bytes section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

get_100_zeroed_bytes:
   SUBROUTINE

   LDA   #$64
   STA   __zpabi_calloc__nmemb_0
   LDA   #$01
   STA   __zpabi_calloc__size_0
   LDA   #$00
   STA   __zpabi_calloc__nmemb_1
   STA   __zpabi_calloc__nmemb_2
   STA   __zpabi_calloc__nmemb_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   JMP   calloc

fill_100_bytes:
   SUBROUTINE

   LDY   __zpabi_fill_100_bytes__byte_0
   STY   __zpabi_memset__c_0
   LDY   __zpabi_fill_100_bytes__byte_1
   STY   __zpabi_memset__c_1
   LDY   #$64
   STY   __zpabi_memset__n_0
   LDY   #$00
   STY   __zpabi_memset__n_1
   STY   __zpabi_memset__n_2
   STY   __zpabi_memset__n_3
   JMP   memset

free_bytes:
   SUBROUTINE

   JMP   free
