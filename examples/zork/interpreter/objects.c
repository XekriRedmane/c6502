/*
    objects.c -- the object tree, attributes, and properties.

    Objects are numbered from 1. Each has a 9-byte entry in the object
    table, after the 31 default property values: 32 attribute bits,
    the numbers of its parent, its next sibling, and its first child
    (0 for none), and the address of its properties. The property
    table starts with the object's name, a byte count of words and
    then the z-string, and then its properties in descending order,
    each a size byte, whose top three bits are the length less one and
    whose low five the number, and then the value. A 0 size byte ends
    the list.

    Departures from the original:

    - get_object_addr returns the entry's address, and print_obj and
      the property routines take their inputs as arguments, where
      the original passes them in A, SCRATCH1, and SCRATCH2.
    - get_property_ptr leaves the property table's address in props
      and returns the offset of the first property, where the original
      leaves them in SCRATCH2 and Y, and the routines that walk the
      table take the offset as an argument and return the next one.
    - attr_ptr_and_mask leaves the attribute word's address and the
      mask in attr_ptr and attr_mask, and returns the word, where the
      original leaves them in SCRATCH2, SCRATCH3, and SCRATCH1.
    - remove_obj keeps the object's entry in a local where the
      original pushes it.

    Zork I (r15), output/main.asm lines 964-1018, 1058-1125,
    1228-1237, 1270-1309, 1319-1340, 1360-1463, 1704-1740, 2191-2305:

    instr_get_sibling:
        LDA      OPERAND0
        JSR      get_object_addr
        LDY      #OBJECT_SIBLING_OFFSET
        JMP      push_and_check_obj
    instr_get_child:
        LDA      OPERAND0
        JSR      get_object_addr
        LDY      #OBJECT_CHILD_OFFSET
    push_and_check_obj:
        LDA      (SCRATCH2),Y
        PHA
        STA      SCRATCH2
        LDA      #$00
        STA      SCRATCH2+1
        JSR      store_var    ; store in var of next code byte.
        PLA
        ORA      #$00
        BNE      take_branch
        JMP      negated_branch
    instr_get_parent:
        LDA      OPERAND0
        JSR      get_object_addr
        LDY      #OBJECT_PARENT_OFFSET
        LDA      (SCRATCH2),Y
        STA      SCRATCH2
        LDA      #$00
        STA      SCRATCH2+1
        JMP      store_and_next
    instr_get_prop_len:
        CLC
        LDA      OPERAND0
        ADC      Z_HEADER_ADDR
        STA      SCRATCH2
        LDA      OPERAND0+1
        ADC      Z_HEADER_ADDR+1
        STA      SCRATCH2+1
        LDA      SCRATCH2
        SEC
        SBC      #$01
        STA      SCRATCH2
        BCS      .continue
        DEC      SCRATCH2+1
    .continue:
        LDY      #$00
        JSR      get_property_len
        CLC
        ADC      #$01
        JMP      store_A_and_next
    instr_remove_obj:
        JSR      remove_obj
        JMP      do_instruction
    remove_obj:
        LDA      OPERAND0               ; obj_ptr = get_object_addr<obj_num>
        JSR      get_object_addr
        LDY      #OBJECT_PARENT_OFFSET  ; A = obj_ptr->parent
        LDA      (SCRATCH2),Y
        BNE      .continue              ; if (!A) return
        RTS
    .continue:
        TAX                             ; save obj_ptr
        PSHW     SCRATCH2
        TXA
        JSR      get_object_addr        ; parent_ptr = get_object_addr<A>
        LDY      #OBJECT_CHILD_OFFSET   ; child_num = parent_ptr->child
        LDA      (SCRATCH2),Y
        CMP      OPERAND0               ; if (child_num != obj_num) loop
        BNE      .loop
        PULW     SCRATCH1               ; restore obj_ptr
        PSHW     SCRATCH1
        LDY      #OBJECT_SIBLING_OFFSET ; A = obj_ptr->next
        LDA      (SCRATCH1),Y
        LDY      #OBJECT_CHILD_OFFSET   ; parent_ptr->child = A
        STA      (SCRATCH2),Y
        JMP      .detach
    .loop:
        JSR      get_object_addr        ; child_ptr = get_object_addr<child_num>
        LDY      #OBJECT_SIBLING_OFFSET ; child_num = child_ptr->next
        LDA      (SCRATCH2),Y
        CMP      OPERAND0               ; if (child_num != obj_num) loop
        BNE      .loop
        PULW     SCRATCH1               ; restore obj_ptr
        PSHW     SCRATCH1
        LDA      (SCRATCH1),Y           ; child_ptr->next = obj_ptr->next
        STA      (SCRATCH2),Y
    .detach:
        PULW     SCRATCH2               ; restore obj_ptr
        LDY      #OBJECT_PARENT_OFFSET  ; obj_ptr->parent = 0
        LDA      #$00
        STA      (SCRATCH2),Y
        INY
        STA      (SCRATCH2),Y
        RTS                      ; detach
    instr_print_obj:
        LDA      OPERAND0
        JSR      print_obj_in_A
        JMP      do_instruction
    print_obj_in_A:
        JSR      get_object_addr        ; obj_ptr = get_object_addr<A>
        LDY      #OBJECT_PROPS_OFFSET   ; props_ptr = obj_ptr->props
        LDA      (SCRATCH2),Y
        STA      SCRATCH1+1
        INY
        LDA      (SCRATCH2),Y
        STA      SCRATCH1
        MOVW     SCRATCH1, SCRATCH2
        INCW     SCRATCH2               ; ++props_ptr
        JSR      load_address           ; Z_PC2 = props_ptr
        JMP      print_zstring          ; print_zstring<Z_PC2>
    instr_jin:
        LDA      OPERAND0
        JSR      get_object_addr
        LDY      #OBJECT_PARENT_OFFSET
        LDA      OPERAND1
        CMP      (SCRATCH2),Y
        BEQ      stretch_to_branch
        JMP      negated_branch
    instr_test_attr:
        JSR      attr_ptr_and_mask
        LDA      SCRATCH1+1
        AND      SCRATCH3+1
        STA      SCRATCH1+1
        LDA      SCRATCH1
        AND      SCRATCH3
        ORA      SCRATCH1+1
        BNE      stretch_to_branch
        JMP      negated_branch
    instr_set_attr:
        JSR      attr_ptr_and_mask
        LDY      #$01
        LDA      SCRATCH1
        ORA      SCRATCH3
        STA      (SCRATCH2),Y
        DEY
        LDA      SCRATCH1+1
        ORA      SCRATCH3+1
        STA      (SCRATCH2),Y
        JMP      do_instruction
    instr_clear_attr:
        JSR      attr_ptr_and_mask
        LDY      #$01
        LDA      SCRATCH3
        EOR      #$FF
        AND      SCRATCH1
        STA      (SCRATCH2),Y
        DEY
        LDA      SCRATCH3+1
        EOR      #$FF
        AND      SCRATCH1+1
        STA      (SCRATCH2),Y
        JMP      do_instruction
    instr_insert_obj:
        JSR      remove_obj             ; remove_obj<OPERAND0>
        LDA      OPERAND0
        JSR      get_object_addr        ; obj_ptr = get_object_addr<OPERAND0>
        PSHW     SCRATCH2
        LDY      #OBJECT_PARENT_OFFSET
        LDA      OPERAND1
        STA      (SCRATCH2),Y           ; obj_ptr->parent = OPERAND1
        JSR      get_object_addr        ; dest_ptr = get_object_addr<OPERAND1>
        LDY      #OBJECT_CHILD_OFFSET   ; tmp = dest_ptr->child
        LDA      (SCRATCH2),Y
        TAX
        LDA      OPERAND0               ; dest_ptr->child = OPERAND0
        STA      (SCRATCH2),Y
        PULW     SCRATCH2
        TXA
        BEQ      .continue
        LDY      #OBJECT_SIBLING_OFFSET ; obj_ptr->sibling = tmp
        STA      (SCRATCH2),Y
    .continue:
        JMP      do_instruction
    instr_get_prop:
        JSR      get_property_ptr
    .loop:
        JSR      get_property_num
        CMP      OPERAND1
        BEQ      .found
        BCC      .get_default
        JSR      next_property
        JMP      .loop
    .get_default:
        LDY      #HEADER_OBJECT_TABLE_ADDR+1
        CLC
        LDA      (Z_HEADER_ADDR),Y
        ADC      Z_HEADER_ADDR
        STA      SCRATCH1
        DEY
        LDA      (Z_HEADER_ADDR),Y
        ADC      Z_HEADER_ADDR+1
        STA      SCRATCH1+1             ; table_ptr
        LDA      OPERAND1               ; SCRATCH2 <- table_ptr[2*OPERAND1]
        ASL
        TAY
        DEY
        LDA      (SCRATCH1),Y
        STA      SCRATCH2
        DEY
        LDA      (SCRATCH1),Y
        STA      SCRATCH2+1
        JMP      store_and_next
    .found:
        JSR      get_property_len
        INY
        CMP      #$00
        BEQ      .byte_prop
        CMP      #$01
        BEQ      .word_prop
        JSR      brk
    .word_prop:
        LDA      (SCRATCH2),Y
        STA      SCRATCH1+1
        INY
        LDA      (SCRATCH2),Y
        STA      SCRATCH1
        MOVW     SCRATCH1, SCRATCH2
        JMP      store_and_next
    .byte_prop:
        LDA      (SCRATCH2),Y
        STA      SCRATCH2
        LDA      #$00
        STA      SCRATCH2+1
        JMP      store_and_next
    instr_get_prop_addr:
        JSR      get_property_ptr
    .loop:
        JSR      get_property_num
        CMP      OPERAND1
        BEQ      .found
        BCS      .next
        JMP      store_zero_and_next
    .next:
        JSR      next_property
        JMP      .loop
    .found:
        INCW     SCRATCH2
        CLC
        TYA
        ADDAC    SCRATCH2
        SUBW     SCRATCH2, Z_HEADER_ADDR, SCRATCH2
        JMP      store_and_next
    instr_get_next_prop:
        JSR      get_property_ptr
        LDA      OPERAND1
        BEQ      .store
    .loop:
        JSR      get_property_num
        CMP      OPERAND1
        BEQ      .found
        BCS      .continue
        JMP      store_zero_and_next
    .continue:
        JSR      next_property
        JMP      .loop
    .store:
        JSR      get_property_num
        JMP      store_A_and_next
    .found:
        JSR      next_property
        JMP      .store
    instr_put_prop:
        JSR      get_property_ptr
    .loop:
        JSR      get_property_num
        CMP      OPERAND1
        BEQ      .found
        BCS      .continue
        JSR      brk
    .continue:
        JSR      next_property
        JMP      .loop
    .found:
        JSR      get_property_len
        INY
        CMP      #$00
        BEQ      .byte_property
        CMP      #$01
        BEQ      .word_property
        JSR      brk
    .word_property:
        LDA      OPERAND2+1
        STA      (SCRATCH2),Y
        INY
        LDA      OPERAND2
        STA      (SCRATCH2),Y
        JMP      do_instruction
    .byte_property:
        LDA      OPERAND2
        STA      (SCRATCH2),Y
        JMP      do_instruction
    attr_ptr_and_mask:
        LDA      OPERAND0           ; SCRATCH2 = get_object_addr<obj_num>
        JSR      get_object_addr
        LDA      OPERAND1           ; if (attr_num >= #$10) {
        CMP      #$10               ;  SCRATCH2 += 2; attr_num -= #$10
        BCC      .continue2         ; }
        SEC
        SBC      #$10
        INCW     SCRATCH2
        INCW     SCRATCH2
    .continue2:
        STA      SCRATCH1           ; SCRATCH1 = attr_num
        STOW     #$0001, SCRATCH3
        LDA      #$0F
        SEC
        SBC      SCRATCH1
        TAX
    .shift_loop:
        BEQ      .done_shift
        ASL      SCRATCH3
        ROL      SCRATCH3+1
        DEX
        JMP      .shift_loop
    .done_shift:
        LDY      #$00
        LDA      (SCRATCH2),Y
        STA      SCRATCH1+1
        INY
        LDA      (SCRATCH2),Y
        STA      SCRATCH1
        RTS
    get_property_ptr:
        LDA      OPERAND0
        JSR      get_object_addr
        LDY      #OBJECT_PROPS_OFFSET
        LDA      (SCRATCH2),Y
        STA      SCRATCH1+1
        INY
        LDA      (SCRATCH2),Y
        STA      SCRATCH1
        ADDW     SCRATCH1, Z_HEADER_ADDR, SCRATCH2
        LDY      #$00
        LDA      (SCRATCH2),Y
        ASL
        TAY
        INY
        RTS
    get_property_num:
        LDA      (SCRATCH2),Y
        AND      #$1F
        RTS
    get_property_len:
        LDA      (SCRATCH2),Y
        ROR
        ROR
        ROR
        ROR
        ROR
        AND      #$07
        RTS
    next_property:
        JSR      get_property_len
        TAX
    .loop:
        INY
        DEX
        BPL      .loop
        INY
        RTS
    get_object_addr:
        STA      SCRATCH2
        STOB     #$00, SCRATCH2+1
        LDA      SCRATCH2
        ASL      SCRATCH2
        ROL      SCRATCH2+1
        ASL      SCRATCH2
        ROL      SCRATCH2+1
        ASL      SCRATCH2
        ROL      SCRATCH2+1
        CLC
        ADC      SCRATCH2
        BCC      .continue
        INC      SCRATCH2+1
        CLC
    .continue:
        ADC      #FIRST_OBJECT_OFFSET
        STA      SCRATCH2
        BCC      .continue2
        INC      SCRATCH2+1
    .continue2:
        LDY      #HEADER_OBJECT_TABLE_ADDR+1
        LDA      (Z_HEADER_ADDR),Y
        CLC
        ADC      SCRATCH2
        STA      SCRATCH2
        DEY
        LDA      (Z_HEADER_ADDR),Y
        ADC      SCRATCH2+1
        ADC      Z_HEADER_ADDR+1
        STA      SCRATCH2+1
        RTS
*/
#include "zork.h"

