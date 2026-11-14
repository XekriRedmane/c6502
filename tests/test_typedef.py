"""Tests for `typedef` (C99 §6.7.7).

The parser puts each typedef name's type in place of its uses, so the
AST never names a typedef. Coverage:

  * A typedef name in declarations, casts, sizeof, parameters, struct
    members, and `for` initializers.
  * The "lexer hack": `T * x;` declares x when T names a type and
    multiplies when it doesn't, and a declaration of T in an inner
    scope (an object, a parameter, a `for` variable) hides the typedef
    name until that scope ends.
  * Typedefs of pointers, arrays (qualified ones too), functions, and
    function pointers; anonymous structs and unions.
  * The forward declaration that stands in for a tag a typedef
    introduces.
  * The rejected forms: an initializer, two kinds of symbol in one
    scope, conflicting redefinitions, a typedef in a parameter or a
    `for` initializer, a function definition typed by a typedef, and
    a typedef whose struct tag an inner declaration hides.
  * Programs using typedefs run with and without `--optimize`.
"""
from __future__ import annotations

import shutil
import unittest

import c99_ast
from parser import ParserError, parse
from sim.harness import build_sim


def _decls(src: str) -> list:
    return parse(src).declaration


def _block_items(src: str) -> list:
    """The block items of the body of the last function in `src`."""
    fn = [d for d in _decls(src) if isinstance(d, c99_ast.FunctionDecl)][-1]
    return fn.function_decl.body.block_item


def _var(decl) -> c99_ast.Type_var_decl:
    return decl.declaration.var_decl


