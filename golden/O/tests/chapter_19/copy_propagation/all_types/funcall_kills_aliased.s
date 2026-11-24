SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__d	EQU	$88
__local_kill_aliased__1	EQU	$91
__local_kill_aliased__2	EQU	$92
__local_kill_aliased__3	EQU	$93
__local_kill_aliased__4	EQU	$94
__local_kill_aliased__5	EQU	$95
__local_kill_aliased__6	EQU	$96
__local_kill_aliased__7	EQU	$97
__local_kill_aliased__d	EQU	$98

; @zp-link-meta-begin
; def kill_aliased params= locals=__local_kill_aliased__0,__local_kill_aliased__1,__local_kill_aliased__2,__local_kill_aliased__3,__local_kill_aliased__4,__local_kill_aliased__5,__local_kill_aliased__6,__local_kill_aliased__7,__local_kill_aliased__d,__local_kill_aliased__9,__local_kill_aliased__10,__local_kill_aliased__11,__local_kill_aliased__12,__local_kill_aliased__13,__local_kill_aliased__14,__local_kill_aliased__15 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__d,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15 indirect=false in_cycle=false
; def save_ptr params=__zpabi_save_ptr__to_save_0,__zpabi_save_ptr__to_save_1 locals=__local_save_ptr__to_save_1,__local_save_ptr__to_save_0 indirect=false in_cycle=false param_regs=A,X
; def target params= locals= indirect=false in_cycle=false
; def update_ptr params= locals= indirect=false in_cycle=false
; call kill_aliased -> save_ptr
; call kill_aliased -> update_ptr
; call main -> kill_aliased
; call main -> target
; call target -> update_ptr
; item save_ptr section=.text local=false zero=false
; item update_ptr section=.text local=false zero=false
; item update_ptr@run1 section=.text local=true zero=false
; item update_ptr@run0 section=.text local=true zero=false
; item target section=.text local=false zero=false
; item kill_aliased section=.text local=false zero=false
; item main section=.text local=false zero=false
; item globl_ptr section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

save_ptr:
   SUBROUTINE

   STA   globl_ptr
   STX   globl_ptr+1
   RTS

update_ptr:
   SUBROUTINE

   JSR   update_ptr@run1
   LDY   #$00
   STA   (DPTR),Y
   JSR   update_ptr@run1
   LDY   #$01
   STA   (DPTR),Y
   JSR   update_ptr@run1
   LDY   #$02
   STA   (DPTR),Y
   JSR   update_ptr@run1
   LDY   #$03
   STA   (DPTR),Y
   JSR   update_ptr@run1
   LDY   #$04
   STA   (DPTR),Y
   JSR   update_ptr@run1
   LDY   #$05
   STA   (DPTR),Y
   JSR   update_ptr@run0
   LDA   #$10
   LDY   #$06
   STA   (DPTR),Y
   JSR   update_ptr@run0
   LDA   #$40
   LDY   #$07
   STA   (DPTR),Y
   RTS

update_ptr@run1:
   SUBROUTINE

   JSR   update_ptr@run0
   LDA   #$00
   RTS

update_ptr@run0:
   SUBROUTINE

   LDA   globl_ptr
   STA   DPTR
   LDA   globl_ptr+1
   STA   DPTR+1
   RTS

target:
   SUBROUTINE

   JSR   update_ptr
   LDX   #$00
   LDA   #$0A
   RTS

kill_aliased:
   SUBROUTINE

   LDA   #$00
   STA   __local_kill_aliased__d
   STA   __local_kill_aliased__d+1
   STA   __local_kill_aliased__d+2
   STA   __local_kill_aliased__d+3
   STA   __local_kill_aliased__d+4
   STA   __local_kill_aliased__d+5
   LDA   #$F0
   STA   __local_kill_aliased__d+6
   LDA   #$3F
   STA   __local_kill_aliased__d+7
   LDX   #>__local_kill_aliased__d
   LDA   #<__local_kill_aliased__d
   JSR   save_ptr
   JSR   update_ptr@run0
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_kill_aliased__7
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__6
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__5
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__4
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__3
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__2
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__1
   INY
   LDA   (DPTR),Y
   CMP   #$3F
   BNE   .jcmp_skip@0
   LDA   __local_kill_aliased__1
   CMP   #$F0
   BNE   .jcmp_skip@0
   LDA   __local_kill_aliased__2
   BNE   .jcmp_skip@0
   LDA   __local_kill_aliased__3
   BNE   .jcmp_skip@0
   LDA   __local_kill_aliased__4
   BNE   .jcmp_skip@0
   LDA   __local_kill_aliased__5
   BNE   .jcmp_skip@0
   LDA   __local_kill_aliased__6
   BNE   .jcmp_skip@0
   LDA   __local_kill_aliased__7
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   TXA
   RTS
