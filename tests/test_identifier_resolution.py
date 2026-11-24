import unittest

import c99_ast
from parser import parse
from passes.identifier_resolution import Linkage, Resolver


class TestMakeUnique(unittest.TestCase):
    def test_format_is_at_counter_dot_original(self):
        r = Resolver()
        self.assertEqual(r.make_unique("x"), "@0.x")
        self.assertEqual(r.make_unique("y"), "@1.y")

    def test_counter_is_global_across_calls(self):
        # Even the same original name gets a fresh number each time,
        # so names stay program-unique rather than scope-unique.
        r = Resolver()
        self.assertEqual(r.make_unique("a"), "@0.a")
        self.assertEqual(r.make_unique("a"), "@1.a")


class TestLinkageTracking(unittest.TestCase):
    """The resolver tags every visible identifier with its C99 §6.2.2
    linkage kind. Today: function declarations and definitions are
    EXTERNAL; block-scope variable declarations are NONE. The pass
    uses linkage to decide whether to rename (NONE → fresh `@<N>.<orig>`,
    INTERNAL/EXTERNAL → keep source spelling), so reading the resolver's
    internal tables is the cleanest way to assert linkage is being
    recorded — until the type-checking pass lands and consumes it
    via a public surface."""

    def test_function_decl_recorded_as_external(self):
        # `int main(...) { ... }` is a file-scope function definition;
        # the block-scope `int foo(void);` declaration doesn't appear
        # in the file-scope table at all (it lives in the body's local
        # scope dict, which goes out of scope when the body is done).
        # So we expect only `main` here, with EXTERNAL linkage.
        prog = parse("int main(void) { int foo(void); return foo(); }")
        r = Resolver()
        r.resolve_program(prog)
        self.assertEqual(r._file_scope["main"], ("main", Linkage.EXTERNAL))
        self.assertNotIn("foo", r._file_scope)

    def test_top_level_function_definition_recorded_as_external(self):
        # `int foo(void) { ... } int main(void) { ... }` — both
        # registered (with EXTERNAL linkage) before any body is
        # walked, so each can call the other regardless of source
        # order.
        prog = parse(
            "int foo(void) { return 1; } "
            "int main(void) { return 0; }"
        )
        r = Resolver()
        r.resolve_program(prog)
        self.assertEqual(r._file_scope, {
            "foo": ("foo", Linkage.EXTERNAL),
            "main": ("main", Linkage.EXTERNAL),
        })

    def test_static_at_file_scope_is_internal(self):
        # `static int foo(void);` at file scope → INTERNAL linkage.
        prog = parse("static int foo(void) { return 0; }")
        r = Resolver()
        r.resolve_program(prog)
        self.assertEqual(r._file_scope["foo"], ("foo", Linkage.INTERNAL))

    def test_extern_inherits_prior_linkage(self):
        # File-scope `static int foo(void);` followed by `extern int
        # foo(void) { ... }` — the extern decl takes the prior visible
        # decl's linkage, so the definition is also INTERNAL.
        prog = parse(
            "static int foo(void); "
            "extern int foo(void) { return 0; }"
        )
        r = Resolver()
        r.resolve_program(prog)
        self.assertEqual(r._file_scope["foo"], ("foo", Linkage.INTERNAL))

    def test_file_scope_variable_recorded(self):
        # `int g;` at file scope → EXTERNAL linkage; `static int s;`
        # → INTERNAL.
        prog = parse("int g; static int s; int main(void) { return 0; }")
        r = Resolver()
        r.resolve_program(prog)
        self.assertEqual(r._file_scope["g"], ("g", Linkage.EXTERNAL))
        self.assertEqual(r._file_scope["s"], ("s", Linkage.INTERNAL))


    def test_block_scope_variable_recorded_as_none_linkage(self):
        # `int a;` at block scope — NONE linkage, gets renamed.
        # The scope dict goes out of scope when resolve_block returns,
        # so we exercise resolve_var_decl directly to inspect the
        # tagged entry.
        r = Resolver()
        scope: dict = {}
        r.resolve_var_decl(
            c99_ast.Type_var_decl(name="a", init=None, data_type=c99_ast.Int()),
            scope,
            Linkage.NONE,
        )
        self.assertEqual(scope["a"], ("@0.a", True, Linkage.NONE))

    def test_external_linkage_var_keeps_source_spelling(self):
        # Forcing a synthetic EXTERNAL var-decl through the resolver
        # exercises the future-extern path: the unique-counter is
        # NOT bumped, and the resolved name equals the source name.
        r = Resolver()
        scope: dict = {}
        result = r.resolve_var_decl(
            c99_ast.Type_var_decl(name="g", init=None, data_type=c99_ast.Int()),
            scope,
            Linkage.EXTERNAL,
        )
        self.assertEqual(result.name, "g")
        self.assertEqual(scope["g"], ("g", True, Linkage.EXTERNAL))
        # Counter untouched — the next NONE-linkage var still gets
        # `@0.<name>`, not `@1.<name>`.
        self.assertEqual(r.make_unique("x"), "@0.x")


class TestErrors(unittest.TestCase):
    def test_unknown_exp_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_exp,), {})
        with self.assertRaises(TypeError):
            Resolver().resolve_exp(stub(), {})

    def test_unknown_statement_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_statement,), {})
        with self.assertRaises(TypeError):
            Resolver().resolve_statement(stub(), {})

    def test_unknown_block_item_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_block_item,), {})
        with self.assertRaises(TypeError):
            Resolver().resolve_block_item(stub(), {})


if __name__ == "__main__":
    unittest.main()
