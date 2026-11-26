# A copy of a returned pointer kept in its zero-page bytes

`apply_ret_slot_coalescing` in
[cpp/src/passes/ret_slot_coalescing.h](../../cpp/src/passes/ret_slot_coalescing.h)
renames a pair of a function's zero-page locals to `F@ret`, the
zero-page pointer in which a `static` function `F` returns its result
([a pointer returned in zero page](return_in_zero_page.md)), when the
pair is copied from `F@ret` and the two are never live with different
values. It's the rule of [a parameter's copy in the parameter's
slot](slot_coalescing.md), applied to `F@ret`, and it runs once,
right after that pass.

A caller that keeps the pointer past the next call of `F`, or changes
it, copies `F@ret` into a local pair after each call, 8 bytes. With
the pair renamed, each copy is a load and a store of the same byte,
which the next peephole fixed point drops, and a `(zp),Y` access
through the pair goes through `F@ret`.

The rename adds writes of `F@ret` to the function, `G`, so `G` must
already be one whose calls may write it: `G` calls `F`. A call that
`G` makes while the pair is live may write `F@ret` when it's a call of
`F` or of a function of the unit that calls `F`, and then the two
interfere. A call that leaves the unit could reach `F` only through
one of the unit's global or address-taken functions. One of those
that leads to `G` would make a cycle through `G`'s call, which the
linker rejects, since `G` is off every cycle and makes no
`save_hardware_stack` call. So such a call may write `F@ret` only when
some entry point calls `F` without leading to `G`. The module
docstring has the rest of the conditions.

## Example

From `display_message` in `examples/ali_baba/text.c`, which skips
`index` messages, each a group of length-prefixed strings ended by a
0, with `next_string`:

```c
for (; index != 0; index--) {
    while (*msg != 0)
        msg = next_string(msg);
    msg++;
}
display_message_loop(msg);
```

### Before

```
.display_message@rot@1:
   LDX   __local_display_message__msg_1
   LDA   __local_display_message__msg_0
   JSR   next_string
   LDA   next_string@ret
   STA   __local_display_message__msg_0
   LDA   next_string@ret+1
   STA   __local_display_message__msg_1
.loop@4_continue:
   LDY   #$00
   LDA   (__local_display_message__msg_0),Y
   BNE   .display_message@rot@1
   INC   __local_display_message__msg_0
   BNE   .inc_done@1
   INC   __local_display_message__msg_1
```

### After

```
.display_message@rot@1:
   LDX   next_string@ret+1
   LDA   next_string@ret
   JSR   next_string
.loop@4_continue:
   LDY   #$00
   LDA   (next_string@ret),Y
   BNE   .display_message@rot@1
   INC   next_string@ret
   BNE   .inc_done@1
   INC   next_string@ret+1
```

`display_message_loop` keeps its pointer in `next_string@ret` across
calls of `print_bottom_centered` and other functions that call
`hrcg_cout` in another unit. The unit's entry points that call
`next_string` are `display_message` and `display_message_loop`, and
both lead to `display_message_loop`, so `hrcg_cout` can't reach
`next_string` without a cycle. The pass saves 16 bytes in text.c, 24
in Ali Baba's hrcg.c and 8 in its input.c, and 16 in Lode Runner's
sprites.c.
