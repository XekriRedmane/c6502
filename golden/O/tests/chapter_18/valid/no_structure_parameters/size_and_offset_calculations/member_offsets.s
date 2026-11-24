SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_malloc__size_0	EQU	$80
__zpabi_malloc__size_1	EQU	$81
__zpabi_malloc__size_2	EQU	$82
__zpabi_malloc__size_3	EQU	$83
__local_main__0	EQU	$84
__local_test_contains_struct_array_array__1	EQU	$87
__local_test_eightbytes__1	EQU	$87
__local_test_internal_padding__1	EQU	$87
__local_test_sixteen_bytes__1	EQU	$87
__local_test_three_bytes__1	EQU	$87
__local_test_wonky_array__1	EQU	$87
__local_test_contains_struct_array_array__2	EQU	$88
__local_test_eightbytes__2	EQU	$88
__local_test_internal_padding__2	EQU	$88
__local_test_sixteen_bytes__2	EQU	$88
__local_test_three_bytes__2	EQU	$88
__local_test_wonky_array__2	EQU	$88
__local_test_contains_struct_array_array__3	EQU	$89
__local_test_eightbytes__3	EQU	$89
__local_test_internal_padding__3	EQU	$89
__local_test_sixteen_bytes__3	EQU	$89
__local_test_three_bytes__3	EQU	$89
__local_test_wonky_array__3	EQU	$89
__local_test_eightbytes__4	EQU	$8A
__local_test_internal_padding__4	EQU	$8A
__local_test_wonky_array__4	EQU	$8A
__local_test_internal_padding__5	EQU	$8B
__local_test_eightbytes__6	EQU	$8C
__local_test_internal_padding__6	EQU	$8C
__local_test_wonky_array__6	EQU	$8C
__local_test_internal_padding__7	EQU	$8D
__local_test_eightbytes__8	EQU	$8E
__local_test_contains_struct_array_array__10	EQU	$90
__local_test_eightbytes__s	EQU	$91
__local_test_contains_struct_array_array__12	EQU	$92
__local_test_wonky_array__13	EQU	$93
__local_test_wonky_array__15	EQU	$95
__local_test_wonky_array__16	EQU	$96
__local_test_wonky_array__18	EQU	$98
__local_test_contains_struct_array_array__19	EQU	$99
__local_test_wonky_array__wonky_array	EQU	$9B
__local_test_contains_struct_array_array__arr	EQU	$9C

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_contains_struct_array_array params= locals=__local_test_contains_struct_array_array__0,__local_test_contains_struct_array_array__1,__local_test_contains_struct_array_array__2,__local_test_contains_struct_array_array__3,__local_test_contains_struct_array_array__4,__local_test_contains_struct_array_array__5,__local_test_contains_struct_array_array__6,__local_test_contains_struct_array_array__7,__local_test_contains_struct_array_array__8,__local_test_contains_struct_array_array__9,__local_test_contains_struct_array_array__10,__local_test_contains_struct_array_array__11,__local_test_contains_struct_array_array__12,__local_test_contains_struct_array_array__13,__local_test_contains_struct_array_array__14,__local_test_contains_struct_array_array__15,__local_test_contains_struct_array_array__16,__local_test_contains_struct_array_array__17,__local_test_contains_struct_array_array__18,__local_test_contains_struct_array_array__19,__local_test_contains_struct_array_array__20,__local_test_contains_struct_array_array__21,__local_test_contains_struct_array_array__arr,__local_test_contains_struct_array_array__23,__local_test_contains_struct_array_array__24,__local_test_contains_struct_array_array__25,__local_test_contains_struct_array_array__26,__local_test_contains_struct_array_array__27,__local_test_contains_struct_array_array__28,__local_test_contains_struct_array_array__29,__local_test_contains_struct_array_array__30,__local_test_contains_struct_array_array__31,__local_test_contains_struct_array_array__32,__local_test_contains_struct_array_array__33,__local_test_contains_struct_array_array__34,__local_test_contains_struct_array_array__35,__local_test_contains_struct_array_array__36,__local_test_contains_struct_array_array__37,__local_test_contains_struct_array_array__38,__local_test_contains_struct_array_array__39,__local_test_contains_struct_array_array__40,__local_test_contains_struct_array_array__41,__local_test_contains_struct_array_array__42,__local_test_contains_struct_array_array__43,__local_test_contains_struct_array_array__44,__local_test_contains_struct_array_array__45,__local_test_contains_struct_array_array__46,__local_test_contains_struct_array_array__47,__local_test_contains_struct_array_array__48,__local_test_contains_struct_array_array__49,__local_test_contains_struct_array_array__50,__local_test_contains_struct_array_array__51 indirect=false in_cycle=false
; def test_eightbytes params= locals=__local_test_eightbytes__0,__local_test_eightbytes__1,__local_test_eightbytes__2,__local_test_eightbytes__3,__local_test_eightbytes__4,__local_test_eightbytes__5,__local_test_eightbytes__6,__local_test_eightbytes__7,__local_test_eightbytes__8,__local_test_eightbytes__9,__local_test_eightbytes__10,__local_test_eightbytes__s,__local_test_eightbytes__12,__local_test_eightbytes__13 indirect=false in_cycle=false
; def test_internal_padding params= locals=__local_test_internal_padding__0,__local_test_internal_padding__1,__local_test_internal_padding__2,__local_test_internal_padding__3,__local_test_internal_padding__4,__local_test_internal_padding__5,__local_test_internal_padding__6,__local_test_internal_padding__7 indirect=false in_cycle=false
; def test_sixteen_bytes params= locals=__local_test_sixteen_bytes__0,__local_test_sixteen_bytes__1,__local_test_sixteen_bytes__2,__local_test_sixteen_bytes__3,__local_test_sixteen_bytes__4,__local_test_sixteen_bytes__5,__local_test_sixteen_bytes__6,__local_test_sixteen_bytes__7,__local_test_sixteen_bytes__8,__local_test_sixteen_bytes__9,__local_test_sixteen_bytes__10,__local_test_sixteen_bytes__11,__local_test_sixteen_bytes__12,__local_test_sixteen_bytes__13,__local_test_sixteen_bytes__14,__local_test_sixteen_bytes__15,__local_test_sixteen_bytes__16,__local_test_sixteen_bytes__17,__local_test_sixteen_bytes__18,__local_test_sixteen_bytes__19,__local_test_sixteen_bytes__20,__local_test_sixteen_bytes__21 indirect=false in_cycle=false
; def test_three_bytes params= locals=__local_test_three_bytes__0,__local_test_three_bytes__1,__local_test_three_bytes__2,__local_test_three_bytes__3,__local_test_three_bytes__4,__local_test_three_bytes__5,__local_test_three_bytes__6,__local_test_three_bytes__7,__local_test_three_bytes__8,__local_test_three_bytes__9,__local_test_three_bytes__10,__local_test_three_bytes__11,__local_test_three_bytes__12,__local_test_three_bytes__13 indirect=false in_cycle=false
; def test_wonky_array params= locals=__local_test_wonky_array__0,__local_test_wonky_array__1,__local_test_wonky_array__2,__local_test_wonky_array__3,__local_test_wonky_array__4,__local_test_wonky_array__5,__local_test_wonky_array__6,__local_test_wonky_array__7,__local_test_wonky_array__8,__local_test_wonky_array__9,__local_test_wonky_array__10,__local_test_wonky_array__11,__local_test_wonky_array__12,__local_test_wonky_array__13,__local_test_wonky_array__14,__local_test_wonky_array__15,__local_test_wonky_array__16,__local_test_wonky_array__17,__local_test_wonky_array__18,__local_test_wonky_array__19,__local_test_wonky_array__20,__local_test_wonky_array__wonky_array,__local_test_wonky_array__22,__local_test_wonky_array__23,__local_test_wonky_array__24,__local_test_wonky_array__25,__local_test_wonky_array__26,__local_test_wonky_array__27,__local_test_wonky_array__28,__local_test_wonky_array__29,__local_test_wonky_array__30,__local_test_wonky_array__31,__local_test_wonky_array__32,__local_test_wonky_array__33,__local_test_wonky_array__34,__local_test_wonky_array__35,__local_test_wonky_array__36,__local_test_wonky_array__37,__local_test_wonky_array__38,__local_test_wonky_array__39,__local_test_wonky_array__40,__local_test_wonky_array__41,__local_test_wonky_array__42,__local_test_wonky_array__43,__local_test_wonky_array__44,__local_test_wonky_array__45,__local_test_wonky_array__46,__local_test_wonky_array__47,__local_test_wonky_array__48,__local_test_wonky_array__49,__local_test_wonky_array__50,__local_test_wonky_array__51,__local_test_wonky_array__52,__local_test_wonky_array__53,__local_test_wonky_array__54,__local_test_wonky_array__55,__local_test_wonky_array__56,__local_test_wonky_array__57,__local_test_wonky_array__58,__local_test_wonky_array__59,__local_test_wonky_array__60,__local_test_wonky_array__61,__local_test_wonky_array__62,__local_test_wonky_array__63,__local_test_wonky_array__64,__local_test_wonky_array__65,__local_test_wonky_array__66,__local_test_wonky_array__67,__local_test_wonky_array__68,__local_test_wonky_array__69,__local_test_wonky_array__70,__local_test_wonky_array__71,__local_test_wonky_array__72,__local_test_wonky_array__73,__local_test_wonky_array__74,__local_test_wonky_array__75,__local_test_wonky_array__76,__local_test_wonky_array__77,__local_test_wonky_array__78,__local_test_wonky_array__79,__local_test_wonky_array__80,__local_test_wonky_array__81,__local_test_wonky_array__82,__local_test_wonky_array__83,__local_test_wonky_array__84,__local_test_wonky_array__85,__local_test_wonky_array__86,__local_test_wonky_array__87,__local_test_wonky_array__88,__local_test_wonky_array__89,__local_test_wonky_array__90,__local_test_wonky_array__91,__local_test_wonky_array__92,__local_test_wonky_array__93,__local_test_wonky_array__94,__local_test_wonky_array__95,__local_test_wonky_array__96,__local_test_wonky_array__97,__local_test_wonky_array__98,__local_test_wonky_array__99,__local_test_wonky_array__100,__local_test_wonky_array__101,__local_test_wonky_array__102,__local_test_wonky_array__103,__local_test_wonky_array__104,__local_test_wonky_array__105,__local_test_wonky_array__106,__local_test_wonky_array__107,__local_test_wonky_array__108,__local_test_wonky_array__109,__local_test_wonky_array__110,__local_test_wonky_array__111,__local_test_wonky_array__112,__local_test_wonky_array__113,__local_test_wonky_array__114,__local_test_wonky_array__115 indirect=false in_cycle=false
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; call main -> test_contains_struct_array_array
; call main -> test_eightbytes
; call main -> test_internal_padding
; call main -> test_sixteen_bytes
; call main -> test_three_bytes
; call main -> test_wonky_array
; call test_internal_padding -> malloc
; item test_eightbytes section=.text local=false zero=false
; item test_eightbytes@run0 section=.text local=true zero=false
; item test_eightbytes@tail1 section=.text local=true zero=false
; item test_eightbytes@tail0 section=.text local=true zero=false
; item test_internal_padding section=.text local=false zero=false
; item test_three_bytes section=.text local=false zero=false
; item test_sixteen_bytes section=.text local=false zero=false
; item test_wonky_array section=.text local=false zero=false
; item test_contains_struct_array_array section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @11.s section=.bss local=true zero=true
; item @20.s section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_eightbytes:
   SUBROUTINE

   LDA   #>__local_test_eightbytes__s
   STA   __local_test_eightbytes__8
   LDA   #<__local_test_eightbytes__s
   CLC
   ADC   #$02
   STA   __local_test_eightbytes__1
   BCC   .carry_inc@0
   INC   __local_test_eightbytes__8