/* Returns the address of object `obj`'s entry. */
static uint8_t *get_object_addr(register uint8_t obj) {
    uint16_t table = ((uint16_t)Z_HEADER_ADDR[HEADER_OBJECT_TABLE_ADDR] << 8)
        | Z_HEADER_ADDR[HEADER_OBJECT_TABLE_ADDR + 1];
    return Z_HEADER_ADDR + table + ((uint16_t)obj << 3) + obj
        + FIRST_OBJECT_OFFSET;
}

/* Takes object OPERAND0 out of its parent's list of children. */
static void remove_obj(void) {
    uint8_t obj = (uint8_t)OPERANDS[0];
    uint8_t *o = get_object_addr(obj);
    uint8_t parent = o[OBJECT_PARENT_OFFSET];
    if (parent == 0)
        return;
    uint8_t *p = get_object_addr(parent);
    uint8_t child = p[OBJECT_CHILD_OFFSET];
    if (child == obj) {
        p[OBJECT_CHILD_OFFSET] = o[OBJECT_SIBLING_OFFSET];
    } else {
        uint8_t *c;
        do {
            c = get_object_addr(child);
            child = c[OBJECT_SIBLING_OFFSET];
        } while (child != obj);
        c[OBJECT_SIBLING_OFFSET] = o[OBJECT_SIBLING_OFFSET];
    }
    o[OBJECT_PARENT_OFFSET] = 0;
    o[OBJECT_SIBLING_OFFSET] = 0;
}

