/*
    decode.c -- decoding and running one instruction, and what every
    instruction shares: its operands, the variables, the stack, the
    branch that ends a test, and calls and returns.

    An instruction is an opcode, its operands, and then, for some, the
    variable to store its result in and the branch to take. Operands
    are constants of one or two bytes or variables. Variable 0 is the
    top of the stack, 1 to 15 the current routine's locals, and 16 to
    255 the story's globals. The stack holds big-endian words and grows
    down from $03E8.

    Departures from the original:

    - The original jumps through four tables of handler addresses
      (routines_table_0op, _1op, _2op, and _var). Here each table is a
      switch, which the compiler turns into a chain of compares, and
      the handlers of a few lines are written into their cases. A
      table of function pointers would make each handler's address
      taken, which keeps it off the zero-page calling convention, and
      calls through a pointer go through the runtime's icall
      trampoline, which this repository doesn't have.
    - The original runs DEBUG_JUMP, a JMP in zero page that the boot
      loader makes an RTS, before each handler, and copies Z_PC to
      TMP_Z_PC for a debugger. Here neither is done.
    - Handlers end by returning to do_instruction's caller, where the
      original jumps back to do_instruction.
    - The operands, the variables' values, and the branch offset are
      arguments and return values, where the original passes them in
      SCRATCH1, SCRATCH2, and SCRATCH3. store_and_next, which stores
      SCRATCH2 in the variable the next code byte names, is
      store_result(value). store_zero_and_next and store_A_and_next are
      store_result(0) and store_result(a).
    - branch and negated_branch, which end a test that has passed and
      one that hasn't, are one routine, branch(condition).
    - The 6502's version of instr_ret restores the locals through a
      pointer that starts at GLOBAL_ZVARS_ADDR's own address, $98, two
      bytes before LOCAL_ZVARS. Here it indexes LOCAL_ZVARS.
    - instr_call pushes the count of the routine's locals as a word
      whose high byte is whatever was in SCRATCH2+1. Here the high
      byte is 0. instr_ret reads only the low byte.
    - instr_rtrue and instr_rfalse store their result in OPERAND0 and
      then fall into instr_ret. Here they call return_value(1) and
      return_value(0), and OPERAND0 is left as it was.
    - instr_je compares OPERAND0 with each later operand in turn. The
      original, on a mismatch in the low byte of the fourth operand,
      goes back to compare the second again, which fails again and
      ends at the same place.
    - instr_loadw doubles OPERAND1 in place, and instr_storew shifts
      OPERAND1's high byte in place. Here the operands are left as
      they are.

    Zork I (r15), output/main.asm lines 404-907, 922-963, 1019-1057,
    1126-1227, 1238-1269, 1310-1318, 1341-1359, 1464-1473, 1538-1703,
    2020-2031, 2084-2095, 2306-2357:

    routines_table_0op:
        WORD     instr_rtrue
        WORD     instr_rfalse
        WORD     instr_print
        WORD     instr_print_ret
        WORD     instr_nop
        WORD     instr_save
        WORD     instr_restore
        WORD     instr_restart
        WORD     instr_ret_popped
        WORD     instr_pop
        WORD     instr_quit
        WORD     instr_new_line
    routines_table_1op:
        WORD     instr_jz
        WORD     instr_get_sibling
        WORD     instr_get_child
        WORD     instr_get_parent
        WORD     instr_get_prop_len
        WORD     instr_inc
        WORD     instr_dec
        WORD     instr_print_addr
        WORD     illegal_opcode
        WORD     instr_remove_obj
        WORD     instr_print_obj
        WORD     instr_ret
        WORD     instr_jump
        WORD     instr_print_paddr
        WORD     instr_load
        WORD     instr_not
    routines_table_2op:
        WORD     illegal_opcode
        WORD     instr_je
        WORD     instr_jl
        WORD     instr_jg
        WORD     instr_dec_chk
        WORD     instr_inc_chk
        WORD     instr_jin
        WORD     instr_test
        WORD     instr_or
        WORD     instr_and
        WORD     instr_test_attr
        WORD     instr_set_attr
        WORD     instr_clear_attr
        WORD     instr_store
        WORD     instr_insert_obj
        WORD     instr_loadw
        WORD     instr_loadb
        WORD     instr_get_prop
        WORD     instr_get_prop_addr
        WORD     instr_get_next_prop
        WORD     instr_add
        WORD     instr_sub
        WORD     instr_mul
        WORD     instr_div
        WORD     instr_mod
    routines_table_var:
        WORD     instr_call
        WORD     instr_storew
        WORD     instr_storeb
        WORD     instr_put_prop
        WORD     instr_sread
        WORD     instr_print_char
        WORD     instr_print_num
        WORD     instr_random
        WORD     instr_push
        WORD     instr_pull
    do_instruction:
        MOVW     Z_PC, TMP_Z_PC     ; Save PC for debugging
        MOVB     Z_PC+2, TMP_Z_PC+2
        STOB     #$00, OPERAND_COUNT
        JSR      get_next_code_byte
        STA      CURR_OPCODE
        CMP      #$80           ; is 2op?
        BCS      .is_gte_80
        JMP      .do_2op
    .is_gte_80:
        CMP      #$B0           ; is 1op?
        BCS      .is_gte_B0
        JMP      .do_1op
    .is_gte_B0:
        CMP      #$C0           ; is 0op?
        BCC      .do_0op
        JSR      get_next_code_byte
        ; Falls through to varop handling.
        LDX      #$00               ; operand number
    .get_next_operand:
        PHA                         ; save operand map
        TAY
        TXA
        PHA                         ; save operand number
        TYA
        AND      #$C0               ; check top 2 bits
        BNE      .is_01_10_11
        JSR      get_const_word            ; handle 00
        JMP      .store_operand
    .is_01_10_11:
        CMP      #$80
        BNE      .is_01_11
        JSR      get_var_content           ; handle 10
        JMP      .store_operand
    .is_01_11:
        CMP      #$40
        BNE      .is_11
        JSR      get_const_byte            ; handle 01
        JMP      .store_operand
    .is_11:
        PLA
        PLA
        JMP      .handle_varoperand_opcode ; handle 11 (ends operand list)
    .store_operand:
        PLA
        TAX
        LDA      SCRATCH2
        STA      OPERAND0,X
        LDA      SCRATCH2+1
        STA      OPERAND0+1,X
        INX
        INX
        INC      OPERAND_COUNT
        PLA                                ; shift operand map left 2 bits
        SEC
        ROL
        SEC
        ROL
        JMP      .get_next_operand
    .handle_varoperand_opcode:
        STOW     routines_table_var, SCRATCH2
        LDA      CURR_OPCODE
        CMP      #$E0
        BCS      .is_vararg_instr
        JMP      .check_for_good_2op
    .is_vararg_instr:
        SBC      #$E0               ; Allow only E0-E9.
        CMP      #$0A
        BCC      .opcode_table_jump
        JMP      illegal_opcode
    .opcode_table_jump:
        ASL
        TAY
        LDA      (SCRATCH2),Y
        STA      SCRATCH1
        INY
        LDA      (SCRATCH2),Y
        STA      SCRATCH1+1
        JSR      DEBUG_JUMP
        JMP      (SCRATCH1)
    .do_0op:
        SEC
        SBC      #$B0
        CMP      #$0C
        BCC      .load_opcode_table
        JMP      illegal_opcode
    .load_opcode_table:
        PHA
        STOW     routines_table_0op, SCRATCH2
        PLA
        JMP      .opcode_table_jump
    .do_1op:
        AND      #$30
        BNE      .is_90_to_AF
        JSR      get_const_word   ; Get operand for opcodes 80-8F
        JMP      .1op_arg_loaded
    .is_90_to_AF:
        CMP      #$10
        BNE      .is_A0_to_AF
        JSR      get_const_byte   ; Get operand for opcodes 90-9F
        JMP      .1op_arg_loaded
    .is_A0_to_AF:
        JSR      get_var_content  ; Get operand for opcodes A0-AF
    .1op_arg_loaded:
        STOB     #$01, OPERAND_COUNT
        MOVW     SCRATCH2, OPERAND0
        LDA      CURR_OPCODE
        AND      #$0F
        CMP      #$10
        BCC      .go_to_1op
        JMP      illegal_opcode
    .go_to_1op:
        PHA
        STOW     routines_table_1op, SCRATCH2
        PLA
        JMP      .opcode_table_jump
    .do_2op:
        AND      #$40
        BNE      .first_arg_is_var
        JSR      get_const_byte
        JMP      .get_next_arg
    .first_arg_is_var:
        JSR      get_var_content
    .get_next_arg:
        MOVW     SCRATCH2, OPERAND0
        LDA      CURR_OPCODE
        AND      #$20
        BNE      .second_arg_is_var
        JSR      get_const_byte
        JMP      .store_second_arg
    .second_arg_is_var:
        JSR      get_var_content
    .store_second_arg:
        MOVW     SCRATCH2, OPERAND1
        STOB     #$02, OPERAND_COUNT
        LDA      CURR_OPCODE
    .check_for_good_2op:
        AND      #$1F
        CMP      #$19
        BCC      .go_to_op2
        JMP      illegal_opcode
    .go_to_op2:
        PHA
        STOW     routines_table_2op, SCRATCH2
        PLA
        JMP      .opcode_table_jump
    get_const_byte:
        JSR      get_next_code_byte
        STA      SCRATCH2
        STOB     #$00, SCRATCH2+1
        RTS
    get_const_word:
        JSR      get_next_code_byte
        PHA
        JSR      get_next_code_byte
        STA      SCRATCH2
        PLA
        STA      SCRATCH2+1
        RTS
    var_get:
        ORA      #$00
        BEQ      pop_push
        JMP      get_nonstack_var
    var_put:
        ORA      #$00
        BEQ      .pop_push
        JMP      store_var2
    pop_push:
        JSR      pop
        JMP      push
    .pop_push:
        PSHW     SCRATCH2
        JSR      pop
        PULW     SCRATCH2
        JMP      push
    get_var_content:
        JSR      get_next_code_byte         ; A = get_next_code_byte<Z_PC>
        ORA      #$00                       ; if (!A) get_top_of_stack
        BEQ      get_top_of_stack
    get_nonstack_var:
        CMP      #$10                       ; if (A < #$10) {
        BCS      .compute_global_var_index
        SEC                                 ;   SCRATCH2 = LOCAL_ZVARS[A - 1]
        SBC      #$01
        ASL
        TAX
        LDA      LOCAL_ZVARS,X
        STA      SCRATCH2+1
        INX
        LDA      LOCAL_ZVARS,X
        STA      SCRATCH2
        RTS                                 ;   return
                                            ; }
    .compute_global_var_index:
        SEC                                 ; var_ptr = 2 * (A - #$10)
        SBC      #$10
        ASL
        STA      SCRATCH1
        LDA      #$00
        ROL
        STA      SCRATCH1+1
    .get_global_var_addr:
        ; var_ptr += GLOBAL_ZVARS_ADDR
        ADDW     GLOBAL_ZVARS_ADDR, SCRATCH1, SCRATCH1
    .get_global_var_value:
        LDY      #$00                       ; SCRATCH2 = *var_ptr
        LDA      (SCRATCH1),Y
        STA      SCRATCH2+1
        INY
        LDA      (SCRATCH1),Y
        STA      SCRATCH2
        RTS                                 ; return
    get_top_of_stack:
        JSR      pop                        ; SCRATCH2 = pop()
        RTS                                 ; return
    store_zero_and_next:
        LDA      #$00
    store_A_and_next:
        STA      SCRATCH2
        STOB     #$00, SCRATCH2+1
    store_and_next:
        JSR      store_var
        JMP      do_instruction
    store_var:
        PSHW     SCRATCH2               ; A = get_next_code_byte()
        JSR      get_next_code_byte
        TAX
        PULW     SCRATCH2
        TXA
    store_var2:
        ORA      #$00
        BNE      .nonstack
        JMP      push
    .nonstack:
        CMP      #$10
        BCS      .global_var
        SEC
        SBC      #$01
        ASL
        TAX
        LDA      SCRATCH2+1
        STA      LOCAL_ZVARS,X
        INX
        LDA      SCRATCH2
        STA      LOCAL_ZVARS,X
        RTS
    .global_var:
        SEC
        SBC      #$10
        ASL
        STA      SCRATCH1
        LDA      #$00
        ROL
        STA      SCRATCH1+1
        ADDW     GLOBAL_ZVARS_ADDR, SCRATCH1, SCRATCH1
        LDY      #$00
        LDA      SCRATCH2+1
        STA      (SCRATCH1),Y
        INY
        LDA      SCRATCH2
        STA      (SCRATCH1),Y
        RTS
    branch:
        JSR      get_next_code_byte
        ORA      #$00
        BMI      .do_branch
        BPL      .no_branch     ; unconditional
    negated_branch:
        JSR      get_next_code_byte
        ORA      #$00
        BPL      .do_branch
    .no_branch:
        AND      #$40
        BNE      .next
        JSR      get_next_code_byte
    .next:
        JMP      do_instruction
    .do_branch:
        TAX
        AND      #$40
        BEQ      .get_14_bit_offset
    .offset_is_6_bits:
        TXA
        AND      #$3F
        STA      SCRATCH2
        LDA      #$00
        STA      SCRATCH2+1
        JMP      .check_for_return_false
    .get_14_bit_offset:
        TXA
        AND      #$3F
        PHA
        JSR      get_next_code_byte
        STA      SCRATCH2
        PLA
        STA      SCRATCH2+1
        AND      #$20
        BEQ      .check_for_return_false
        LDA      SCRATCH2+1
        ORA      #$C0
        STA      SCRATCH2+1
    .check_for_return_false:
        LDA      SCRATCH2+1
        ORA      SCRATCH2
        BEQ      instr_rfalse
        LDA      SCRATCH2
        SEC
        SBC      #$01
        STA      SCRATCH2
        BCS      .check_for_return_true
        DEC      SCRATCH2+1
    .check_for_return_true:
        LDA      SCRATCH2+1
        ORA      SCRATCH2
        BEQ      instr_rtrue
    branch_to_offset:
        SUBB     SCRATCH2, #$01
        LDA      SCRATCH2+1
        STA      SCRATCH1
        ASL
        LDA      #$00
        ROL
        STA      SCRATCH1+1
        LDA      Z_PC
        CLC
        ADC      SCRATCH2
        BCC      .continue2
        INC      SCRATCH1
        BNE      .continue2
        INC      SCRATCH1+1
    .continue2:
        STA      Z_PC
        LDA      SCRATCH1+1
        ORA      SCRATCH1
        BEQ      .next
        CLC
        LDA      SCRATCH1
        ADC      Z_PC+1
        STA      Z_PC+1
        LDA      SCRATCH1+1
        ADC      Z_PC+2
        AND      #$01
        STA      Z_PC+2
        LDA      #$00
        STA      ZCODE_PAGE_VALID
        JMP      do_instruction
    .next:
        JMP      do_instruction
    instr_rtrue:
        LDA      #$01
    ret_a:
        STA      OPERAND0
        LDA      #$00
        STA      OPERAND0+1
        JMP      instr_ret
    instr_rfalse:
        LDA      #$00
        JMP      ret_a
    instr_print:
        JSR      print_string_literal
        JMP      do_instruction
    instr_print_ret:
        JSR      print_string_literal
        LDA      #$0D
        JSR      buffer_char
        LDA      #$0A
        JSR      buffer_char
        JMP      instr_rtrue
    instr_nop:
        JMP      do_instruction
    instr_ret_popped:
        JSR      pop
        MOVW     SCRATCH2, OPERAND0
        JMP      instr_ret
    instr_pop:
        JSR      pop
        JMP      do_instruction
    instr_new_line:
        LDA      #$0D
        JSR      buffer_char
        LDA      #$0A
        JSR      buffer_char
        JMP      do_instruction
    instr_jz:
        LDA      OPERAND0+1
        ORA      OPERAND0
        BEQ      take_branch
        JMP      negated_branch
    take_branch:
        JMP      branch
    instr_inc:
        JSR      inc_var
        JMP      do_instruction
    instr_dec:
        JSR      dec_var
        JMP      do_instruction
    inc_var:
        LDA      OPERAND0
        JSR      var_get
        INCW     SCRATCH2
    inc_var_continue:
        PSHW     SCRATCH2
        LDA      OPERAND0
        JSR      var_put
        PULW     SCRATCH2
        RTS
    dec_var:
        LDA      OPERAND0
        JSR      var_get
        SUBB     SCRATCH2, #$01
        JMP      inc_var_continue
    instr_print_addr:
        MOVW     OPERAND0, SCRATCH2
        JSR      load_address
        JMP      print_zstring_and_next
    illegal_opcode:
        JSR      brk
    instr_ret:
        MOVW     FRAME_Z_SP, Z_SP
        MOVB     FRAME_STACK_COUNT, STACK_COUNT
        JSR      pop
        LDA      SCRATCH2
        BEQ      .done_locals
        STOW     GLOBAL_ZVARS_ADDR, SCRATCH1    ; ptr = GLOBAL_ZVARS_ADDR
        MOVB     SCRATCH2, SCRATCH3             ; count = STRATCH2
        ASL                                     ; ptr += 2 * count
        ADDA     SCRATCH1
    .loop:
        JSR      pop                ; SCRATCH2 = pop()
        LDY      #$01               ; *ptr = SCRATCH2
        LDA      SCRATCH2
        STA      (SCRATCH1),Y
        DEY
        LDA      SCRATCH2+1
        STA      (SCRATCH1),Y
        SUBB     SCRATCH1, #$02     ; ptr -= 2
        DEC      SCRATCH3           ; --count
        BNE      .loop
    .done_locals:
        JSR      pop
        MOVW     SCRATCH2, Z_PC+1
        JSR      pop
        MOVW     SCRATCH2, FRAME_Z_SP
        JSR      pop
        MOVB     SCRATCH2+1, Z_PC
        MOVB     SCRATCH2, FRAME_STACK_COUNT
        STOB     #$00, ZCODE_PAGE_VALID
        MOVW     OPERAND0, SCRATCH2
        JMP      store_and_next
    instr_jump:
        MOVW     OPERAND0, SCRATCH2
        SUBB     SCRATCH2, #$01
        JMP      branch_to_offset
    instr_print_paddr:
        MOVW     OPERAND0, SCRATCH2     ; Z_PC2 <- OPERAND0 * 2
        JSR      load_packed_address
        ; Falls through to print_zstring_and_next
    print_zstring_and_next:
        JSR      print_zstring
        JMP      do_instruction
    instr_load:
        LDA      OPERAND0
        JSR      var_get
        JMP      store_and_next
    instr_not:
        LDA      OPERAND0
        EOR      #$FF
        STA      SCRATCH2
        LDA      OPERAND0+1
        EOR      #$FF
        STA      SCRATCH2+1
        JMP      store_and_next
    instr_jl:
        MOVW     OPERAND0, SCRATCH2
        MOVW     OPERAND1, SCRATCH1
        JSR      cmp16
        BCC      stretch_to_branch
        JMP      negated_branch
    instr_jg:
        MOVW     OPERAND0, SCRATCH1
        MOVW     OPERAND1, SCRATCH2
        JSR      cmp16
        BCC      stretch_to_branch
        JMP      negated_branch
    instr_dec_chk:
        JSR      dec_var
        MOVW     OPERAND1, SCRATCH1
        JMP      do_chk
    instr_inc_chk:
        JSR      inc_var
        MOVW     SCRATCH2, SCRATCH1
        MOVW     OPERAND1, SCRATCH2
    do_chk:
        JSR      cmp16
        BCC      stretch_to_branch
        JMP      negated_branch
    stretch_to_branch:
        JMP      branch
    instr_test:
        MOVB     OPERAND1+1, SCRATCH2+1
        AND      OPERAND0+1
        STA      SCRATCH1+1
        MOVB     OPERAND1, SCRATCH2
        AND      OPERAND0
        STA      SCRATCH1
        JSR      cmpu16
        BEQ      stretch_to_branch
        JMP      negated_branch
    instr_or:
        LDA      OPERAND1+1
        ORA      OPERAND0+1
        STA      SCRATCH2+1
        LDA      OPERAND1
        ORA      OPERAND0
        STA      SCRATCH2
        JMP      store_and_next
    instr_and:
        LDA      OPERAND1+1
        AND      OPERAND0+1
        STA      SCRATCH2+1
        LDA      OPERAND1
        AND      OPERAND0
        STA      SCRATCH2
        JMP      store_and_next
    instr_store:
        MOVW     OPERAND1, SCRATCH2
        LDA      OPERAND0
    stretch_var_put:
        JSR      var_put
        JMP      do_instruction
    instr_loadw:
        ASL      OPERAND1               ; OPERAND1 *= 2
        ROL      OPERAND1+1
        ADDW     OPERAND1, OPERAND0, SCRATCH2
        JSR      load_address
        JSR      get_next_code_word
        JMP      store_and_next
    instr_loadb:
        ADDW     OPERAND1, OPERAND0, SCRATCH2   ; SCRATCH2 = OPERAND0 + OPERAND1
        JSR      load_address                   ; Z_PC2 = SCRATCH2
        JSR      get_next_code_byte2            ; A = *Z_PC2
        STA      SCRATCH2                       ; SCRATCH2 = uint16(A)
        LDA      #$00
        STA      SCRATCH2+1
        JMP      store_and_next                 ; store_and_next(SCRATCH2)
    instr_add:
        ADDW     OPERAND0, OPERAND1, SCRATCH2
        JMP      store_and_next
    instr_sub:
        SUBW     OPERAND0, OPERAND1, SCRATCH2
        JMP      store_and_next
    instr_je:
        LDX      OPERAND_COUNT
        DEX
        BNE      .check_second
        JSR      brk
    .check_second:
        LDA      OPERAND0
        CMP      OPERAND1
        BNE      .check_next
        LDA      OPERAND0+1
        CMP      OPERAND1+1
        BEQ      .branch
    .check_next:
        DEX
        BEQ      .neg_branch
        LDA      OPERAND0
        CMP      OPERAND0+4
        BNE      .check_next2
        LDA      OPERAND0+1
        CMP      OPERAND0+5
        BEQ      .branch
    .check_next2:
        DEX
        BEQ      .neg_branch
        LDA      OPERAND0
        CMP      OPERAND0+6
        BNE      .check_second      ; why not just go to .neg_branch?
        LDA      OPERAND0+1
        CMP      OPERAND0+7
        BEQ      .branch
    .neg_branch:
        JMP      negated_branch
    .branch:
        JMP      branch
    instr_call:
        LDA      OPERAND0
        ORA      OPERAND0+1
        BNE      .push_frame
        STOW     #$0000, SCRATCH2
        JMP      store_and_next
    .push_frame:
        MOVB     FRAME_STACK_COUNT, SCRATCH2
        MOVB     Z_PC, SCRATCH2+1
        JSR      push
        MOVW     FRAME_Z_SP, SCRATCH2
        JSR      push
        MOVW     Z_PC+1, SCRATCH2
        JSR      push
        STOB     #$00, ZCODE_PAGE_VALID
        LDA      OPERAND0
        ASL
        STA      Z_PC
        LDA      OPERAND0+1
        ROL
        STA      Z_PC+1
        LDA      #$00
        ROL
        STA      Z_PC+2
        JSR      get_next_code_byte     ; local_var_count = get_next_code_byte()
        PHA                             ; Save local_var_count
        ORA      #$00
        BEQ      .after_loop2
        LDX      #$00                   ; X = 0
    .push_and_init_local_vars:
        PHA                             ; Save local_var_count
        LDA      LOCAL_ZVARS,X          ; Push LOCAL_ZVAR[X] onto the stack
        STA      SCRATCH2+1
        INX
        LDA      LOCAL_ZVARS,X
        STA      SCRATCH2
        DEX
        TXA
        PHA
        JSR      push
        JSR      get_next_code_byte     ; SCRATCH2 = next init val
        PHA
        JSR      get_next_code_byte
        STA      SCRATCH2
        PLA
        STA      SCRATCH2+1
        PLA                             ; Restore local_var_count
        TAX
        LDA      SCRATCH2+1             ; LOCAL_ZVARS[X] = SCRATCH2
        STA      LOCAL_ZVARS,X
        INX
        LDA      SCRATCH2
        STA      LOCAL_ZVARS,X
        INX                             ; Increment X
        PLA                             ; Decrement local_var_count
        SEC
        SBC      #$01
        BNE      .push_and_init_local_vars  ; Loop until no more vars
    .after_loop2:
        LDA      OPERAND_COUNT          ; count = OPERAND_COUNT - 1
        STA      SCRATCH3
        DEC      SCRATCH3
        BEQ      .done_init_local_vars  ; if (!count) .done_init_local_vars
        STOB     #$00, SCRATCH1         ; operand = 0
        STOB     #$00, SCRATCH2         ; zvar = 0
    .loop:
        LDX      SCRATCH1               ; LOCAL_ZVARS[zvar] = OPERANDS[operand+1]
        LDA      OPERAND1+1,X           ; high byte first
        LDX      SCRATCH2
        STA      LOCAL_ZVARS,X
        INC      SCRATCH2
        LDX      SCRATCH1
        LDA      OPERAND1,X
        LDX      SCRATCH2
        STA      LOCAL_ZVARS,X
        INC      SCRATCH2               ; ++zvar
        INC      SCRATCH1               ; ++operand
        INC      SCRATCH1
        DEC      SCRATCH3               ; --count
        BNE      .loop                  ; if (count) .loop
    .done_init_local_vars:
        PULB     SCRATCH2               ; Restore local_var_count
        JSR      push                   ; Push local_var_count
        MOVB     STACK_COUNT, FRAME_STACK_COUNT
        MOVW     Z_SP, FRAME_Z_SP
        JMP      do_instruction
    instr_storew:
        LDA      OPERAND1       ; SCRATCH2 = Z_HEADER_ADDR + OPERAND0 + 2*OPERAND1
        ASL
        ROL      OPERAND1+1
        CLC
        ADC      OPERAND0
        STA      SCRATCH2
        LDA      OPERAND1+1
        ADC      OPERAND0+1
        STA      SCRATCH2+1
        ADDW     SCRATCH2, Z_HEADER_ADDR, SCRATCH2
        LDY      #$00
        LDA      OPERAND2+1
        STA      (SCRATCH2),Y
        INY
        LDA      OPERAND2
        STA      (SCRATCH2),Y
        JMP      do_instruction
    instr_storeb:
        LDA      OPERAND1       ; SCRATCH2 = Z_HEADER_ADDR + OPERAND0 + OPERAND1
        CLC
        ADC      OPERAND0
        STA      SCRATCH2
        LDA      OPERAND1+1
        ADC      OPERAND0+1
        STA      SCRATCH2+1
        ADDW     SCRATCH2, Z_HEADER_ADDR, SCRATCH2
        LDY      #$00
        LDA      OPERAND2
        STA      (SCRATCH2),Y
        JMP      do_instruction
    instr_print_char:
        LDA      OPERAND0
        JSR      buffer_char
        JMP      do_instruction
    instr_print_num:
        MOVW     OPERAND0, SCRATCH2
        JSR      print_number
        JMP      do_instruction
    instr_push:
        MOVW     OPERAND0, SCRATCH2
        JSR      push
        JMP      do_instruction
    instr_pull:
        JSR      pop
        LDA      OPERAND0
        JMP      stretch_var_put
    cmp16:
        LDA      SCRATCH1+1
        EOR      SCRATCH2+1
        BPL      cmpu16
        LDA      SCRATCH1+1
        CMP      SCRATCH2+1
        RTS
    cmpu16:
        LDA      SCRATCH2+1
        CMP      SCRATCH1+1
        BNE      .end
        LDA      SCRATCH2
        CMP      SCRATCH1
    .end:
        RTS
    push:
        SUBB     Z_SP, #$01
        LDY      #$00
        LDA      SCRATCH2
        STA      (Z_SP),Y
        SUBB     Z_SP, #$01
        LDA      SCRATCH2+1
        STA      (Z_SP),Y
        INC      STACK_COUNT
        LDA      STACK_COUNT
        CMP      #$B4
        BCC      .end
        JSR      brk
    .end:
        RTS
    pop:
        LDY      #$00
        LDA      (Z_SP),Y
        STA      SCRATCH2+1
        INCW     Z_SP
        LDA      (Z_SP),Y
        STA      SCRATCH2
        INCW     Z_SP
        DEC      STACK_COUNT
        BNE      .end
        JSR      brk
    .end:
        RTS
*/
#include "zork.h"