.carry_inc@0:
   LDA   #>__local_test_eightbytes__s
   STA   __local_test_eightbytes__6
   LDA   #<__local_test_eightbytes__s
   CLC
   ADC   #$03
   STA   __local_test_eightbytes__4
   BCC   .carry_inc@1
   INC   __local_test_eightbytes__6
.carry_inc@1:
   LDA   #<__local_test_eightbytes__s
   AND   #$03
   BNE   .jcmp_skip@2
   LDA   #>__local_test_eightbytes__s
   CMP   #>__local_test_eightbytes__s
   BNE   .jcmp_skip@2
   LDA   #<__local_test_eightbytes__s
   CMP   #<__local_test_eightbytes__s
   BNE   .jcmp_skip@2
   LDA   __local_test_eightbytes__1
   SEC
   SBC   #<__local_test_eightbytes__s
   STA   __local_test_eightbytes__3
   LDA   __local_test_eightbytes__8
   SBC   #>__local_test_eightbytes__s
   STA   __local_test_eightbytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_eightbytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@2
   LDA   __local_test_eightbytes__1
   BNE   .jcmp_skip@2
   LDA   __local_test_eightbytes__2
   BNE   .jcmp_skip@2
   LDA   __local_test_eightbytes__3
   CMP   #$04
   BNE   .jcmp_skip@2
   LDA   __local_test_eightbytes__4
   SEC
   SBC   #<__local_test_eightbytes__s
   STA   __local_test_eightbytes__3
   LDA   __local_test_eightbytes__6
   SBC   #>__local_test_eightbytes__s
   STA   __local_test_eightbytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_eightbytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@2
   LDA   __local_test_eightbytes__1
   BNE   .jcmp_skip@2
   LDA   __local_test_eightbytes__2
   BNE   .jcmp_skip@2
   LDA   __local_test_eightbytes__3
   CMP   #$08
   BEQ   .if_end@3
