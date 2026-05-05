paint_hud_strip_p1:
   SUBROUTINE

.paint_hud_strip_p1@asm_ssa_preheader@0:
.paint_hud_strip_p1@ssa_block@0:
   LDA   #$00
   STA   $89
   LDA   #$0F
   STA   $88
.loop@0_start:
   LDA   $88
   BMI   .sx_neg@0
.paint_hud_strip_p1@asm_ssa_block@0:
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   $82
   LDA   $88
   SEC
   SBC   #$00
   LDA   $82
   SBC   #$00
   BVC   .jcmp_novf@2
.paint_hud_strip_p1@asm_ssa_block@1:
   EOR   #$80
.jcmp_novf@2:
   BPL   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
.paint_hud_strip_p1@asm_ssa_block@2:
   LDA   $89
   CLC
   ADC   #$01
   STA   $87
   LDA   $80
   CLC
   ADC   $89
   STA   DPTR
   LDA   $81
   ADC   #$00
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $85
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$24
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@3
.paint_hud_strip_p1@asm_ssa_block@3:
   LDA   #$00
   JMP   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$28
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@5
.paint_hud_strip_p1@asm_ssa_block@4:
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$2C
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@7
.paint_hud_strip_p1@asm_ssa_block@5:
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$30
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@9
.paint_hud_strip_p1@asm_ssa_block@6:
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$34
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@11
.paint_hud_strip_p1@asm_ssa_block@7:
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$38
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@13
.paint_hud_strip_p1@asm_ssa_block@8:
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$3C
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@15
.paint_hud_strip_p1@asm_ssa_block@9:
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   $87
   CLC
   ADC   #$01
   STA   $86
   LDA   $80
   CLC
   ADC   $87
   STA   DPTR
   LDA   $81
   ADC   #$00
   STA   DPTR+1
   LDA   (DPTR),Y
   STA   $85
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$20
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@17
.paint_hud_strip_p1@asm_ssa_block@10:
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   $86
   CLC
   ADC   #$01
   STA   $87
   LDA   $80
   CLC
   ADC   $86
   STA   DPTR
   LDA   $81
   ADC   #$00
   STA   DPTR+1
   LDA   (DPTR),Y
   STA   $85
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$24
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@19
.paint_hud_strip_p1@asm_ssa_block@11:
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$28
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@21
.paint_hud_strip_p1@asm_ssa_block@12:
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$2C
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@23
.paint_hud_strip_p1@asm_ssa_block@13:
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$30
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@25
.paint_hud_strip_p1@asm_ssa_block@14:
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$34
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@27
.paint_hud_strip_p1@asm_ssa_block@15:
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$38
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@29
.paint_hud_strip_p1@asm_ssa_block@16:
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$3C
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@31
.paint_hud_strip_p1@asm_ssa_block@17:
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   $87
   CLC
   ADC   #$01
   STA   $86
   LDA   $80
   CLC
   ADC   $87
   STA   DPTR
   LDA   $81
   ADC   #$00
   STA   DPTR+1
   LDA   (DPTR),Y
   STA   $85
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$21
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@33
.paint_hud_strip_p1@asm_ssa_block@18:
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   $86
   CLC
   ADC   #$01
   STA   $87
   LDA   $80
   CLC
   ADC   $86
   STA   DPTR
   LDA   $81
   ADC   #$00
   STA   DPTR+1
   LDA   (DPTR),Y
   STA   $85
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$25
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@35
.paint_hud_strip_p1@asm_ssa_block@19:
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$29
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@37
.paint_hud_strip_p1@asm_ssa_block@20:
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$2D
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@39
.paint_hud_strip_p1@asm_ssa_block@21:
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$31
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@41
.paint_hud_strip_p1@asm_ssa_block@22:
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$35
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@43
.paint_hud_strip_p1@asm_ssa_block@23:
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$39
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@45
.paint_hud_strip_p1@asm_ssa_block@24:
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$3D
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@47
.paint_hud_strip_p1@asm_ssa_block@25:
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   $87
   CLC
   ADC   #$01
   STA   $86
   LDA   $80
   CLC
   ADC   $87
   STA   DPTR
   LDA   $81
   ADC   #$00
   STA   DPTR+1
   LDA   (DPTR),Y
   STA   $85
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$21
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@49
.paint_hud_strip_p1@asm_ssa_block@26:
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   $86
   CLC
   ADC   #$01
   STA   $89
   LDA   $80
   CLC
   ADC   $86
   STA   DPTR
   LDA   $81
   ADC   #$00
   STA   DPTR+1
   LDA   (DPTR),Y
   STA   $85
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$25
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@51
.paint_hud_strip_p1@asm_ssa_block@27:
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$29
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@53
.paint_hud_strip_p1@asm_ssa_block@28:
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$2D
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@55
.paint_hud_strip_p1@asm_ssa_block@29:
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$31
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@57
.paint_hud_strip_p1@asm_ssa_block@30:
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$35
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@59
.paint_hud_strip_p1@asm_ssa_block@31:
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$39
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@61
.paint_hud_strip_p1@asm_ssa_block@32:
   LDA   #$00
   JMP   .sx_done@62
.sx_neg@61:
   LDA   #$FF
.sx_done@62:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$80
   CLC
   ADC   #$0C
   STA   $84
   LDA   #$3D
   ADC   #$00
   STA   $83
   LDA   $88
   BMI   .sx_neg@63
.paint_hud_strip_p1@asm_ssa_block@33:
   LDA   #$00
   JMP   .sx_done@64
.sx_neg@63:
   LDA   #$FF
.sx_done@64:
   STA   $82
   LDA   $84
   CLC
   ADC   $88
   STA   DPTR
   LDA   $83
   ADC   $82
   STA   DPTR+1
   LDA   $85
   LDY   #$00
   STA   (DPTR),Y
.loop@0_continue:
   LDA   $88
   SEC
   SBC   #$01
   STA   $88
   JMP   .loop@0_start
.loop@0_break:
   RTS