/* Pushes the word `value`. More than 179 words on the stack is an
 * internal error. */
void push(uint16_t value) {
    Z_SP--;
    *Z_SP = (uint8_t)value;
    Z_SP--;
    *Z_SP = (uint8_t)(value >> 8);
    if (++STACK_COUNT >= 0xB4)
        internal_error();
}

/* Pops a word. Popping from an empty stack is an internal error. */
uint16_t pop(void) {
    uint8_t hi = Z_SP[0];
    uint8_t lo = Z_SP[1];
    Z_SP += 2;
    if (--STACK_COUNT == 0)
        internal_error();
    return ((uint16_t)hi << 8) | lo;
}

/* Returns local or global variable `var`, which isn't 0. */
static uint16_t get_nonstack_var(register uint8_t var) {
    if (var < 0x10) {
        uint8_t x = (uint8_t)((var - 1) << 1);
        return ((uint16_t)LOCAL_ZVARS[x] << 8) | LOCAL_ZVARS[x + 1];
    }
    uint8_t *p = GLOBAL_ZVARS_ADDR + ((uint16_t)(uint8_t)(var - 0x10) << 1);
    return ((uint16_t)p[0] << 8) | p[1];
}

/* Sets local or global variable `var`, which isn't 0, to `value`. */
static void set_nonstack_var(register uint8_t var, uint16_t value) {
    if (var < 0x10) {
        uint8_t x = (uint8_t)((var - 1) << 1);
        LOCAL_ZVARS[x] = (uint8_t)(value >> 8);
        LOCAL_ZVARS[x + 1] = (uint8_t)value;
        return;
    }
    uint8_t *p = GLOBAL_ZVARS_ADDR + ((uint16_t)(uint8_t)(var - 0x10) << 1);
    p[0] = (uint8_t)(value >> 8);
    p[1] = (uint8_t)value;
}