.jcmp_skip@2:
   JMP   test_eightbytes@tail1
.if_end@3:
   JMP   test_eightbytes@tail0

test_eightbytes@run0:
   SUBROUTINE

   LDA   #$00
   SBC   #$00
   RTS

test_eightbytes@tail1:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_eightbytes@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_internal_padding:
   SUBROUTINE

   LDA   #$09
   STA   __zpabi_malloc__size_0
   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JSR   malloc
   STA   __local_test_internal_padding__7
   STX   __local_test_internal_padding__6
   CLC
   ADC   #$01
   STA   __local_test_internal_padding__2
   TXA
   ADC   #$00
   STA   __local_test_internal_padding__1
   LDA   __local_test_internal_padding__7
   CLC
   ADC   #$09
   STA   __local_test_internal_padding__5
   TXA
   ADC   #$00
   STA   __local_test_internal_padding__4
   LDA   __local_test_internal_padding__7
   AND   #$07
   BNE   .jcmp_skip@5
   CPX   __local_test_internal_padding__6
   BNE   .jcmp_skip@5
   LDA   __local_test_internal_padding__7
   CMP   __local_test_internal_padding__7
   BNE   .jcmp_skip@5
   LDA   __local_test_internal_padding__2
   SEC
   SBC   __local_test_internal_padding__7
   STA   __local_test_internal_padding__3
   LDA   __local_test_internal_padding__1
   SBC   __local_test_internal_padding__6
   STA   __local_test_internal_padding__2
   JSR   test_eightbytes@run0
   STA   __local_test_internal_padding__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@5
   LDA   __local_test_internal_padding__1
   BNE   .jcmp_skip@5
   LDA   __local_test_internal_padding__2
   BNE   .jcmp_skip@5
   LDA   __local_test_internal_padding__3
   CMP   #$08
   BNE   .jcmp_skip@5
   LDA   __local_test_internal_padding__5
   SEC
   SBC   __local_test_internal_padding__7
   STA   __local_test_internal_padding__3
   LDA   __local_test_internal_padding__4
   SBC   __local_test_internal_padding__6
   STA   __local_test_internal_padding__2
   JSR   test_eightbytes@run0
   STA   __local_test_internal_padding__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@5
   LDA   __local_test_internal_padding__1
   BNE   .jcmp_skip@5
   LDA   __local_test_internal_padding__2
   BNE   .jcmp_skip@5
   LDA   __local_test_internal_padding__3
   CMP   #$10
   BEQ   .if_end@7