.if_end@0:
   LDA   #$00
   STA   __local_kill_aliased__d
   STA   __local_kill_aliased__d+1
   STA   __local_kill_aliased__d+2
   STA   __local_kill_aliased__d+3
   STA   __local_kill_aliased__d+4
   STA   __local_kill_aliased__d+5
   STA   __local_kill_aliased__d+6
   LDA   #$40
   STA   __local_kill_aliased__d+7
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_kill_aliased__7
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__6
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__5
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__4
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__3
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__2
   INY
   LDA   (DPTR),Y
   STA   __local_kill_aliased__1
   INY
   LDA   (DPTR),Y
   CMP   #$40
   BNE   .jcmp_skip@1
   LDA   __local_kill_aliased__1
   BNE   .jcmp_skip@1
   LDA   __local_kill_aliased__2
   BNE   .jcmp_skip@1
   LDA   __local_kill_aliased__3
   BNE   .jcmp_skip@1
   LDA   __local_kill_aliased__4
   BNE   .jcmp_skip@1
   LDA   __local_kill_aliased__5
   BNE   .jcmp_skip@1
   LDA   __local_kill_aliased__6
   BNE   .jcmp_skip@1
   LDA   __local_kill_aliased__7
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   TXA
   RTS
.if_end@1:
   JSR   update_ptr
   LDA   __local_kill_aliased__d
   STA   HARGS
   LDA   __local_kill_aliased__d+1
   STA   HARGS+1
   LDA   __local_kill_aliased__d+2
   STA   HARGS+2
   LDA   __local_kill_aliased__d+3
   STA   HARGS+3
   LDA   __local_kill_aliased__d+4
   STA   HARGS+4
   LDA   __local_kill_aliased__d+5
   STA   HARGS+5
   LDA   __local_kill_aliased__d+6
   STA   HARGS+6
   LDA   __local_kill_aliased__d+7
   STA   HARGS+7
   JSR   d2i
   LDA   HARGS+8
   LDX   HARGS+9
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__d
   STA   __local_main__d+1
   STA   __local_main__d+2
   STA   __local_main__d+3
   STA   __local_main__d+4
   STA   __local_main__d+5
   STA   __local_main__d+6
   STA   __local_main__d+7
   LDA   #<__local_main__d
   STA   globl_ptr
   LDA   #>__local_main__d
   STA   globl_ptr+1
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDA   __local_main__d+7
   CMP   #$40
   BNE   .jcmp_skip@3
   LDA   __local_main__d+6
   CMP   #$10
   BNE   .jcmp_skip@3
   LDA   __local_main__d+5
   BNE   .jcmp_skip@3
   LDA   __local_main__d+4
   BNE   .jcmp_skip@3
   LDA   __local_main__d+3
   BNE   .jcmp_skip@3
   LDA   __local_main__d+2
   BNE   .jcmp_skip@3
   LDA   __local_main__d+1
   BNE   .jcmp_skip@3
   LDA   __local_main__d
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   LDA   #$00
   STA   __local_main__d
   STA   __local_main__d+1
   STA   __local_main__d+2
   STA   __local_main__d+3
   STA   __local_main__d+4
   STA   __local_main__d+5
   STA   __local_main__d+6
   STA   __local_main__d+7
   JSR   kill_aliased
   STA   HARGS
   STX   HARGS+1
   JSR   i2d
   LDA   HARGS+9
   CMP   #$40
   BNE   .jcmp_skip@4
   LDA   HARGS+8
   CMP   #$10
   BNE   .jcmp_skip@4
   LDA   HARGS+7
   BNE   .jcmp_skip@4
   LDA   HARGS+6
   BNE   .jcmp_skip@4
   LDA   HARGS+5
   BNE   .jcmp_skip@4
   LDA   HARGS+4
   BNE   .jcmp_skip@4
   LDA   HARGS+3
   BNE   .jcmp_skip@4
   LDA   HARGS+2
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@4:
   LDA   __local_main__d
   ORA   __local_main__d+1
   ORA   __local_main__d+2
   ORA   __local_main__d+3
   ORA   __local_main__d+4
   ORA   __local_main__d+5
   ORA   __local_main__d+6
   ORA   __local_main__d+7
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$04
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS

globl_ptr:
   DS.B  2
