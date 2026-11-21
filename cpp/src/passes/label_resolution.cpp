/*
The label resolution pass, c99_ast to c99_ast
(passes/label_resolution.py).

It checks and renames the labeled statements (`label: stmt`, C99
§6.8.1) and the `goto label;` statements (§6.8.6) of each function.
Every label a function declares must be unique in the function, since
§6.2.3 gives labels a namespace of their own but forbids two of one name
in a function, and every `goto` must name a label declared somewhere in
its function. Both checks are per function. A label one function
declares is invisible from another, and a `goto` can't leave its
function (§6.8.6.1, "The identifier in a goto statement shall name a
label located somewhere in the enclosing function").

§6.8.6.1 also forbids jumping into the scope of an identifier with a
variably modified type. c6502 has no variable-length arrays, so there is
nothing to enforce.

Each label becomes `.<funcname>@<orig>`. The leading `.` makes it a
dasm local label, valid only between the function's `SUBROUTINE`
directive and the next one, which gives each function a namespace of
labels, so two functions may each have a label `foo`. The `@`, which
can't appear in a C identifier, keeps the label apart from every name
the program writes. The labels the translator makes contain `@` too
(`.if_end@0`), and the two forms stay apart because the part after the
`@` is a C identifier here and digits there. The function's name isn't
needed for correctness, since the local labels are already scoped to the
function, but it makes the emitted code name each label by its
function, which helps a reader of the output.

The errors (`LabelResolutionError`) are a label declared twice in one
function and a `goto` to a label its function doesn't declare. Each
function is checked whole before the next, and its labels are collected
before any `goto` is checked, so a duplicate label is reported before an
undefined target in the same function.

The Python pass builds a new program and keeps every field. This one
rewrites the program it is given, which it takes by value. It runs after
identifier_resolution, and the order is free, since labels and
variables share no namespace.
*/
#include "passes/label_resolution.h"

#include <string>
#include <unordered_map>
#include <utility>

#include "errors.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::c99_ast;

using Labels = std::unordered_map<std::string, std::string>;

[[noreturn]] void fail(const std::string& message) {
  throw CompileError("LabelResolutionError", message);
}

// Collects every labeled statement of a function, giving each its new
// name and rejecting a duplicate. Declarations, the for header, and the
// other statements hold no labeled statement. The case and default
// labels are the translator's and are left to loop_labeling.
void collect_statement(const ir::Type_statement& stmt, const std::string& fn_name,
                       Labels& labels);

void collect_block(const ir::Type_block& block, const std::string& fn_name, Labels& labels) {
  for (const auto& item : std::get<ir::Block>(block).block_item) {
    if (const auto* s = std::get_if<ir::S>(&item)) {
      collect_statement(*s->statement, fn_name, labels);
    }
  }
}

void collect_statement(const ir::Type_statement& stmt, const std::string& fn_name,
                       Labels& labels) {
  switch (ir::kind(stmt)) {
    case ir::Kind_statement::LabeledStmt: {
      // The labeled statement may be another (`a: b: ;`), an `if`
      // with labels in its branches, and so on.
      const auto& l = std::get<ir::LabeledStmt>(stmt);
      if (labels.count(l.label) != 0) {
        fail("duplicate label " + pyrepr::string(l.label));
      }
      labels[l.label] = "." + fn_name + "@" + l.label;
      collect_statement(*l.statement, fn_name, labels);
      return;
    }
    case ir::Kind_statement::IfStmt: {
      const auto& i = std::get<ir::IfStmt>(stmt);
      collect_statement(*i.then_clause, fn_name, labels);
      if (i.else_clause) {
        collect_statement(**i.else_clause, fn_name, labels);
      }
      return;
    }
    case ir::Kind_statement::Compound:
      // The labels in a block belong to the enclosing function.
      collect_block(*std::get<ir::Compound>(stmt).block, fn_name, labels);
      return;
    case ir::Kind_statement::WhileStmt:
      collect_statement(*std::get<ir::WhileStmt>(stmt).body, fn_name, labels);
      return;
    case ir::Kind_statement::DoWhileStmt:
      collect_statement(*std::get<ir::DoWhileStmt>(stmt).body, fn_name, labels);
      return;
    case ir::Kind_statement::ForStmt:
      collect_statement(*std::get<ir::ForStmt>(stmt).body, fn_name, labels);
      return;
    case ir::Kind_statement::SwitchStmt:
      // A switch's body may hold labels and gotos to them as any
      // statement may.
      collect_statement(*std::get<ir::SwitchStmt>(stmt).body, fn_name, labels);
      return;
    case ir::Kind_statement::CaseStmt:
      collect_statement(*std::get<ir::CaseStmt>(stmt).body, fn_name, labels);
      return;
    case ir::Kind_statement::DefaultStmt:
      collect_statement(*std::get<ir::DefaultStmt>(stmt).body, fn_name, labels);
      return;
    default:
      return;
  }
}