.jcmp_skip@5:
   JMP   test_eightbytes@tail1
.if_end@7:
   JMP   test_eightbytes@tail0

test_three_bytes:
   SUBROUTINE

   LDA   #>@11.s
   CMP   #>@11.s
   BNE   .jcmp_skip@6
   LDA   #<@11.s
   CMP   #<@11.s
   BEQ   .if_end@8
.jcmp_skip@6:
   JMP   test_eightbytes@tail1
.if_end@8:
   LDA   #>@11.s
   CMP   #>@11.s
   BNE   .jcmp_skip@7
   LDA   #<@11.s
   CMP   #<@11.s
   BEQ   .if_end@9
.jcmp_skip@7:
   LDX   #$00
   TXA
   RTS
.if_end@9:
   LDA   #<(@11.s+1)
   SEC
   SBC   #<@11.s
   STA   __local_test_three_bytes__3
   LDA   #>(@11.s+1)
   SBC   #>@11.s
   STA   __local_test_three_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_three_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@8
   LDA   __local_test_three_bytes__1
   BNE   .jcmp_skip@8
   LDA   __local_test_three_bytes__2
   BNE   .jcmp_skip@8
   LDA   __local_test_three_bytes__3
   CMP   #$01
   BEQ   .if_end@10
.jcmp_skip@8:
   LDX   #$00
   TXA
   RTS
.if_end@10:
   LDA   #>(@11.s+1)
   CMP   #>(@11.s+1)
   BNE   .jcmp_skip@9
   LDA   #<(@11.s+1)
   CMP   #<(@11.s+1)
   BEQ   .if_end@11
.jcmp_skip@9:
   LDX   #$00
   TXA
   RTS
.if_end@11:
   LDA   #<(@11.s+2)
   SEC
   SBC   #<@11.s
   STA   __local_test_three_bytes__3
   LDA   #>(@11.s+2)
   SBC   #>@11.s
   STA   __local_test_three_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_three_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@12
   LDA   __local_test_three_bytes__1
   BNE   .jcmp_skip@12
   LDA   __local_test_three_bytes__2
   BNE   .jcmp_skip@12
   LDA   __local_test_three_bytes__3
   CMP   #$02
   BNE   .jcmp_skip@12
   LDA   #<(@11.s+3)
   SEC
   SBC   #<@11.s
   STA   __local_test_three_bytes__3
   LDA   #>(@11.s+3)
   SBC   #>@11.s
   STA   __local_test_three_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_three_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@12
   LDA   __local_test_three_bytes__1
   BNE   .jcmp_skip@12
   LDA   __local_test_three_bytes__2
   BNE   .jcmp_skip@12
   LDA   __local_test_three_bytes__3
   CMP   #$03
   BNE   .jcmp_skip@12
   LDA   #<(@11.s+3)
   SEC
   SBC   #<@11.s
   STA   __local_test_three_bytes__3
   LDA   #>(@11.s+3)
   SBC   #>@11.s
   STA   __local_test_three_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_three_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@12
   LDA   __local_test_three_bytes__1
   BNE   .jcmp_skip@12
   LDA   __local_test_three_bytes__2
   BNE   .jcmp_skip@12
   LDA   __local_test_three_bytes__3
   CMP   #$03
   BEQ   .if_end@14