/* Returns variable `var`. Variable 0, the top of the stack, stays on
 * the stack. */
uint16_t var_get(register uint8_t var) {
    if (var == 0) {
        uint16_t value = pop();
        push(value);
        return value;
    }
    return get_nonstack_var(var);
}

/* Sets variable `var` to `value`. Variable 0 replaces the top of the
 * stack. */
void var_put(register uint8_t var, uint16_t value) {
    if (var == 0) {
        pop();
        push(value);
        return;
    }
    set_nonstack_var(var, value);
}

/* Stores `value` in the variable the next code byte names. Variable 0
 * pushes it. */
void store_result(uint16_t value) {
    uint8_t var = get_next_code_byte();
    if (var == 0)
        push(value);
    else
        set_nonstack_var(var, value);
}

/* The operand forms: a word, a byte, and a variable, whose number is
 * the next code byte. Variable 0 pops the stack. */
static uint16_t get_const_word(void) {
    uint8_t hi = get_next_code_byte();
    uint8_t lo = get_next_code_byte();
    return ((uint16_t)hi << 8) | lo;
}

static uint16_t get_var_content(void) {
    uint8_t var = get_next_code_byte();
    if (var == 0)
        return pop();
    return get_nonstack_var(var);
}

/* Returns from the current routine with `value`: pops the stack back
 * to where the routine started, restores the caller's locals, its
 * Z_PC, and its frame, and stores `value` in the caller's result
 * variable. */
