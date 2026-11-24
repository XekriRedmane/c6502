SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_target__0	EQU	$0200
__local_target__2	EQU	$0202
__local_target__3	EQU	$0203
__local_target__4	EQU	$0204
__local_target__nested	EQU	$0205

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals=__local_target__0,__local_target__1,__local_target__2,__local_target__3,__local_target__4,__local_target__nested,__local_target__6,__local_target__7,__local_target__8,__local_target__9,__local_target__10,__local_target__11,__local_target__12,__local_target__13,__local_target__14,__local_target__15,__local_target__16,__local_target__17,__local_target__18,__local_target__19,__local_target__20,__local_target__21,__local_target__22,__local_target__23,__local_target__24,__local_target__25,__local_target__26,__local_target__27,__local_target__28,__local_target__29,__local_target__30,__local_target__31,__local_target__32,__local_target__33,__local_target__34,__local_target__35,__local_target__36,__local_target__37,__local_target__38,__local_target__39,__local_target__40,__local_target__41,__local_target__42,__local_target__43,__local_target__44,__local_target__45,__local_target__46,__local_target__47,__local_target__48,__local_target__49,__local_target__50,__local_target__51,__local_target__52,__local_target__53,__local_target__54,__local_target__55,__local_target__56,__local_target__57,__local_target__58,__local_target__59,__local_target__60,__local_target__61,__local_target__62,__local_target__63,__local_target__64,__local_target__65,__local_target__66,__local_target__67,__local_target__68,__local_target__69,__local_target__70,__local_target__71,__local_target__72,__local_target__73,__local_target__74,__local_target__75,__local_target__76,__local_target__77,__local_target__78,__local_target__79,__local_target__80,__local_target__81,__local_target__82,__local_target__83,__local_target__84,__local_target__85,__local_target__86,__local_target__87,__local_target__88,__local_target__89,__local_target__90,__local_target__91,__local_target__92,__local_target__93,__local_target__94,__local_target__95,__local_target__96,__local_target__97,__local_target__98,__local_target__99,__local_target__100,__local_target__101,__local_target__102,__local_target__103,__local_target__104,__local_target__105,__local_target__106,__local_target__107,__local_target__108,__local_target__109,__local_target__110,__local_target__111,__local_target__112,__local_target__113,__local_target__114,__local_target__115,__local_target__116,__local_target__117,__local_target__118,__local_target__119,__local_target__120,__local_target__121,__local_target__122,__local_target__123,__local_target__124,__local_target__125,__local_target__126,__local_target__127,__local_target__128,__local_target__129,__local_target__130,__local_target__131,__local_target__132,__local_target__133,__local_target__134,__local_target__135,__local_target__136,__local_target__137,__local_target__138,__local_target__139,__local_target__140,__local_target__141,__local_target__142 indirect=false in_cycle=false
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   #$2A
   STA   __local_target__4
.clear@0:
   DEC   __local_target__4
   LDA   #<__local_target__nested
   STA   __local_target__2
   LDA   #>__local_target__nested
   STA   __local_target__3
   LDA   __local_target__4
   CLC
   ADC   __local_target__2
   STA   __local_target__0
   LDA   #$00
   ADC   __local_target__3
   STA   DPTR+1
   LDA   __local_target__0
   STA   DPTR
   LDA   #$00
   LDY   #$04
   STA   (DPTR),Y
   LDA   __local_target__4
   BNE   .clear@0
   LDA   #$5A
   STA   __local_target__4
.clear@1:
   DEC   __local_target__4
   LDA   #<__local_target__nested
   STA   __local_target__2
   LDA   #>__local_target__nested
   STA   __local_target__3
   LDA   __local_target__4
   CLC
   ADC   __local_target__2
   STA   __local_target__0
   LDA   #$00
   ADC   __local_target__3
   STA   DPTR+1
   LDA   __local_target__0
   STA   DPTR
   LDA   #$00
   LDY   #$30
   STA   (DPTR),Y
   LDA   __local_target__4
   BNE   .clear@1
   LDA   __local_target__nested+92
   LDX   __local_target__nested+93
   RTS

main:
   SUBROUTINE

   JMP   target