.jcmp_skip@12:
   LDX   #$00
   TXA
   RTS
.if_end@14:
   JMP   test_eightbytes@tail0

test_sixteen_bytes:
   SUBROUTINE

   LDA   #<@20.s
   AND   #$03
   BEQ   .if_end@15
   JMP   test_eightbytes@tail1
.if_end@15:
   LDA   #>@20.s
   CMP   #>@20.s
   BNE   .jcmp_skip@13
   LDA   #<@20.s
   CMP   #<@20.s
   BEQ   .if_end@16
.jcmp_skip@13:
   LDX   #$00
   TXA
   RTS
.if_end@16:
   LDA   #>@20.s
   CMP   #>@20.s
   BNE   .jcmp_skip@14
   LDA   #<@20.s
   CMP   #<@20.s
   BEQ   .if_end@17
.jcmp_skip@14:
   LDX   #$00
   TXA
   RTS
.if_end@17:
   LDA   #<(@20.s+2)
   SEC
   SBC   #<@20.s
   STA   __local_test_sixteen_bytes__3
   LDA   #>(@20.s+2)
   SBC   #>@20.s
   STA   __local_test_sixteen_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_sixteen_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@15
   LDA   __local_test_sixteen_bytes__1
   BNE   .jcmp_skip@15
   LDA   __local_test_sixteen_bytes__2
   BNE   .jcmp_skip@15
   LDA   __local_test_sixteen_bytes__3
   CMP   #$04
   BEQ   .if_end@18
.jcmp_skip@15:
   LDX   #$00
   TXA
   RTS
.if_end@18:
   LDA   #<(@20.s+3)
   SEC
   SBC   #<@20.s
   STA   __local_test_sixteen_bytes__3
   LDA   #>(@20.s+3)
   SBC   #>@20.s
   STA   __local_test_sixteen_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_sixteen_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@16
   LDA   __local_test_sixteen_bytes__1
   BNE   .jcmp_skip@16
   LDA   __local_test_sixteen_bytes__2
   BNE   .jcmp_skip@16
   LDA   __local_test_sixteen_bytes__3
   CMP   #$08
   BEQ   .if_end@20
.jcmp_skip@16:
   LDX   #$00
   TXA
   RTS
.if_end@20:
   LDA   #<(@20.s+3)
   SEC
   SBC   #<@20.s
   STA   __local_test_sixteen_bytes__3
   LDA   #>(@20.s+3)
   SBC   #>@20.s
   STA   __local_test_sixteen_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_sixteen_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@17
   LDA   __local_test_sixteen_bytes__1
   BNE   .jcmp_skip@17
   LDA   __local_test_sixteen_bytes__2
   BNE   .jcmp_skip@17
   LDA   __local_test_sixteen_bytes__3
   CMP   #$08
   BEQ   .if_end@21
.jcmp_skip@17:
   LDX   #$00
   TXA
   RTS
.if_end@21:
   LDA   #<(@20.s+4)
   SEC
   SBC   #<@20.s
   STA   __local_test_sixteen_bytes__3
   LDA   #>(@20.s+4)
   SBC   #>@20.s
   STA   __local_test_sixteen_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_sixteen_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@18
   LDA   __local_test_sixteen_bytes__1
   BNE   .jcmp_skip@18
   LDA   __local_test_sixteen_bytes__2
   BNE   .jcmp_skip@18
   LDA   __local_test_sixteen_bytes__3
   CMP   #$09
   BEQ   .if_end@22
.jcmp_skip@18:
   LDX   #$00
   TXA
   RTS
.if_end@22:
   LDA   #<(@20.s+5)
   SEC
   SBC   #<@20.s
   STA   __local_test_sixteen_bytes__3
   LDA   #>(@20.s+5)
   SBC   #>@20.s
   STA   __local_test_sixteen_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_sixteen_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@19
   LDA   __local_test_sixteen_bytes__1
   BNE   .jcmp_skip@19
   LDA   __local_test_sixteen_bytes__2
   BNE   .jcmp_skip@19
   LDA   __local_test_sixteen_bytes__3
   CMP   #$0A
   BEQ   .if_end@23
