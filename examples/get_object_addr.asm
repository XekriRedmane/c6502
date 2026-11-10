__zpabi_get_object_addr__obj	EQU	$80
__local_main__0	EQU	$81
__local_main__1	EQU	$82
__local_get_object_addr__0	EQU	$83
__local_get_object_addr__1	EQU	$84
__local_get_object_addr__2	EQU	$85
__local_get_object_addr__3	EQU	$86
__local_get_object_addr__4	EQU	$87
z_header_addr	EQU	$BA

; @zp-link-meta-begin
; def get_object_addr params=__zpabi_get_object_addr__obj locals=__local_get_object_addr__0,__local_get_object_addr__1,__local_get_object_addr__2,__local_get_object_addr__3,__local_get_object_addr__4 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call main -> get_object_addr
; @zp-link-meta-end

get_object_addr:
   SUBROUTINE

   LDY   #$0A
   LDA   (z_header_addr),Y
   STA   __local_get_object_addr__0
   INY
   LDA   (z_header_addr),Y
   STA   __local_get_object_addr__1
   LDA   z_header_addr
   CLC
   ADC   __local_get_object_addr__1
   STA   __local_get_object_addr__1
   LDA   z_header_addr+1
   ADC   __local_get_object_addr__0
   STA   __local_get_object_addr__4
   LDA   __zpabi_get_object_addr__obj
   STA   __local_get_object_addr__2
   ASL   __local_get_object_addr__2
   LDA   #$00
   STA   __local_get_object_addr__3
   ROL   __local_get_object_addr__3
   ASL   __local_get_object_addr__2
   ROL   __local_get_object_addr__3
   ASL   __local_get_object_addr__2
   ROL   __local_get_object_addr__3
   LDA   __local_get_object_addr__2
   CLC
   ADC   __zpabi_get_object_addr__obj
   STA   __local_get_object_addr__0
   BCC   .carry_inc@0
   INC   __local_get_object_addr__3
.carry_inc@0:
   LDA   __local_get_object_addr__1
   CLC
   ADC   __local_get_object_addr__0
   STA   __local_get_object_addr__1
   LDA   __local_get_object_addr__4
   ADC   __local_get_object_addr__3
   STA   __local_get_object_addr__0
   LDA   __local_get_object_addr__1
   CLC
   ADC   #$35
   STA   HARGS
   LDA   __local_get_object_addr__0
   ADC   #$00
   STA   HARGS+1
   RTS

main:
   SUBROUTINE

   LDA   #<story
   STA   z_header_addr
   LDA   #>story
   STA   z_header_addr+1
   LDA   #<story
   STA   __local_main__0
   LDA   #>story
   STA   __local_main__1
   LDA   #$01
   LDY   #$0A
   STA   (__local_main__0),Y
   LDA   #<story
   STA   __local_main__0
   LDA   #>story
   STA   __local_main__1
   LDA   #<story
   STA   DPTR
   LDA   #>story
   STA   DPTR+1
   LDA   #$00
   INY
   STA   (__local_main__0),Y
   LDA   #$03
   STA   __zpabi_get_object_addr__obj
   JSR   get_object_addr
   LDA   HARGS
   SEC
   SBC   #<story
   STA   __local_main__0
   LDA   HARGS+1
   SBC   #>story
   TAX
   LDA   __local_main__0
   RTS

story:
   DS.B  512

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z_header_addr
   STA   z_header_addr+1
   RTS
