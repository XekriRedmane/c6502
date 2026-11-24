SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

target:
   SUBROUTINE

   ; prologue: 0 arg bytes, 246 local bytes
   SEC
   LDA   SSP
   SBC   #$F8
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$F7
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   SEC
   LDA   #$00
   SBC   #$64
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$64
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$C8
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$2C
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$90
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$0A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$53
   ADC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$54
   ADC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$55
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$56
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$57
   ADC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$58
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$59
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$14
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   LDY   #$65
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   SEC
   LDY   #$63
   SBC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$64
   SBC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$65
   SBC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$66
   SBC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$67
   SBC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$68
   SBC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$69
   SBC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$6A
   SBC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$1E
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   LDY   #$75
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDY   #$73
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$7B
   STA   (FP),Y
   LDA   HARGS+17
   INY
   STA   (FP),Y
   LDA   HARGS+18
   INY
   STA   (FP),Y
   LDA   HARGS+19
   INY
   STA   (FP),Y
   LDA   HARGS+20
   INY
   STA   (FP),Y
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$64
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   LDY   #$85
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDY   #$83
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+16
   LDY   #$8B
   STA   (FP),Y
   LDA   HARGS+17
   INY
   STA   (FP),Y
   LDA   HARGS+18
   INY
   STA   (FP),Y
   LDA   HARGS+19
   INY
   STA   (FP),Y
   LDA   HARGS+20
   INY
   STA   (FP),Y
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDA   #$06
   LDY   #$93
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   LDY   #$95
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   CLC
   LDY   #$93
   ADC   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$94
   ADC   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$95
   ADC   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$96
   ADC   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$97
   ADC   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$98
   ADC   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$99
   ADC   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$9A
   ADC   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDY   #$9B
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   LDY   #$A3
   STA   (FP),Y
   LDA   HARGS+25
   INY
   STA   (FP),Y
   LDA   HARGS+26
   INY
   STA   (FP),Y
   LDA   HARGS+27
   INY
   STA   (FP),Y
   LDA   HARGS+28
   INY
   STA   (FP),Y
   LDA   HARGS+29
   INY
   STA   (FP),Y
   LDA   HARGS+30
   INY
   STA   (FP),Y
   LDA   HARGS+31
   INY
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$5A
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$AF
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$B4
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$11
   LDA   (FP),Y
   LDY   #$B3
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$10
   LDA   (FP),Y
   LDY   #$B2
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$B1
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$B0
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$AF
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$AE
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$AD
   CMP   (FP),Y
   BEQ   .cmp_true@22
.thread@0:
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$B5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .and_false@6
.lb_skip@3:
   LDA   #$50
   LDY   #$B7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   LDY   #$B9
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$BE
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$21
   LDA   (FP),Y
   LDY   #$BD
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$20
   LDA   (FP),Y
   LDY   #$BC
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$BB
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$BA
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$B9
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$B8
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$B7
   CMP   (FP),Y
   BEQ   .cmp_true@27
.thread@1:
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$BF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@7
.and_false@6:
   LDA   #$00
   LDY   #$C1
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$C1
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .and_false@4
.lb_skip@2:
   LDA   #$70
   LDY   #$C3
   STA   (FP),Y
   LDA   #$17
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$C5
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$CA
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$31
   LDA   (FP),Y
   LDY   #$C9
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$30
   LDA   (FP),Y
   LDY   #$C8
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$C7
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$C6
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$C5
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$C4
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$C3
   CMP   (FP),Y
   BEQ   .cmp_true@32
.thread@2:
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$CB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$CD
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$CD
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .and_false@2
.lb_skip@1:
   LDA   #$09
   LDY   #$CF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   LDY   #$D1
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$D6
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$41
   LDA   (FP),Y
   LDY   #$D5
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$40
   LDA   (FP),Y
   LDY   #$D4
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$D3
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$D2
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$D1
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$D0
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$CF
   CMP   (FP),Y
   BEQ   .cmp_true@37
.thread@3:
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$D7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   LDY   #$D9
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$D9
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@0
.lb_skip@0:
   LDA   #$04
   LDY   #$DB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@41
   LDA   #$FF
.sx_done@41:
   LDY   #$DD
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$E2
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$51
   LDA   (FP),Y
   LDY   #$E1
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$50
   LDA   (FP),Y
   LDY   #$E0
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$DF
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$DE
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$DD
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$DC
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$DB
   CMP   (FP),Y
   BEQ   .cmp_true@42
.thread@4:
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$E3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$E5
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$E5
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@46
   LDA   #$FF
.sx_done@46:
   LDY   #$E9
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$E7
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$F8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$F7
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@8:
   LDA   #$01
   LDY   #$EF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@48
   LDA   #$FF
.sx_done@48:
   LDY   #$F1
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$EF
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$F8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 8 local bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   target
   LDA   HARGS+16
   LDY   #$01
   STA   (FP),Y
   LDA   HARGS+17
   INY
   STA   (FP),Y
   LDA   HARGS+18
   INY
   STA   (FP),Y
   LDA   HARGS+19
   INY
   STA   (FP),Y
   LDA   HARGS+20
   INY
   STA   (FP),Y
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