void return_value(uint16_t value) {
    Z_SP = FRAME_Z_SP;
    STACK_COUNT = FRAME_STACK_COUNT;
    uint8_t count = (uint8_t)pop();
    if (count != 0) {
        uint8_t x = (uint8_t)(count << 1);
        do {
            uint16_t w = pop();
            x -= 2;
            LOCAL_ZVARS[x] = (uint8_t)(w >> 8);
            LOCAL_ZVARS[x + 1] = (uint8_t)w;
        } while (--count != 0);
    }
    /* The caller's Z_PC, high and top bytes, then its frame. */
    uint16_t pc_high = pop();
    FRAME_Z_SP = (uint8_t *)pop();
    uint16_t w = pop();
    Z_PC.lo = (pc_high << 8) | (w >> 8);
    Z_PC.hi = (uint8_t)(pc_high >> 8);
    FRAME_STACK_COUNT = (uint8_t)w;
    ZCODE_PAGE_VALID = 0;
    store_result(value);
}

/* Moves Z_PC by `offset`, a signed offset from the instruction's end
 * less two, and keeps Z_PC within the 128K a story can be. The
 * instruction reader's page stays mapped when only Z_PC's low byte
 * changes. */
static void branch_to_offset(uint16_t offset) {
    /* The offset's high byte, sign-extended, and then the carry from
     * adding the low byte. */
    uint16_t pages = (uint8_t)(offset >> 8);
    if (offset & 0x8000)
        pages += 0x100;
    uint8_t lo = (uint8_t)Z_PC.lo;
    uint8_t new_lo = lo + (uint8_t)offset;
    if (new_lo < lo)
        pages++;
    if (pages == 0) {
        Z_PC.lo = (Z_PC.lo & 0xFF00) | new_lo;
        return;
    }
    uint16_t page = (Z_PC.lo >> 8) | ((uint16_t)Z_PC.hi << 8);
    page += pages;
    Z_PC.lo = (page << 8) | new_lo;
    Z_PC.hi = (uint8_t)(page >> 8) & 0x01;
    ZCODE_PAGE_VALID = 0;
}