.jcmp_skip@19:
   LDX   #$00
   TXA
   RTS
.if_end@23:
   LDA   #>(@20.s+5)
   CMP   #>(@20.s+5)
   BNE   .jcmp_skip@20
   LDA   #<(@20.s+5)
   CMP   #<(@20.s+5)
   BEQ   .if_end@24
.jcmp_skip@20:
   LDX   #$00
   TXA
   RTS
.if_end@24:
   LDA   #<(@20.s+5)
   SEC
   SBC   #<@20.s
   STA   __local_test_sixteen_bytes__3
   LDA   #>(@20.s+5)
   SBC   #>@20.s
   STA   __local_test_sixteen_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_sixteen_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@21
   LDA   __local_test_sixteen_bytes__1
   BNE   .jcmp_skip@21
   LDA   __local_test_sixteen_bytes__2
   BNE   .jcmp_skip@21
   LDA   __local_test_sixteen_bytes__3
   CMP   #$0A
   BEQ   .if_end@25
.jcmp_skip@21:
   LDX   #$00
   TXA
   RTS
.if_end@25:
   LDA   #<(@20.s+8)
   SEC
   SBC   #<@20.s
   STA   __local_test_sixteen_bytes__3
   LDA   #>(@20.s+8)
   SBC   #>@20.s
   STA   __local_test_sixteen_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_sixteen_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@22
   LDA   __local_test_sixteen_bytes__1
   BNE   .jcmp_skip@22
   LDA   __local_test_sixteen_bytes__2
   BNE   .jcmp_skip@28
   LDA   __local_test_sixteen_bytes__3
   CMP   #$0D
   BEQ   .if_end@26
.jcmp_skip@22:
   LDX   #$00
   TXA
   RTS
.if_end@26:
   LDA   #<(@20.s+8)
   SEC
   SBC   #<@20.s
   STA   __local_test_sixteen_bytes__3
   LDA   #>(@20.s+8)
   SBC   #>@20.s
   STA   __local_test_sixteen_bytes__2
   JSR   test_eightbytes@run0
   STA   __local_test_sixteen_bytes__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@28
   LDA   __local_test_sixteen_bytes__1
   BNE   .jcmp_skip@28
   LDA   __local_test_sixteen_bytes__2
   BNE   .jcmp_skip@28
   LDA   __local_test_sixteen_bytes__3
   CMP   #$10
   BNE   .jcmp_skip@28
   LDA   #>@20.s
   CMP   #>@20.s
   BNE   .jcmp_skip@28
   LDA   #<@20.s
   CMP   #<@20.s
   BNE   .jcmp_skip@28
   LDA   #>(@20.s+2)
   CMP   #>(@20.s+2)
   BNE   .jcmp_skip@28
   LDA   #<(@20.s+2)
   CMP   #<(@20.s+2)
   BNE   .jcmp_skip@28
   LDA   #>(@20.s+3)
   CMP   #>(@20.s+3)
   BNE   .jcmp_skip@28
   LDA   #<(@20.s+3)
   CMP   #<(@20.s+3)
   BNE   .jcmp_skip@28
   LDA   #>(@20.s+4)
   CMP   #>(@20.s+4)
   BNE   .jcmp_skip@28
   LDA   #<(@20.s+4)
   CMP   #<(@20.s+4)
   BNE   .jcmp_skip@28
   LDA   #>(@20.s+5)
   CMP   #>(@20.s+5)
   BNE   .jcmp_skip@28
   LDA   #<(@20.s+5)
   CMP   #<(@20.s+5)
   BEQ   .if_end@32
.jcmp_skip@28:
   LDX   #$00
   TXA
   RTS
.if_end@32:
   JMP   test_eightbytes@tail0

test_wonky_array:
   SUBROUTINE

   LDA   #>__local_test_wonky_array__wonky_array
   STA   __local_test_wonky_array__18
   LDA   #<__local_test_wonky_array__wonky_array
   CLC
   ADC   #$39
   STA   __local_test_wonky_array__16
   BCC   .carry_inc@2
   INC   __local_test_wonky_array__18
.carry_inc@2:
   LDA   #>__local_test_wonky_array__wonky_array
   STA   __local_test_wonky_array__15
   LDA   #<__local_test_wonky_array__wonky_array
   CLC
   ADC   #$39
   STA   __local_test_wonky_array__13
   BCC   .carry_inc@3
   INC   __local_test_wonky_array__15
.carry_inc@3:
   LDA   #>__local_test_wonky_array__wonky_array
   STA   __local_test_wonky_array__6
   LDA   #<__local_test_wonky_array__wonky_array
   CLC
   ADC   #$5F
   STA   __local_test_wonky_array__4
   BCC   .carry_inc@4
   INC   __local_test_wonky_array__6