void instr_remove_obj(void) {
    remove_obj();
}

/* Makes object OPERAND0 the first child of object OPERAND1. */
void instr_insert_obj(void) {
    remove_obj();
    uint8_t obj = (uint8_t)OPERANDS[0];
    uint8_t dest = (uint8_t)OPERANDS[1];
    uint8_t *o = get_object_addr(obj);
    o[OBJECT_PARENT_OFFSET] = dest;
    uint8_t *d = get_object_addr(dest);
    uint8_t first = d[OBJECT_CHILD_OFFSET];
    d[OBJECT_CHILD_OFFSET] = obj;
    if (first != 0)
        o[OBJECT_SIBLING_OFFSET] = first;
}

/* Prints object `obj`'s name. */
void print_obj(register uint8_t obj) {
    uint8_t *o = get_object_addr(obj);
    uint16_t props = ((uint16_t)o[OBJECT_PROPS_OFFSET] << 8)
        | o[OBJECT_PROPS_OFFSET + 1];
    load_address(props + 1);
    print_zstring();
}

/* Stores the sibling or child of object OPERAND0, and branches if
 * there is one. */
static void store_and_branch_on(register uint8_t related) {
    store_result(related);
    branch(related != 0);
}

void instr_get_sibling(void) {
    uint8_t *o = get_object_addr((uint8_t)OPERANDS[0]);
    store_and_branch_on(o[OBJECT_SIBLING_OFFSET]);
}