/* Ends a test whose result is `condition`. The next code byte's top
 * bit says which result branches. Its bit 6 says whether the offset
 * is the byte's low six bits or fourteen bits, sign-extended, from it
 * and the byte after. An offset of 0 returns false from the routine,
 * 1 returns true, and any other moves Z_PC. */
void branch(register bool condition) {
    uint8_t b = get_next_code_byte();
    bool on_true = (b & 0x80) != 0;
    if (on_true != condition) {
        if ((b & 0x40) == 0)
            get_next_code_byte();
        return;
    }
    uint16_t offset;
    if (b & 0x40) {
        offset = b & 0x3F;
    } else {
        uint8_t hi = b & 0x3F;
        uint8_t lo = get_next_code_byte();
        if (hi & 0x20)
            hi |= 0xC0;
        offset = ((uint16_t)hi << 8) | lo;
    }
    if (offset == 0) {
        return_value(0);
        return;
    }
    if (offset == 1) {
        return_value(1);
        return;
    }
    branch_to_offset(offset - 2);
}

/* Calls the routine at packed address OPERAND0 with the other
 * operands as its first locals. The stack gets the caller's frame,
 * its Z_PC, and its locals, and then the count of locals. Calling
 * address 0 stores 0 and calls nothing. */
static void instr_call(void) {
    if (OPERANDS[0] == 0) {
        store_result(0);
        return;
    }
    push(((uint16_t)(uint8_t)Z_PC.lo << 8) | FRAME_STACK_COUNT);
    push((uint16_t)FRAME_Z_SP);
    push((Z_PC.lo >> 8) | ((uint16_t)Z_PC.hi << 8));
    ZCODE_PAGE_VALID = 0;
    Z_PC.lo = OPERANDS[0] << 1;
    Z_PC.hi = (uint8_t)(OPERANDS[0] >> 15);
    /* The routine starts with its count of locals and their initial
     * values. The caller's locals are pushed as they're replaced. */
    uint8_t locals = get_next_code_byte();
    if (locals != 0) {
        uint8_t x = 0;
        uint8_t n = locals;
        do {
            push(((uint16_t)LOCAL_ZVARS[x] << 8) | LOCAL_ZVARS[x + 1]);
            uint8_t hi = get_next_code_byte();
            uint8_t lo = get_next_code_byte();
            LOCAL_ZVARS[x] = hi;
            LOCAL_ZVARS[x + 1] = lo;
            x += 2;
        } while (--n != 0);
    }
    /* The arguments replace the first locals' initial values. */
    uint8_t args = OPERAND_COUNT - 1;
    uint8_t i = 0;
    while (args != 0) {
        uint16_t v = OPERANDS[i + 1];
        uint8_t x = (uint8_t)(i << 1);
        LOCAL_ZVARS[x] = (uint8_t)(v >> 8);
        LOCAL_ZVARS[x + 1] = (uint8_t)v;
        i++;
        args--;
    }
    push(locals);
    FRAME_STACK_COUNT = STACK_COUNT;
    FRAME_Z_SP = Z_SP;
}

