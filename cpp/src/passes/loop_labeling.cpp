/*
The loop and switch labeling pass, c99_ast to c99_ast.

It mints a unique label for each iteration statement (`while`,
`do-while`, `for`) and each `switch`, stamps it onto the statement's
`label` field, and stamps the matching label onto every `break` and
`continue` whose target is that statement. It also collects the case
and default labels of each switch into the SwitchStmt's `cases` and
`default_label` fields, so that the TAC translator can emit the dispatch
chain without walking the body again.

Per C99 §6.8.6.3 a `break` targets the innermost enclosing iteration or
switch statement, and per §6.8.6.2 a `continue` targets only an
iteration statement. So two pieces of state are threaded through the
walk.

  current_loop          the innermost iteration statement's label, or
                        none, which resolves `continue`.
  current_break_target  the innermost iteration or switch statement's
                        label, or none, which resolves `break`.

Iteration statements set both, and a switch sets only
`current_break_target`, so a `continue` inside a switch inside a loop
still finds the loop. The third piece of state is `current_switch`, the
innermost enclosing switch's collector, which receives the case and
default labels of its switch. Case labels can sit inside if, loop, and
compound bodies (Duff's device), all of which keep `current_switch`, and
only a nested switch starts a new collector for its own body.

The errors (`LoopLabelingError`) are a `break` outside any iteration or
switch statement, a `continue` outside any iteration statement
(including inside a switch with no enclosing loop), a `case` or a
`default` outside any switch, and a second `default` in one switch.
That last check is structural. The uniqueness of the case values is the
type checker's job.

The labels are `.loop@<N>` for an iteration statement, `.switch@<N>` for
a switch, `.case@<N>` for a case, and `.default@<N>` for a default. All
four share the program's counter, so the labels are unique in the
program. Each is minted before the statement's body is walked, so the
numbers follow the order of the walk. The leading `.` makes them dasm
local labels, scoped to the SUBROUTINE, and the `@` separator, which is
illegal in a C identifier, keeps them apart from any user-written label,
which label_resolution spells `.<funcname>@<orig>` with a C identifier
after the `@`.

Code generation derives the labels of an iteration statement by
appending `_start`, `_continue`, and `_break` to its label, and a switch
uses only `_break`, since the dispatch chain emits the case and default
labels directly.

The pass runs after label_resolution, so the user's labels are already
resolved when it mints its own. It labels the program it is given,
which it takes by value.
*/
#include "passes/loop_labeling.h"

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "errors.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::c99_ast;

struct SwitchCollector {
  std::vector<ir::Type_switch_case> cases;
  std::optional<std::string> default_label;
};

struct LabelState {
  std::optional<std::string> current_loop;
  std::optional<std::string> current_break_target;
  SwitchCollector* current_switch = nullptr;
};

[[noreturn]] void fail(const std::string& message) {
  throw CompileError("LoopLabelingError", message);
}

class LoopLabeler {
 public:
  ir::Program label_program(ir::Program prog) {
    for (auto& decl : prog.declaration) {
      if (auto* fd = std::get_if<ir::FunctionDecl>(&decl)) {
        if (fd->function_decl->body) {
          label_block(**fd->function_decl->body, LabelState{});
        }
      }
    }
    return prog;
  }

 private:
  int counter_ = 0;

  std::string make_label(const char* prefix) {
    std::string name = ".";
    name += prefix;
    name += "@";
    name += std::to_string(counter_);
    ++counter_;
    return name;
  }

  void label_block(ir::Type_block& block, const LabelState& state) {
    for (auto& item : std::get<ir::Block>(block).block_item) {
      if (auto* s = std::get_if<ir::S>(&item)) {
        label_statement(*s->statement, state);
      }
    }
  }

  static LabelState loop_state(const std::string& label, const LabelState& state) {
    return LabelState{
        .current_loop = label,
        .current_break_target = label,
        .current_switch = state.current_switch,
    };
  }

  void label_statement(ir::Type_statement& stmt, const LabelState& state) {
    switch (ir::kind(stmt)) {
      case ir::Kind_statement::BreakStmt: {
        if (!state.current_break_target) {
          fail("'break' statement not inside a loop or switch");
        }
        std::get<ir::BreakStmt>(stmt).label = *state.current_break_target;
        return;
      }
      case ir::Kind_statement::ContinueStmt: {
        if (!state.current_loop) {
          fail("'continue' statement not inside a loop");
        }
        std::get<ir::ContinueStmt>(stmt).label = *state.current_loop;
        return;
      }
      case ir::Kind_statement::WhileStmt: {
        auto& w = std::get<ir::WhileStmt>(stmt);
        const std::string label = make_label("loop");
        label_statement(*w.body, loop_state(label, state));
        w.label = label;
        return;
      }
      case ir::Kind_statement::DoWhileStmt: {
        auto& d = std::get<ir::DoWhileStmt>(stmt);
        const std::string label = make_label("loop");
        label_statement(*d.body, loop_state(label, state));
        d.label = label;
        return;
      }
      case ir::Kind_statement::ForStmt: {
        auto& f = std::get<ir::ForStmt>(stmt);
        const std::string label = make_label("loop");
        label_statement(*f.body, loop_state(label, state));
        f.label = label;
        return;
      }
      case ir::Kind_statement::SwitchStmt: {
        auto& sw = std::get<ir::SwitchStmt>(stmt);
        const std::string label = make_label("switch");
        SwitchCollector collector;
        const LabelState inner{
            .current_loop = state.current_loop,
            .current_break_target = label,
            .current_switch = &collector,
        };
        label_statement(*sw.body, inner);
        sw.label = label;
        sw.cases = std::move(collector.cases);
        sw.default_label = std::move(collector.default_label);
        return;
      }
      case ir::Kind_statement::CaseStmt: {
        auto& c = std::get<ir::CaseStmt>(stmt);
        if (state.current_switch == nullptr) {
          fail("'case' label not within a switch statement");
        }
        const std::string label = make_label("case");
        state.current_switch->cases.push_back(ir::Type_switch_case{.value = c.value, .label = label});
        label_statement(*c.body, state);
        c.label = label;
        return;
      }
      case ir::Kind_statement::DefaultStmt: {
        auto& d = std::get<ir::DefaultStmt>(stmt);
        if (state.current_switch == nullptr) {
          fail("'default' label not within a switch statement");
        }
        if (state.current_switch->default_label) {
          fail("multiple default labels in one switch");
        }
        const std::string label = make_label("default");
        state.current_switch->default_label = label;
        label_statement(*d.body, state);
        d.label = label;
        return;
      }
      case ir::Kind_statement::IfStmt: {
        auto& i = std::get<ir::IfStmt>(stmt);
        label_statement(*i.then_clause, state);
        if (i.else_clause) {
          label_statement(**i.else_clause, state);
        }
        return;
      }
      case ir::Kind_statement::Compound:
        label_block(*std::get<ir::Compound>(stmt).block, state);
        return;
      case ir::Kind_statement::LabeledStmt:
        label_statement(*std::get<ir::LabeledStmt>(stmt).statement, state);
        return;
      case ir::Kind_statement::Return:
      case ir::Kind_statement::Expression:
      case ir::Kind_statement::Goto:
      case ir::Kind_statement::Null:
        return;
    }
  }
};

}  // namespace

c99_ast::Program label_program(c99_ast::Program prog) {
  return LoopLabeler().label_program(std::move(prog));
}

}  // namespace c6502::passes