.carry_inc@4:
   LDA   __local_test_wonky_array__16
   SEC
   SBC   #<__local_test_wonky_array__wonky_array
   STA   __local_test_wonky_array__3
   LDA   __local_test_wonky_array__18
   SBC   #>__local_test_wonky_array__wonky_array
   STA   __local_test_wonky_array__2
   JSR   test_eightbytes@run0
   STA   __local_test_wonky_array__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@33
   LDA   __local_test_wonky_array__1
   BNE   .jcmp_skip@33
   LDA   __local_test_wonky_array__2
   BNE   .jcmp_skip@33
   LDA   __local_test_wonky_array__3
   CMP   #$39
   BNE   .jcmp_skip@33
   LDA   __local_test_wonky_array__15
   CMP   __local_test_wonky_array__18
   BNE   .jcmp_skip@33
   LDA   __local_test_wonky_array__13
   CMP   __local_test_wonky_array__16
   BNE   .jcmp_skip@33
   LDA   #<(__local_test_wonky_array__wonky_array+40)
   SEC
   SBC   #<__local_test_wonky_array__wonky_array
   STA   __local_test_wonky_array__3
   LDA   #>(__local_test_wonky_array__wonky_array+40)
   SBC   #>__local_test_wonky_array__wonky_array
   STA   __local_test_wonky_array__2
   JSR   test_eightbytes@run0
   STA   __local_test_wonky_array__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@33
   LDA   __local_test_wonky_array__1
   BNE   .jcmp_skip@33
   LDA   __local_test_wonky_array__2
   BNE   .jcmp_skip@33
   LDA   __local_test_wonky_array__3
   CMP   #$28
   BNE   .jcmp_skip@33
   LDA   #>(__local_test_wonky_array__wonky_array+57)
   CMP   __local_test_wonky_array__18
   BNE   .jcmp_skip@33
   LDA   #<(__local_test_wonky_array__wonky_array+57)
   CMP   __local_test_wonky_array__16
   BNE   .jcmp_skip@33
   LDA   #>(__local_test_wonky_array__wonky_array+95)
   CMP   __local_test_wonky_array__6
   BNE   .jcmp_skip@33
   LDA   #<(__local_test_wonky_array__wonky_array+95)
   CMP   __local_test_wonky_array__4
   BEQ   .if_end@37
.jcmp_skip@33:
   JMP   test_eightbytes@tail1
.if_end@37:
   JMP   test_eightbytes@tail0

test_contains_struct_array_array:
   SUBROUTINE

   LDA   #>__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__19
   CLC
   BCC   .carry_inc@5
   INC   __local_test_contains_struct_array_array__19
.carry_inc@5:
   LDA   #>__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__12
   LDA   #<__local_test_contains_struct_array_array__arr
   CLC
   ADC   #$0A
   STA   __local_test_contains_struct_array_array__10
   BCC   .carry_inc@6
   INC   __local_test_contains_struct_array_array__12
.carry_inc@6:
   LDA   #<__local_test_contains_struct_array_array__arr
   AND   #$03
   BEQ   .if_end@38
   JMP   test_eightbytes@tail1
.if_end@38:
   LDA   __local_test_contains_struct_array_array__19
   CMP   #>__local_test_contains_struct_array_array__arr
   BNE   .jcmp_skip@34
   LDA   #<__local_test_contains_struct_array_array__arr
   CMP   #<__local_test_contains_struct_array_array__arr
   BEQ   .if_end@39
.jcmp_skip@34:
   LDX   #$00
   TXA
   RTS
.if_end@39:
   LDA   #<(__local_test_contains_struct_array_array__arr+1)
   SEC
   SBC   #<__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__3
   LDA   #>(__local_test_contains_struct_array_array__arr+1)
   SBC   #>__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__2
   JSR   test_eightbytes@run0
   STA   __local_test_contains_struct_array_array__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@35
   LDA   __local_test_contains_struct_array_array__1
   BNE   .jcmp_skip@35
   LDA   __local_test_contains_struct_array_array__2
   BNE   .jcmp_skip@35
   LDA   __local_test_contains_struct_array_array__3
   CMP   #$04
   BEQ   .if_end@40
.jcmp_skip@35:
   LDX   #$00
   TXA
   RTS
.if_end@40:
   LDA   #<(__local_test_contains_struct_array_array__arr+3)
   SEC
   SBC   #<__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__3
   LDA   #>(__local_test_contains_struct_array_array__arr+3)
   SBC   #>__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__2
   JSR   test_eightbytes@run0
   STA   __local_test_contains_struct_array_array__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@36
   LDA   __local_test_contains_struct_array_array__1
   BNE   .jcmp_skip@36
   LDA   __local_test_contains_struct_array_array__2
   BNE   .jcmp_skip@36
   LDA   __local_test_contains_struct_array_array__3
   CMP   #$08
   BEQ   .if_end@41