/* Adds 1 to or subtracts 1 from the variable OPERAND0 names, and
 * returns its new value. */
static uint16_t inc_var(void) {
    uint8_t var = (uint8_t)OPERANDS[0];
    uint16_t v = var_get(var) + 1;
    var_put(var, v);
    return v;
}

static uint16_t dec_var(void) {
    uint8_t var = (uint8_t)OPERANDS[0];
    uint16_t v = var_get(var) - 1;
    var_put(var, v);
    return v;
}

/* Instructions with no operands, $B0 to $BB. */
static void do_0op(register uint8_t n) {
    switch (n) {
    case 0x0: return_value(1); break;                   /* rtrue */
    case 0x1: return_value(0); break;                   /* rfalse */
    case 0x2: print_string_literal(); break;            /* print */
    case 0x3:                                           /* print_ret */
        print_string_literal();
        new_line();
        return_value(1);
        break;
    case 0x4: break;                                    /* nop */
    case 0x5: instr_save(); break;
    case 0x6: instr_restore(); break;
    case 0x7: instr_restart(); break;
    case 0x8: return_value(pop()); break;               /* ret_popped */
    case 0x9: pop(); break;                             /* pop */
    case 0xA: instr_quit(); break;
    case 0xB: new_line(); break;                        /* new_line */
    default: internal_error(); break;
    }
}