void instr_get_child(void) {
    uint8_t *o = get_object_addr((uint8_t)OPERANDS[0]);
    store_and_branch_on(o[OBJECT_CHILD_OFFSET]);
}

void instr_get_parent(void) {
    uint8_t *o = get_object_addr((uint8_t)OPERANDS[0]);
    store_result(o[OBJECT_PARENT_OFFSET]);
}

/* Branches if object OPERAND0's parent is object OPERAND1. */
void instr_jin(void) {
    uint8_t *o = get_object_addr((uint8_t)OPERANDS[0]);
    branch((uint8_t)OPERANDS[1] == o[OBJECT_PARENT_OFFSET]);
}

/* ---- Attributes ------------------------------------------------------ */

/* The word holding attribute OPERAND1 of object OPERAND0, and the
 * attribute's bit in it. Attribute 0 is the top bit of the first
 * word. */
static uint8_t *attr_ptr;
static uint16_t attr_mask;

/* Sets attr_ptr and attr_mask, and returns the word. */
static uint16_t attr_ptr_and_mask(void) {
    attr_ptr = get_object_addr((uint8_t)OPERANDS[0]);
    uint8_t attr = (uint8_t)OPERANDS[1];
    if (attr >= 0x10) {
        attr -= 0x10;
        attr_ptr += 2;
    }
    uint16_t mask = 1;
    for (uint8_t n = 0x0F - attr; n != 0; n--)
        mask <<= 1;
    attr_mask = mask;
    return ((uint16_t)attr_ptr[0] << 8) | attr_ptr[1];
}