.jcmp_skip@36:
   LDX   #$00
   TXA
   RTS
.if_end@41:
   LDA   #>(__local_test_contains_struct_array_array__arr+10)
   CMP   __local_test_contains_struct_array_array__12
   BNE   .jcmp_skip@37
   LDA   #<(__local_test_contains_struct_array_array__arr+10)
   CMP   __local_test_contains_struct_array_array__10
   BEQ   .if_end@42
.jcmp_skip@37:
   LDX   #$00
   TXA
   RTS
.if_end@42:
   LDA   #<(__local_test_contains_struct_array_array__arr+11)
   SEC
   SBC   #<__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__3
   LDA   #>(__local_test_contains_struct_array_array__arr+11)
   SBC   #>__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__2
   JSR   test_eightbytes@run0
   STA   __local_test_contains_struct_array_array__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@38
   LDA   __local_test_contains_struct_array_array__1
   BNE   .jcmp_skip@38
   LDA   __local_test_contains_struct_array_array__2
   BNE   .jcmp_skip@38
   LDA   __local_test_contains_struct_array_array__3
   CMP   #$20
   BEQ   .if_end@43
.jcmp_skip@38:
   LDX   #$00
   TXA
   RTS
.if_end@43:
   LDA   #<(__local_test_contains_struct_array_array__arr+11)
   SEC
   SBC   __local_test_contains_struct_array_array__10
   STA   __local_test_contains_struct_array_array__3
   LDA   #>(__local_test_contains_struct_array_array__arr+11)
   SBC   __local_test_contains_struct_array_array__12
   STA   __local_test_contains_struct_array_array__2
   JSR   test_eightbytes@run0
   STA   __local_test_contains_struct_array_array__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@41
   LDA   __local_test_contains_struct_array_array__1
   BNE   .jcmp_skip@41
   LDA   __local_test_contains_struct_array_array__2
   BNE   .jcmp_skip@41
   LDA   __local_test_contains_struct_array_array__3
   CMP   #$04
   BNE   .jcmp_skip@41
   LDA   #<(__local_test_contains_struct_array_array__arr+14)
   SEC
   SBC   #<__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__3
   LDA   #>(__local_test_contains_struct_array_array__arr+14)
   SBC   #>__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__2
   JSR   test_eightbytes@run0
   STA   __local_test_contains_struct_array_array__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@41
   LDA   __local_test_contains_struct_array_array__1
   BNE   .jcmp_skip@41
   LDA   __local_test_contains_struct_array_array__2
   BNE   .jcmp_skip@41
   LDA   __local_test_contains_struct_array_array__3
   CMP   #$28
   BNE   .jcmp_skip@41
   LDA   #<(__local_test_contains_struct_array_array__arr+23)
   SEC
   SBC   #<__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__3
   LDA   #>(__local_test_contains_struct_array_array__arr+23)
   SBC   #>__local_test_contains_struct_array_array__arr
   STA   __local_test_contains_struct_array_array__2
   JSR   test_eightbytes@run0
   STA   __local_test_contains_struct_array_array__1
   JSR   test_eightbytes@run0
   BNE   .jcmp_skip@41
   LDA   __local_test_contains_struct_array_array__1
   BNE   .jcmp_skip@41
   LDA   __local_test_contains_struct_array_array__2
   BNE   .jcmp_skip@41
   LDA   __local_test_contains_struct_array_array__3
   CMP   #$40
   BEQ   .if_end@46
.jcmp_skip@41:
   LDX   #$00
   TXA
   RTS
.if_end@46:
   JMP   test_eightbytes@tail0

main:
   SUBROUTINE

   JSR   test_eightbytes
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@47
   JMP   test_eightbytes@tail0
.if_end@47:
   JSR   test_internal_padding
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@48
   LDX   #$00
   LDA   #$02
   RTS
.if_end@48:
   JSR   test_three_bytes
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@49
   LDX   #$00
   LDA   #$03
   RTS
.if_end@49:
   JSR   test_sixteen_bytes
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@50
   LDX   #$00
   LDA   #$04
   RTS
.if_end@50:
   JSR   test_wonky_array
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@51
   LDX   #$00
   LDA   #$05
   RTS
.if_end@51:
   JSR   test_contains_struct_array_array
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@52
   LDX   #$00
   LDA   #$06
   RTS
.if_end@52:
   JMP   test_eightbytes@tail1

@11.s:
   DS.B  3

@20.s:
   DS.B  8
