# Dead-store elimination

`eliminate_dead_stores` in
[passes/optimization/dead_store_elimination.py](../../passes/optimization/dead_store_elimination.py)
removes TAC instructions whose result nothing reads. It runs on
SSA-form TAC in the optimizer's fixed point, right after copy
propagation, and only under `--optimize`.

In SSA form a renamed variable has one definition, so the definition
is dead exactly when the name appears nowhere in the function as an
operand. The pass collects every name that's read, drops each pure
instruction whose destination isn't among them, and repeats until a
round drops nothing, since dropping one instruction can leave its
operands unread. A call whose result is unread keeps the call and
loses only its destination. Each TAC value that reaches `tac_to_asm`
becomes at least a load and a store per byte, so a value dropped here
never takes code or a zero-page byte.

## Example

```c
#include <stdint.h>
uint8_t poll(void);
static uint8_t last;
void step(uint8_t x) {
    uint8_t twice = x + x;
    uint8_t status = poll();
    last = x;
}
```

### Before

```
ZeroExtend(@0.x, %0.1)
Copy(%0.1, %1.1)
Binary(+, %0.1, %0.1, %2.1)
Truncate(%2.1, %3.1)
Copy(%3.1, @1.twice.1)
FunctionCall(poll, [], %4.1)
Copy(%4.1, @2.status.1)
Copy(@0.x, last)
Ret(None)
```

### After

```
FunctionCall(poll, [], None)
Copy(@0.x, last)
Ret(None)
```

The whole computation of `twice` goes in one call of the pass. The
first round drops the three copies that nothing reads, and each later
round drops the instruction whose only reader went in the round
before. The store to the static `last` stays.

## When it applies

An instruction is dropped when its destination is a name SSA
construction renamed, that name is read nowhere in the function, and
the instruction is one of these.

- `Copy`, `Unary`, `Binary`, or `Phi`.
- `SignExtend`, `ZeroExtend`, `Truncate`, or one of the six
  floating-point conversions.
- `GetAddress`.
- `Load` or `IndexedLoad` that isn't volatile. A read of ordinary RAM
  on the 6502 has no effect other than the value it returns, so a
  load through any pointer can go.

A `FunctionCall` or `IndirectCall` whose destination meets the same
test is kept with its destination set to `None`. The callee can
write memory or do I/O, so only the result capture goes.

## What it leaves alone

- `Store`, `IndexedStore`, `IndexedSymbolStore`, and
  `IndirectIndexedStore`, which write memory other code can read.
- `Ret`, labels, and jumps.
- A volatile `Load`, `IndexedLoad`, `IndexedConstLoad`, or
  `IndirectIndexedLoad`, because C99 §6.7.3 makes each access to a
  volatile object a side effect. `(void)*p` with `volatile uint8_t
  *p` keeps its `Load` even though nothing reads the result, and the
  final code reads the byte with `CMP (zp),Y`.
- A write to a static, to an address-taken local, or to any other
  name SSA didn't rename, since another function or a pointer can
  read it.
- A non-volatile `IndexedConstLoad` or `IndirectIndexedLoad` whose
  result is unread. Neither is on the pass's list of pure
  instructions, so it's kept, although a plain `Load` of the same
  byte would be dropped.

## Related

[Copy propagation](copy_propagation.md) leaves the copies this pass
removes. [Asm dead-store elimination](asm_dead_store.md) removes
stores to memory in the asm, which this pass doesn't touch.