static void set_attr_word(uint16_t word) {
    attr_ptr[1] = (uint8_t)word;
    attr_ptr[0] = (uint8_t)(word >> 8);
}

void instr_test_attr(void) {
    uint16_t word = attr_ptr_and_mask();
    branch((word & attr_mask) != 0);
}

void instr_set_attr(void) {
    uint16_t word = attr_ptr_and_mask();
    set_attr_word(word | attr_mask);
}

void instr_clear_attr(void) {
    uint16_t word = attr_ptr_and_mask();
    set_attr_word(word & ~attr_mask);
}

/* ---- Properties ------------------------------------------------------ */

/* The property table of the object a property instruction names. */
static uint8_t *props;

/* Sets props to object OPERAND0's property table, and returns the
 * offset of its first property, past the name. */
static uint8_t get_property_ptr(void) {
    uint8_t *o = get_object_addr((uint8_t)OPERANDS[0]);
    props = Z_HEADER_ADDR + (((uint16_t)o[OBJECT_PROPS_OFFSET] << 8)
        | o[OBJECT_PROPS_OFFSET + 1]);
    return (uint8_t)(props[0] << 1) + 1;
}

/* The number and the length less one of the property at offset `y`. */
static uint8_t get_property_num(register uint8_t y) {
    return props[y] & 0x1F;
}