class TestTypedefDeclarations(unittest.TestCase):
    def test_typedef_leaves_no_declaration(self) -> None:
        decls = _decls("typedef int T; T x;")
        self.assertEqual(len(decls), 1)
        vd = decls[0].var_decl
        self.assertEqual(vd.name, "x")
        self.assertEqual(vd.data_type, c99_ast.Int())

    def test_typedef_of_typedef(self) -> None:
        decls = _decls(
            "typedef unsigned char u8; typedef u8 byte; byte b;"
        )
        self.assertEqual(decls[0].var_decl.data_type, c99_ast.UChar())

    def test_several_declarators(self) -> None:
        decls = _decls("typedef int T, *PT, AT[2]; T a; PT p; AT q;")
        self.assertEqual(
            [d.var_decl.data_type for d in decls],
            [
                c99_ast.Int(),
                c99_ast.Pointer(referenced_type=c99_ast.Int()),
                c99_ast.Array(element_type=c99_ast.Int(), size=2),
            ],
        )

    def test_typedef_name_in_its_own_declaration(self) -> None:
        # The scope of T begins right after its declarator.
        decls = _decls("typedef int T, *PT[sizeof(T)]; PT x;")
        self.assertEqual(
            decls[0].var_decl.data_type,
            c99_ast.Array(
                element_type=c99_ast.Pointer(referenced_type=c99_ast.Int()),
                size=2,
            ),
        )

    def test_pointer_declarator_on_typedef(self) -> None:
        decls = _decls("typedef long L; L *p, **q;")
        self.assertEqual(
            decls[0].var_decl.data_type,
            c99_ast.Pointer(referenced_type=c99_ast.Long()),
        )
        self.assertEqual(
            decls[1].var_decl.data_type,
            c99_ast.Pointer(
                referenced_type=c99_ast.Pointer(referenced_type=c99_ast.Long())
            ),
        )

    def test_qualifier_on_typedef(self) -> None:
        decls = _decls("typedef int T; const T c = 1; T const d = 2;")
        for d in decls:
            self.assertEqual(
                d.var_decl.data_type,
                c99_ast.Const(referenced_type=c99_ast.Int()),
            )

    def test_qualifier_already_in_typedef(self) -> None:
        # §6.7.3.4: a repeated qualifier is the same as one.
        decls = _decls("typedef const int CI; const CI c = 1;")
        self.assertEqual(
            decls[0].var_decl.data_type,
            c99_ast.Const(referenced_type=c99_ast.Int()),
        )

    def test_qualified_array_typedef_qualifies_elements(self) -> None:
        # §6.7.3.8: `const A` with A an array type is an array of
        # const elements, the same as `const int x[3]`.
        decls = _decls(
            "typedef int A[3]; const A x = {1, 2, 3}; const int y[3];"
        )
        self.assertEqual(decls[0].var_decl.data_type, decls[1].var_decl.data_type)

    def test_struct_typedef(self) -> None:
        decls = _decls("typedef struct node node_t; node_t *p;")
        self.assertIsInstance(decls[0], c99_ast.StructDecl)
        self.assertEqual(decls[0].struct_decl.tag, "node")
        self.assertEqual(decls[0].struct_decl.members, [])
        self.assertEqual(
            decls[1].var_decl.data_type,
            c99_ast.Pointer(referenced_type=c99_ast.Structure(tag="node")),
        )

    def test_struct_typedef_with_body(self) -> None:
        decls = _decls("typedef struct pt { int x; } pt_t; pt_t p;")
        self.assertEqual(len(decls), 2)
        self.assertEqual(decls[0].struct_decl.tag, "pt")
        self.assertEqual(len(decls[0].struct_decl.members), 1)
        self.assertEqual(decls[1].var_decl.data_type, c99_ast.Structure(tag="pt"))

    def test_anonymous_structs_are_distinct(self) -> None:
        decls = _decls(
            "typedef struct { int x; } A; typedef struct { int x; } B;"
            "A a; B b; union { char c; int i; } u;"
        )
        structs = [d for d in decls if isinstance(d, c99_ast.StructDecl)]
        tags = [s.struct_decl.tag for s in structs]
        self.assertEqual(len(set(tags)), 3)
        self.assertTrue(structs[2].struct_decl.is_union)
        a, b, u = [d.var_decl for d in decls if isinstance(d, c99_ast.VarDecl)]
        self.assertEqual(a.data_type, c99_ast.Structure(tag=tags[0]))
        self.assertEqual(b.data_type, c99_ast.Structure(tag=tags[1]))
        self.assertEqual(u.data_type, c99_ast.Union(tag=tags[2]))

    def test_function_typedef_declares_function(self) -> None:
        decls = _decls("typedef int F(int, long); F f;")
        fd = decls[0].function_decl
        self.assertEqual(fd.name, "f")
        self.assertEqual(fd.params, [None, None])
        self.assertEqual(fd.param_registers, ["", ""])
        self.assertEqual(
            fd.data_type,
            c99_ast.FunType(
                params=[c99_ast.Int(), c99_ast.Long()], ret=c99_ast.Int(),
            ),
        )

    def test_parameter_of_function_type_is_adjusted(self) -> None:
        # §6.7.5.3.8: a parameter of function type is a pointer.
        decls = _decls("typedef int F(int); int apply(F f, F);")
        fun = c99_ast.FunType(params=[c99_ast.Int()], ret=c99_ast.Int())
        self.assertEqual(
            decls[0].function_decl.data_type.params,
            [c99_ast.Pointer(referenced_type=fun)] * 2,
        )

    def test_parameter_of_array_type_is_adjusted(self) -> None:
        decls = _decls("typedef int A[3]; int sum(A a, A);")
        self.assertEqual(
            decls[0].function_decl.data_type.params,
            [c99_ast.Pointer(referenced_type=c99_ast.Int())] * 2,
        )

    def test_cast_and_sizeof(self) -> None:
        items = _block_items(
            "typedef long L; int main(void) { return (L)1 + sizeof(L); }"
        )
        ret = items[0].statement.exp
        self.assertEqual(ret.left.target_type, c99_ast.Long())
        self.assertEqual(ret.right.target_type, c99_ast.Long())

    def test_struct_member_of_typedef_type(self) -> None:
        decls = _decls("typedef unsigned char u8; struct s { u8 a; u8 *b; };")
        members = decls[0].struct_decl.members
        self.assertEqual(members[0].data_type, c99_ast.UChar())
        self.assertEqual(
            members[1].data_type,
            c99_ast.Pointer(referenced_type=c99_ast.UChar()),
        )