/* Instructions with one operand, OPERAND0. */
static void do_1op(register uint8_t n) {
    uint16_t a = OPERANDS[0];
    switch (n) {
    case 0x0: branch(a == 0); break;                    /* jz */
    case 0x1: instr_get_sibling(); break;
    case 0x2: instr_get_child(); break;
    case 0x3: instr_get_parent(); break;
    case 0x4: instr_get_prop_len(); break;
    case 0x5: inc_var(); break;                         /* inc */
    case 0x6: dec_var(); break;                         /* dec */
    case 0x7:                                           /* print_addr */
        load_address(a);
        print_zstring();
        break;
    case 0x9: instr_remove_obj(); break;
    case 0xA: print_obj((uint8_t)a); break;             /* print_obj */
    case 0xB: return_value(a); break;                   /* ret */
    case 0xC: branch_to_offset(a - 2); break;           /* jump */
    case 0xD:                                           /* print_paddr */
        load_packed_address(a);
        print_zstring();
        break;
    case 0xE: store_result(var_get((uint8_t)a)); break; /* load */
    case 0xF: store_result(~a); break;                  /* not */
    default: internal_error(); break;
    }
}

/* Instructions with two operands, or, in their variable form, two to
 * four. */
static void do_2op(register uint8_t n) {
    uint16_t a = OPERANDS[0];
    uint16_t b = OPERANDS[1];
    switch (n) {
    case 0x01: {                                        /* je */
        uint8_t left = OPERAND_COUNT - 1;
        if (left == 0)
            internal_error();
        bool equal = a == b;
        if (!equal && --left != 0) {
            equal = a == OPERANDS[2];
            if (!equal && --left != 0)
                equal = a == OPERANDS[3];
        }
        branch(equal);
        break;
    }
    case 0x02: branch((int16_t)a < (int16_t)b); break;  /* jl */
    case 0x03: branch((int16_t)a > (int16_t)b); break;  /* jg */
    case 0x04:                                          /* dec_chk */
        branch((int16_t)dec_var() < (int16_t)b);
        break;
    case 0x05:                                          /* inc_chk */
        branch((int16_t)inc_var() > (int16_t)b);
        break;
    case 0x06: instr_jin(); break;
    case 0x07: branch((a & b) == b); break;             /* test */
    case 0x08: store_result(a | b); break;              /* or */
    case 0x09: store_result(a & b); break;              /* and */
    case 0x0A: instr_test_attr(); break;
    case 0x0B: instr_set_attr(); break;
    case 0x0C: instr_clear_attr(); break;
    case 0x0D: var_put((uint8_t)a, b); break;           /* store */
    case 0x0E: instr_insert_obj(); break;
    case 0x0F:                                          /* loadw */
        load_address(a + (b << 1));
        store_result(get_next_code_word());
        break;
    case 0x10:                                          /* loadb */
        load_address(a + b);
        store_result(get_next_code_byte2());
        break;
    case 0x11: instr_get_prop(); break;
    case 0x12: instr_get_prop_addr(); break;
    case 0x13: instr_get_next_prop(); break;
    case 0x14: store_result(a + b); break;              /* add */
    case 0x15: store_result(a - b); break;              /* sub */
    case 0x16: instr_mul(); break;
    case 0x17: instr_div(); break;
    case 0x18: instr_mod(); break;
    default: internal_error(); break;
    }
}