static uint8_t get_property_len(register uint8_t y) {
    return props[y] >> 5;
}

/* Returns the offset of the property after the one at offset `y`. */
static uint8_t next_property(register uint8_t y) {
    return y + get_property_len(y) + 2;
}

/* Stores property OPERAND1 of object OPERAND0, or its default value
 * if the object doesn't have it. A property longer than two bytes is
 * an internal error. */
void instr_get_prop(void) {
    uint8_t want = (uint8_t)OPERANDS[1];
    uint8_t y = get_property_ptr();
    for (;;) {
        uint8_t num = get_property_num(y);
        if (num == want)
            break;
        if (num < want) {
            /* The default values are words at the start of the object
             * table, from property 1. */
            uint8_t *table = Z_HEADER_ADDR
                + (((uint16_t)Z_HEADER_ADDR[HEADER_OBJECT_TABLE_ADDR] << 8)
                   | Z_HEADER_ADDR[HEADER_OBJECT_TABLE_ADDR + 1]);
            uint8_t i = (uint8_t)(want << 1);
            store_result(((uint16_t)table[(uint8_t)(i - 2)] << 8)
                         | table[(uint8_t)(i - 1)]);
            return;
        }
        y = next_property(y);
    }
    uint8_t len = get_property_len(y);
    y++;
    if (len == 0)
        store_result(props[y]);
    else if (len == 1)
        store_result(((uint16_t)props[y] << 8) | props[y + 1]);
    else
        internal_error();
}

/* Stores the story address of property OPERAND1 of object OPERAND0's
 * value, or 0 if the object doesn't have it. */
void instr_get_prop_addr(void) {
    uint8_t want = (uint8_t)OPERANDS[1];
    uint8_t y = get_property_ptr();
    for (;;) {
        uint8_t num = get_property_num(y);
        if (num == want) {
            store_result((uint16_t)(props + y + 1)
                         - (uint16_t)Z_HEADER_ADDR);
            return;
        }
        if (num < want) {
            store_result(0);
            return;
        }
        y = next_property(y);
    }
}

/* Stores the number of the property after property OPERAND1 of object
 * OPERAND0, or of its first property when OPERAND1 is 0. Stores 0 if
 * the object doesn't have property OPERAND1. */
void instr_get_next_prop(void) {
    uint8_t want = (uint8_t)OPERANDS[1];
    uint8_t y = get_property_ptr();
    if (want != 0) {
        for (;;) {
            uint8_t num = get_property_num(y);
            if (num == want) {
                y = next_property(y);
                break;
            }
            if (num < want) {
                store_result(0);
                return;
            }
            y = next_property(y);
        }
    }
    store_result(get_property_num(y));
}

/* Stores the length of the property whose value is at story address
 * OPERAND0. */
void instr_get_prop_len(void) {
    uint8_t *p = Z_HEADER_ADDR + OPERANDS[0] - 1;
    store_result((uint8_t)((p[0] >> 5) + 1));
}

/* Sets property OPERAND1 of object OPERAND0 to OPERAND2. An object
 * without the property, or a property longer than two bytes, is an
 * internal error. */
void instr_put_prop(void) {
    uint8_t want = (uint8_t)OPERANDS[1];
    uint8_t y = get_property_ptr();
    for (;;) {
        uint8_t num = get_property_num(y);
        if (num == want)
            break;
        if (num < want)
            internal_error();
        y = next_property(y);
    }
    uint8_t len = get_property_len(y);
    y++;
    uint16_t v = OPERANDS[2];
    if (len == 0) {
        props[y] = (uint8_t)v;
    } else if (len == 1) {
        props[y] = (uint8_t)(v >> 8);
        props[y + 1] = (uint8_t)v;
    } else {
        internal_error();
    }
}