class TestTypedefScopes(unittest.TestCase):
    def test_star_declares_when_typedef(self) -> None:
        items = _block_items(
            "typedef int T; int main(void) { T * x; return 0; }"
        )
        self.assertEqual(
            _var(items[0]).data_type,
            c99_ast.Pointer(referenced_type=c99_ast.Int()),
        )

    def test_star_multiplies_when_hidden(self) -> None:
        items = _block_items(
            "typedef int T; int main(void) { int T = 2, x = 3;"
            " T * x; return 0; }"
        )
        self.assertEqual(_var(items[0]).name, "T")
        self.assertIsInstance(items[2].statement.exp, c99_ast.Binary)
        self.assertIsInstance(items[2].statement.exp.op, c99_ast.Multiply)

    def test_object_named_after_typedef(self) -> None:
        # After a type specifier, the name is the declarator.
        items = _block_items(
            "typedef int T; int main(void) { T T = 4; return T; }"
        )
        self.assertEqual(_var(items[0]).name, "T")
        self.assertEqual(_var(items[0]).data_type, c99_ast.Int())

    def test_hiding_ends_with_block(self) -> None:
        items = _block_items(
            "typedef int T; int main(void) {"
            " { long T = 1; T = T * 2; } T y = 0; return y; }"
        )
        self.assertEqual(_var(items[1]).name, "y")
        self.assertEqual(_var(items[1]).data_type, c99_ast.Int())

    def test_parameter_hides_typedef_in_body(self) -> None:
        items = _block_items(
            "typedef int T; int f(int T) { return T * 2; }"
        )
        self.assertIsInstance(items[0].statement.exp, c99_ast.Binary)

    def test_parameter_hides_only_its_own_function(self) -> None:
        items = _block_items(
            "typedef int T; int f(int T) { return T; }"
            " int g(void) { T x = 1; return x; }"
        )
        self.assertEqual(_var(items[0]).data_type, c99_ast.Int())

    def test_nested_function_suffix_parameters(self) -> None:
        # f's parameter is a; b belongs to the returned pointer's type.
        items = _block_items(
            "typedef int a; typedef int b;"
            " int (*f(int a))(int b) { b x = a; return 0; }"
        )
        self.assertEqual(_var(items[0]).name, "x")

    def test_block_scope_typedef(self) -> None:
        items = _block_items(
            "int main(void) { typedef unsigned char B; B b = 1;"
            " { typedef long B; B c = 2; } return b; }"
        )
        self.assertEqual(_var(items[0]).data_type, c99_ast.UChar())
        inner = items[1].statement.block.block_item
        self.assertEqual(_var(inner[0]).data_type, c99_ast.Long())

    def test_typedef_name_after_for_that_hides_it(self) -> None:
        # The token after the loop is lexed before the loop ends.
        items = _block_items(
            "typedef int T; int main(void) { int s = 0;"
            " for (int T = 0; T < 3; T++) s += T; T y = s; return y; }"
        )
        self.assertEqual(_var(items[2]).name, "y")

    def test_for_initializer_of_typedef_type(self) -> None:
        items = _block_items(
            "typedef long I; int main(void) {"
            " for (I i = 0; i < 2; i++) ; return 0; }"
        )
        init = items[0].statement.init
        self.assertEqual(init.var_decls[0].data_type, c99_ast.Long())

    def test_member_named_after_typedef(self) -> None:
        items = _block_items(
            "typedef int T; struct s { T T; };"
            " int main(void) { struct s v; v.T = 1; return v.T; }"
        )
        self.assertEqual(items[1].statement.exp.lval.member, "T")

    def test_tag_named_after_typedef(self) -> None:
        decls = _decls("typedef int T; struct T { T a; }; struct T t;")
        self.assertEqual(decls[0].struct_decl.tag, "T")
        self.assertEqual(decls[1].var_decl.data_type, c99_ast.Structure(tag="T"))

    def test_typedef_redeclared_with_same_type(self) -> None:
        decls = _decls("typedef int T; typedef int T; T x;")
        self.assertEqual(decls[0].var_decl.data_type, c99_ast.Int())