// Renames each label and each `goto`'s target, which must be a label of
// the function.
void rewrite_statement(ir::Type_statement& stmt, const Labels& labels);

void rewrite_block(ir::Type_block& block, const Labels& labels) {
  for (auto& item : std::get<ir::Block>(block).block_item) {
    if (auto* s = std::get_if<ir::S>(&item)) {
      rewrite_statement(*s->statement, labels);
    }
  }
}

void rewrite_statement(ir::Type_statement& stmt, const Labels& labels) {
  switch (ir::kind(stmt)) {
    case ir::Kind_statement::Goto: {
      auto& g = std::get<ir::Goto>(stmt);
      auto it = labels.find(g.label);
      if (it == labels.end()) {
        fail("goto to undefined label " + pyrepr::string(g.label));
      }
      g.label = it->second;
      return;
    }
    case ir::Kind_statement::LabeledStmt: {
      // The collection walk named every label, so the lookup finds it.
      auto& l = std::get<ir::LabeledStmt>(stmt);
      l.label = labels.at(l.label);
      rewrite_statement(*l.statement, labels);
      return;
    }
    case ir::Kind_statement::IfStmt: {
      auto& i = std::get<ir::IfStmt>(stmt);
      rewrite_statement(*i.then_clause, labels);
      if (i.else_clause) {
        rewrite_statement(**i.else_clause, labels);
      }
      return;
    }
    case ir::Kind_statement::Compound:
      rewrite_block(*std::get<ir::Compound>(stmt).block, labels);
      return;
    case ir::Kind_statement::WhileStmt:
      rewrite_statement(*std::get<ir::WhileStmt>(stmt).body, labels);
      return;
    case ir::Kind_statement::DoWhileStmt:
      rewrite_statement(*std::get<ir::DoWhileStmt>(stmt).body, labels);
      return;
    case ir::Kind_statement::ForStmt:
      rewrite_statement(*std::get<ir::ForStmt>(stmt).body, labels);
      return;
    case ir::Kind_statement::SwitchStmt:
      rewrite_statement(*std::get<ir::SwitchStmt>(stmt).body, labels);
      return;
    case ir::Kind_statement::CaseStmt:
      rewrite_statement(*std::get<ir::CaseStmt>(stmt).body, labels);
      return;
    case ir::Kind_statement::DefaultStmt:
      rewrite_statement(*std::get<ir::DefaultStmt>(stmt).body, labels);
      return;
    default:
      // A return, an expression, a break, a continue, and a null
      // statement name no label and hold no statement.
      return;
  }
}

}  // namespace

c99_ast::Program resolve_labels(c99_ast::Program prog) {
  // Only a function's body can hold labels. Objects, struct and union
  // declarations, and functions without a body pass through.
  for (auto& decl : prog.declaration) {
    auto* fd = std::get_if<ir::FunctionDecl>(&decl);
    if (fd == nullptr || !fd->function_decl->body) {
      continue;
    }
    auto& f = *fd->function_decl;
    // The first walk collects the labels, and the second renames them
    // and checks each `goto`.
    Labels labels;
    collect_block(**f.body, f.name, labels);
    rewrite_block(**f.body, labels);
  }
  return prog;
}

}  // namespace c6502::passes
