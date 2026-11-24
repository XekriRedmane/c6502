SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 154 local bytes
   SEC
   LDA   SSP
   SBC   #$9C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   glob
   STA   HARGS
   LDA   glob+1
   STA   HARGS+1
   LDA   glob+2
   STA   HARGS+2
   LDA   glob+3
   STA   HARGS+3
   LDA   glob+4
   STA   HARGS+4
   LDA   glob+5
   STA   HARGS+5
   LDA   glob+6
   STA   HARGS+6
   LDA   glob+7
   STA   HARGS+7
   LDA   #$C2
   STA   HARGS+9
   LDA   #$EB
   STA   HARGS+10
   LDA   #$0B
   STA   HARGS+11
   LDA   #$00
   STA   HARGS+8
   STA   HARGS+12
   LDY   #$00
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
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
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
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
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$13
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
   LDA   glob
   SEC
   LDY   #$11
   SBC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   glob+1
   LDY   #$12
   SBC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   glob+2
   LDY   #$13
   SBC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   glob+3
   LDY   #$14
   SBC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   glob+4
   LDY   #$15
   SBC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   glob+5
   LDY   #$16
   SBC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   glob+6
   LDY   #$17
   SBC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   glob+7
   LDY   #$18
   SBC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$02
   CLC
   LDY   #$23
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   LDY   #$24
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$2D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$06
   SEC
   LDY   #$23
   SBC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   LDY   #$24
   SBC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$2B
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$35
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$39
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$04
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$3D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$41
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDA   #$10
   SEC
   LDY   #$33
   SBC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   LDY   #$34
   SBC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   CLC
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$1A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$23
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$15
   STA   (SSP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$16
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$17
   STA   (SSP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$18
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   check_12_ints
   LDY   #$51
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDA   #$08
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
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
   LDA   glob
   CLC
   LDY   #$53
   ADC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDA   glob+1
   LDY   #$54
   ADC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDA   glob+2
   LDY   #$55
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDA   glob+3
   LDY   #$56
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDA   glob+4
   LDY   #$57
   ADC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDA   glob+5
   LDY   #$58
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   glob+6
   LDY   #$59
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDA   glob+7
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDA   #$1C
   SEC
   LDY   #$65
   SBC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   LDY   #$66
   SBC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDA   #$04
   CLC
   LDY   #$65
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   LDY   #$66
   ADC   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDA   #$20
   SEC
   LDY   #$69
   SBC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   LDY   #$6A
   SBC   (FP),Y
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDA   #$23
   SEC
   LDY   #$71
   SBC   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   LDY   #$72
   SBC   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$7F
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$83
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$05
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$89
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDA   #$06
   CLC
   LDY   #$75
   ADC   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDA   #$00
   LDY   #$76
   ADC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$91
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   ADC   #$00
   LDY   #$92
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$1A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$65
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$15
   STA   (SSP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$16
   STA   (SSP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$17
   STA   (SSP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$18
   STA   (SSP),Y
   LDA   #$0D
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   check_12_ints
   LDY   #$95
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   CMP   #$3B
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   CMP   #$9A
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   CMP   #$CA
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$97
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   SEC
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9B
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@0:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9B
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS

check_12_ints:
   SUBROUTINE

   ; prologue: 26 arg bytes, 50 local bytes
   SEC
   LDA   SSP
   SBC   #$34
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$33
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   CLC
   LDY   #$03
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$35
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@1:
   LDY   #$4D
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$07
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$37
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@2:
   LDY   #$4D
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0B
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@13
   LDY   #$39
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@3:
   LDY   #$4D
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$0F
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@4:
   LDY   #$4D
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$13
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@5:
   LDY   #$4D
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$17
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@6:
   LDY   #$4D
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$1B
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$41
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@7:
   LDY   #$4D
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$1F
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$43
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@8:
   LDY   #$4D
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$23
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@31
   LDY   #$45
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@9:
   LDY   #$4D
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$27
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@34
   LDY   #$47
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@10
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@10:
   LDY   #$4D
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$2B
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDY   #$49
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@11:
   LDY   #$4D
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$2F
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@40
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@12
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@12:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS

glob:
   DC.L  $00000005
   DC.L  $00000000