/* Instructions with a variable number of operands, $E0 to $E9. */
static void do_var(register uint8_t n) {
    uint16_t a = OPERANDS[0];
    switch (n) {
    case 0x0: instr_call(); break;
    case 0x1: {                                         /* storew */
        uint8_t *p = Z_HEADER_ADDR + a + (OPERANDS[1] << 1);
        p[0] = (uint8_t)(OPERANDS[2] >> 8);
        p[1] = (uint8_t)OPERANDS[2];
        break;
    }
    case 0x2:                                           /* storeb */
        Z_HEADER_ADDR[a + OPERANDS[1]] = (uint8_t)OPERANDS[2];
        break;
    case 0x3: instr_put_prop(); break;
    case 0x4: instr_sread(); break;
    case 0x5: buffer_char((uint8_t)a); break;           /* print_char */
    case 0x6: print_number(a); break;                   /* print_num */
    case 0x7: instr_random(); break;
    case 0x8: push(a); break;                           /* push */
    case 0x9: var_put((uint8_t)a, pop()); break;        /* pull */
    default: internal_error(); break;
    }
}

/* Reads the instruction at Z_PC and its operands, and runs it. */
void do_instruction(void) {
    OPERAND_COUNT = 0;
    uint8_t op = get_next_code_byte();
    CURR_OPCODE = op;
    if (op < 0x80) {
        /* Two operands, each a byte constant or a variable. */
        OPERANDS[0] = (op & 0x40) ? get_var_content() : get_next_code_byte();
        OPERANDS[1] = (op & 0x20) ? get_var_content() : get_next_code_byte();
        OPERAND_COUNT = 2;
        do_2op(op & 0x1F);
        return;
    }
    if (op < 0xB0) {
        /* One operand, of the form bits 4 and 5 give. */
        uint8_t form = op & 0x30;
        uint16_t v;
        if (form == 0x00)
            v = get_const_word();
        else if (form == 0x10)
            v = get_next_code_byte();
        else
            v = get_var_content();
        OPERAND_COUNT = 1;
        OPERANDS[0] = v;
        do_1op(op & 0x0F);
        return;
    }
    if (op < 0xC0) {
        do_0op(op - 0xB0);
        return;
    }
    /* Up to four operands, whose forms are two bits each of the next
     * code byte, from the top. 11 ends the list. Shifting in 1s ends
     * it after the fourth. */
    uint8_t forms = get_next_code_byte();
    for (;;) {
        uint8_t form = forms & 0xC0;
        uint16_t v;
        if (form == 0x00)
            v = get_const_word();
        else if (form == 0x80)
            v = get_var_content();
        else if (form == 0x40)
            v = get_next_code_byte();
        else
            break;
        OPERANDS[OPERAND_COUNT] = v;
        OPERAND_COUNT++;
        forms = (uint8_t)(forms << 2) | 0x03;
    }
    if (op >= 0xE0)
        do_var(op - 0xE0);
    else
        do_2op(op & 0x1F);
}