class TestTypedefErrors(unittest.TestCase):
    def _rejects(self, src: str, message: str) -> None:
        with self.assertRaises(ParserError) as cm:
            parse(src)
        self.assertIn(message, str(cm.exception))

    def test_initializer(self) -> None:
        self._rejects("typedef int T = 3;", "can't have an initializer")

    def test_typedef_and_object_in_one_scope(self) -> None:
        self._rejects("typedef int T; int T;", "both as a typedef name")
        self._rejects("int T; typedef int T;", "both as a typedef name")
        self._rejects(
            "typedef int T; int T(void) { return 0; }",
            "both as a typedef name",
        )

    def test_parameter_and_typedef_in_body(self) -> None:
        self._rejects(
            "int f(int a) { typedef int a; return 0; }",
            "both as a typedef name",
        )

    def test_conflicting_redefinition(self) -> None:
        self._rejects(
            "typedef int T; typedef long T;", "again with a different type",
        )

    def test_typedef_combined_with_type_specifier(self) -> None:
        self._rejects("typedef int T; T unsigned x;", "cannot combine")

    def test_typedef_parameter(self) -> None:
        self._rejects("int f(typedef int x);", "'typedef' isn't permitted")

    def test_typedef_in_for_initializer(self) -> None:
        self._rejects(
            "int main(void) { for (typedef int T;;) ; }",
            "can't declare a typedef",
        )

    def test_typedef_without_name(self) -> None:
        self._rejects("typedef struct foo;", "declares no name")

    def test_function_definition_declared_typedef(self) -> None:
        self._rejects(
            "typedef int f(void) { return 0; }", "'typedef' isn't permitted",
        )

    def test_function_definition_typed_by_typedef(self) -> None:
        self._rejects(
            "typedef int F(void); F f { return 0; }",
            "its type can't come from a typedef",
        )

    def test_attribute_on_typedef(self) -> None:
        self._rejects(
            "[[c6502::zp_abi]] typedef int F(void);",
            "doesn't apply to typedef 'F'",
        )

    def test_inner_tag_hides_typedef_tag(self) -> None:
        self._rejects(
            "struct foo { int a; }; typedef struct foo T;"
            " int main(void) { struct foo { long z; }; T t; return 0; }",
            "declared in an outer scope",
        )

    def test_typedef_name_out_of_scope(self) -> None:
        from lark.exceptions import UnexpectedInput
        with self.assertRaises(UnexpectedInput):
            parse("int main(void) { { typedef int T; } T x; return 0; }")


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestTypedefPrograms(unittest.TestCase):
    """Run each program with and without `--optimize`."""

    def _returns(self, src: str, expected: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run(
                    max_cycles=2_000_000,
                )
                self.assertFalse(result.timed_out)
                value = result.return_int()
                if value & 0x8000:
                    value -= 0x10000
                self.assertEqual(value, expected)

    def test_struct_point(self) -> None:
        self._returns(
            "typedef unsigned char u8;"
            "typedef struct { u8 x; int y; } point;"
            "typedef point *ppoint;"
            "int add(ppoint p) { return p->x + p->y; }"
            "int main(void) { point pt; ppoint pp = &pt;"
            " pt.x = 3; pt.y = 4; return add(pp) + sizeof(point); }",
            10,
        )

    def test_hiding(self) -> None:
        self._returns(
            "typedef int T;"
            "int f(int T) { return T * 2; }"
            "int main(void) { int s = 0;"
            " for (int T = 0; T < 3; T++) s += T;"
            " { int T = 10; s += T * f(1); }"
            " T y = s; return y; }",
            23,
        )

    def test_function_and_function_pointer_typedefs(self) -> None:
        self._returns(
            "typedef int F(int); typedef int (*FP)(int);"
            "F inc; int inc(int x) { return x + 1; }"
            "int apply(F *f, FP g, int v) { return g(f(v)); }"
            "int main(void) { return apply(inc, inc, 4); }",
            6,
        )

    def test_array_typedef(self) -> None:
        self._returns(
            "typedef int Row[3]; typedef Row Grid[2];"
            "int sum(Row r) { return r[0] + r[1] + r[2]; }"
            "int main(void) { Grid g = {{1, 2, 3}, {4, 5, 6}};"
            " return sum(g[0]) * 10 + sum(g[1]); }",
            75,
        )

    def test_linked_list_with_forward_tag(self) -> None:
        # The typedef introduces `struct node` at file scope, so the
        # pointer in `first`, declared before the struct's definition,
        # refers to the same tag as `head` does. If it introduced a
        # new tag in `first`'s block, `return p;` wouldn't type-check.
        self._returns(
            "typedef struct node node_t;"
            "node_t *head;"
            "node_t *first(void) { node_t *p = head; return p; }"
            "struct node { int v; node_t *next; };"
            "int sum(void) { int s = 0; node_t *p;"
            " for (p = first(); p; p = p->next) s += p->v; return s; }"
            "int main(void) { node_t a, b; a.v = 2; b.v = 5;"
            " a.next = &b; b.next = 0; head = &a; return sum(); }",
            7,
        )

    def test_stdint_types(self) -> None:
        self._returns(
            "#include <stdint.h>\n"
            "uint8_t lo(uint16_t w) { return (uint8_t)w; }"
            "int main(void) { int32_t big = 70000; uint8_t uint8 = lo(0x1234);"
            " return (int)(big / 1000) + uint8 + sizeof(int64_t); }",
            70 + 0x34 + 8,
        )


if __name__ == "__main__":
    unittest.main()
